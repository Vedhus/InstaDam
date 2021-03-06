#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>

#include "rectangleSelect.h"
#include "label.h"

#define PI 3.14159265

/*!
  \class RectangleSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QGraphicsRectItem
  \brief The RectangleSelect class provides a class for annotating rectangular
         regions.

  Provides a class for annotating rectangular regions in InstaDam. The region is
  described by its top-left and lower-right vertices, and by its angle of
  rotation.
  */

/*!
  Constructs a RectangleSelect object with all vertices at 0,0 in scene
  coordinates
  */
RectangleSelect::RectangleSelect() : RectangleSelect(QPointF(0., 0.)) {
}

/*!
  Constructs a RectangleSelect object by reading a QJsonObject and setting the
  internal rectangle and rotation angle to the values given in \a json. \a label
  is the Label which owns this object and \a item is the parent QGraphicsItem,
  if any.
  */
RectangleSelect::RectangleSelect(const QJsonObject &json,
                                 QSharedPointer<Label> label,
                                 QGraphicsItem *item)
    : BoxBasedSelector(json, label, item), QGraphicsRectItem(item) {
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs a RectangleSelect object by setting all vertices to be a \a point,
  \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
RectangleSelect::RectangleSelect(QPointF point, QSharedPointer<Label> label,
                                 QGraphicsItem *item)
    : BoxBasedSelector(point, label, item), QGraphicsRectItem(item) {
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
 * Sets the opacity of the label to \a val
 */
void RectangleSelect::setOpacity(qreal val) {
    SelectItem::setOpacity(val);
}

/*!
 * Sets the position Z vlaue to \a val
*/
void RectangleSelect::setZValue(int val) {
    SelectItem::setZValue(val);
}

/*!
  Constructs a RectangleSelect object by setting all vertices to be a \a point,
  \a vertSize indicates the size of the vertex highlight boxes, \a label is the
  Label which owns this object and \a item is the parent QGraphicsItem, if any.
  */
RectangleSelect::RectangleSelect(QPointF point, qreal vertSize,
                                 QSharedPointer<Label> label,
                                 QGraphicsItem *item)
    : BoxBasedSelector(point, vertSize, label, item), QGraphicsRectItem(item) {
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    invertColorForPen();
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Destructor
  */
RectangleSelect::~RectangleSelect() {
}

/*---------------------- Overrides ---------------------------*/
/*!
  \reimp
  */
void RectangleSelect::addPoint(QPointF &point, const int vertex) {
    UNUSED(vertex);
    sortCorners(myRect, point);
    calcCorners();

    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
    if (mirror != nullptr)
        mirror->setInitial(myRect, SelectItem::UNSELECTED);
}

/*!
  \reimp
  */
QRectF RectangleSelect::boundingRect() const {
    return QRectF(0., 0., SelectItem::myBounds.width(),
                  SelectItem::myBounds.height());
}

/*!
  \reimp
  */
bool RectangleSelect::isInside(const QPointF &point) const {
    return QGraphicsRectItem::contains(BoxBasedSelector::mapFromScene(point));
}

/*!
  \reimp
  */
void RectangleSelect::moveItem(const QPointF &oldPos, QPointF &newPos) {
    // if there is an active vertex then we are resizing
    if (activeVertex != 0 && activeVertex != UNSELECTED) {
        QPointF newPoint = BoxBasedSelector::mapToScene(getActivePoint()) + (newPos - oldPos);
        addPoint(newPoint);
        resized = true;
        setMirrorResized();
        //calcCorners();
        setRect(myRect);
        if (mirror != nullptr)
            mirror->setInitial(getRect(), SelectItem::UNSELECTED);
    } else {
        // otherwise we are moving the entire object
        QPointF shift = newPos - oldPos;
        //QRectF temp = myRect;
        //checkBoundaries(shift, temp);
        //qreal x = temp.x() - myRect.x();
        //qreal y = temp.y() - myRect.y();
        BoxBasedSelector::setTransform(QTransform::fromTranslate(shift.x(), shift.y()), true);
        myRect = getRect();
        calcCorners();
        if (mirror != nullptr)
            mirror->BoxBasedSelector::setTransform(QTransform::fromTranslate(shift.x(), shift.y()), true);
        moved = true;
        setMirrorMoved();
    }
    QGraphicsRectItem::prepareGeometryChange();
}


/*!
  \reimp
  */
void RectangleSelect::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget) {
    QGraphicsRectItem::paint(painter, option, widget);
    if (active) {
        painter->setBrush(brush());
        QColor col = painter->pen().color();
        painter->drawRect(tl);
        painter->drawRect(bl);
        painter->drawRect(tr);
        painter->drawRect(br);
        painter->setPen(col);
    }
}

/*!
  \reimp
  */
void RectangleSelect::updatePen(QPen pen) {
    setPen(pen);
    QColor col = pen.color();
    col.setAlphaF(0.25);
    QBrush brush(col);
    setBrush(brush);
}

/*!
  \reimp
  */
void RectangleSelect::setInitial(QRectF rect, int actVertex) {
    QGraphicsRectItem::prepareGeometryChange();
    myRect = rect;
    setRect(myRect);
    calcCorners();
}

/*!
  \reimp
  */
void RectangleSelect::toPixmap(QPainter *painter) {
    QPointF ttl = BoxBasedSelector::mapToScene(tl.topLeft());
    QPointF tbr = BoxBasedSelector::mapToScene(br.bottomRight());
    QPointF center = (ttl + tbr) / 2.;
    QSizeF size = QSizeF(abs(tl.topLeft().x() - br.bottomRight().x()),
                         abs(tl.topLeft().y() - br.bottomRight().y()));
    QPointF tlc;
    painter->translate(center);
    painter->rotate(getRotationAngle());
    painter->translate(-center);
    tlc.setX(center.x() - (size.width()/2.));
    tlc.setY(center.y() - (size.height()/2.));
    painter->drawRect(QRectF(tlc, size));
    painter->resetTransform();
}
/*---------------------------- Mirror functions ----------------------*/
/*!
  \reimp
  */
void RectangleSelect::mirrorHide() const {
    if (mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void RectangleSelect::mirrorShow() const {
    if (mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void RectangleSelect::rotateMirror() const {
    if (mirror != nullptr) {
        mirror->myRotation = myRotation;
        mirror->BoxBasedSelector::setTransformOriginPoint(myRect.center());
        mirror->BoxBasedSelector::setRotation(myRotation);
    }
}

/*!
  \reimp
  */
void RectangleSelect::setMirror(SelectItem *item) {
    mirror = dynamic_cast<RectangleSelect*>(item);
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorActive() const {
    if (mirror != nullptr)
        mirror->active = true;
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc,
                                       QRectF brc) const {
    if (mirror != nullptr) {
        mirror->tl = tlc;
        mirror->bl = blc;
        mirror->tr = trc;
        mirror->br = brc;
    }
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorMoved() const {
    if (mirror != nullptr)
        mirror->moved = moved;
}


/*!
  \reimp
  */
void RectangleSelect::setMirrorResized() const {
    if (mirror != nullptr)
        mirror->resized = resized;
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorVertex(int vertex) const {
    if (mirror != nullptr) {
        mirror->setActiveVertex(vertex);
    }
}

/*!
  \reimp
  */
void RectangleSelect::updateMirrorScene() const {
    if (mirror != nullptr)
        mirror->scene()->update();
}

/*!
  \overload SelectItem::scene()

  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* RectangleSelect::scene() {
    return SelectItem::scene();
}

/*!
  \fn QString RectangleSelect::baseInstructions() const

  \reimp
  */

/*!
  \fn RectangleSelect* RectangleSelect::getMirror() const

  \reimp
  */

/*!
  \fn bool RectangleSelect::isVisible()

  Returns whether the RectangleSelect object is visible (\c true) on the QGraphicsScene or not (\c false).
  */

/*!
  \fn void RectangleSelect::setMirrorAdded() const

  \reimp
  */

/*!
  \fn int RectangleSelect::type() const
  \overload SelectItem::type()

  This function returns the type of this item.

  \sa QGraphicsItem::type(), SelectItem::type()
  */


