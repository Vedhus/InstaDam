#include "ellipseSelect.h"

#include <QPainter>
#include <QGraphicsScene>

#include <iostream>
#include <cmath>

#include "label.h"

#define PI 3.14159265

/*!
  \class EllipseSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QGraphicsEllipseItem
  \brief The EllipseSelect class provides a class for annotating elliptical
         regions.

  Provides a class for annotating elliptical regions in InstaDam. The region is
  described by its top-left and lower-right vertices, and by its angle of
  rotation.
  */

/*!
  Constructs an EllipseSelect object with all vertices at 0,0 in scene
  coordinates
  */
EllipseSelect::EllipseSelect() : EllipseSelect(QPointF(0., 0.)) {
    mirror = nullptr;
}

/*!
  Constructs an EllipseSelect object by reading a QJsonObject and setting the
  internal rectangle and rotation angle to the values given in \a json. \a label
  is the Label which owns this object and \a item is the parent QGraphicsItem,
  if any.
  */
EllipseSelect::EllipseSelect(const QJsonObject &json,
                             QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(json, label, item) {
    mirror = nullptr;
    setRect(myRect);
    mytype = SelectItem::Ellipse;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    calcCorners();
    QGraphicsEllipseItem::prepareGeometryChange();
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs an EllipseSelect object by setting all vertices to be a \a point,
  \a label is the Label which owns this object and \a item is the parent
  QGraphicsItem, if any.
  */
EllipseSelect::EllipseSelect(QPointF point, QSharedPointer<Label> label,
                             QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, label, item) {
    mirror = nullptr;
    setRect(myRect);
    mytype = SelectItem::Ellipse;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    invertColorForPen();
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}
/*!
 * Sets the opacity of the label to \a val
*/
void EllipseSelect::setOpacity(qreal val) {
    SelectItem::setOpacity(val);
}

/*!
  Constructs an EllipseSelect object by setting all vertices to be a \a point,
  \a vertexSize indicates the size of the vertex highlight boxes, \a label is
  the Label which owns this object and \a item is the parent QGraphicsItem, if
  any.
  */
EllipseSelect::EllipseSelect(QPointF point, qreal vertexSize,
                             QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsEllipseItem(item),
      BoxBasedSelector(point, vertexSize, label, item) {
    setRect(myRect);
    mytype = SelectItem::Ellipse;
    updatePen(myPen);
    if (label)
        label->addItem(this);

    setPen(BoxBasedSelector::pen);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Destructor
  */
EllipseSelect::~EllipseSelect() {
}

/*------------------------- Overrides ----------------------------*/
/*!
  \reimp
  */
void EllipseSelect::addPoint(QPointF &point, const int vertex) {
    UNUSED(vertex);
    sortCorners(myRect, point);
    calcCorners();

    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);

    if (mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

/*!
  \reimp
  */
QRectF EllipseSelect::boundingRect() const {
    return QRectF(0., 0., SelectItem::myBounds.width(),
                  SelectItem::myBounds.height());
}

/*!
  \reimp
  */
bool EllipseSelect::isInside(const QPointF &point) const {
    QPointF center = BoxBasedSelector::mapToScene((br.bottomLeft() + tl.topRight()) / 2.);
    QPointF myPoint = BoxBasedSelector::mapFromScene(point);
    if ((std::pow(point.x() - center.x(), 2)/std::pow(rect().width()/2., 2)) +
        (std::pow(point.y() - center.y(), 2)/std::pow(rect().height()/2., 2))
        <= 1.) {
        return true;
    } else if (active && (isInsideRect(tl, myPoint) || isInsideRect(tr, myPoint) ||
                          isInsideRect(bl, myPoint) || isInsideRect(br, myPoint))) {
        return true;
    }
    return false;
}

/*!
  \reimp
  */
void EllipseSelect::moveItem(const QPointF &oldPos, QPointF &newPos) {
    // if there is an active vertex then we are resizing
    if (activeVertex != 0) {
        QPointF newPoint = BoxBasedSelector::mapToScene(getActivePoint()) + (newPos - oldPos);
        addPoint(newPoint);
        resized = true;
        setMirrorResized();
        //calcCorners();
        setRect(myRect);
        if (mirror != nullptr)
            mirror->setRectUnchecked(myRect);
    } else {
        // otherwise we are moving the entire object
        QPointF shift = newPos - oldPos;
        //checkBoundaries(shift, myRect);
        BoxBasedSelector::setTransform(QTransform::fromTranslate(shift.x(), shift.y()), true);
        calcCorners();
        myRect = getRect();
        if (mirror != nullptr)
            mirror->BoxBasedSelector::setTransform(QTransform::fromTranslate(shift.x(), shift.y()), true);
        moved = true;
        setMirrorMoved();
    }
    QGraphicsEllipseItem::prepareGeometryChange();
}

/*!
  \reimp
  */
void EllipseSelect::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget) {
    QGraphicsEllipseItem::paint(painter, option, widget);
    if (active) {
        painter->setBrush(brush());
        painter->drawRect(tl);
        painter->drawRect(bl);
        painter->drawRect(tr);
        painter->drawRect(br);
    }
}

/*!
  \reimp
  */
void EllipseSelect::setRectUnchecked(QRectF rect) {
    QGraphicsEllipseItem::prepareGeometryChange();
    myRect = rect;
    setRect(myRect);
    calcCorners();
}

/*!
  \reimp
  */
void EllipseSelect::toPixmap(QPainter *painter) {
    painter->translate(myRect.center());
    painter->rotate(getRotationAngle());
    painter->translate(-myRect.center());
    painter->drawEllipse(myRect);
    painter->resetTransform();
}

/*!
  \reimp
  */
void EllipseSelect::updatePen(QPen pen) {
    setPen(pen);
    QColor col = pen.color();
    col.setAlphaF(0.25);
    QBrush brush(col);
    setBrush(brush);
}

/*---------------------------------- Mirror --------------------------*/
/*!
  \reimp
  */
void EllipseSelect::mirrorHide() const {
    if (mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void EllipseSelect::mirrorShow() const {
    if (mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void EllipseSelect::rotateMirror() const {
    if (mirror != nullptr) {
        mirror->myRotation = myRotation;
        mirror->BoxBasedSelector::setTransformOriginPoint(myRect.center());
        mirror->BoxBasedSelector::setRotation(myRotation);
    }
}

/*!
  \reimp
  */
void EllipseSelect::setMirror(SelectItem *item) {
    mirror = dynamic_cast<EllipseSelect*>(item);
}

/*!
  \reimp
  */
void EllipseSelect::setMirrorActive() const {
    if (mirror != nullptr)
        mirror->active = true;
}

/*!
  \reimp
  */
void EllipseSelect::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc,
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
void EllipseSelect::setMirrorMoved() const {
    if (mirror != nullptr)
        mirror->moved = moved;
}
/*!
  \reimp
  */
void EllipseSelect::setMirrorResized() const {
    if (mirror != nullptr)
        mirror->resized = resized;
}
/*!
  \reimp
  */
void EllipseSelect::setMirrorVertex(int vertex) const {
    if (mirror != nullptr)
        mirror->setActiveVertex(vertex);
}

/*!
  \reimp
  */
void EllipseSelect::updateMirrorScene() const {
    if (mirror != nullptr)
        mirror->scene()->update();
}
/*--------------------------------------- End Mirror ------------------*/

/*!
  \overload SelectItem::scene()

  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* EllipseSelect::scene() {
    return SelectItem::scene();
}

/*!
  \fn QString EllipseSelect::baseInstructions() const

  \reimp
  */

/*!
  \fn EllipseSelect* EllipseSelect::getMirror() const

  \reimp
  */

/*!
  \fn bool EllipseSelect::isVisible() const

  Returns whether the EllipseSelect object is visible (\c true) on the
  QGraphicsScene or not (\c false).
  */

/*!
  \fn void EllipseSelect::setMirrorAdded() const

  \reimp
  */

/*!
  \fn int EllipseSelect::type() const
  \overload SelectItem::type()

  This function returns the type of this item.

  \sa QGraphicsItem::type(), SelectItem::type()
  */
