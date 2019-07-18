#include <math.h>
#include <QPainter>
#include <iostream>

#include "boxbasedselector.h"

#define PI 3.14159265

/*!
  \class BoxBasedSelector
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem
  \brief The BoxBasedSelector class provides a base class for SelectItem
         subclasses that can be described with a box.

  The BoxBasedSelector class provides a base class for SelectItem subclasses
  that can be described with a box, such as RectangleSelect and EllipseSelect.
  It implements several of the virtual functions from SelectItem which will
  have identical implementations on all its subclasses:

  \list
  \li RectangleSelect
  \li EllipseSelect
  \endlist

  */

/*!
  Constructs a BoxBasedSelector object by reading a QJsonObject and setting the
  internal rectangle and rotation angle to the values given in \a json. \a label
  is the Label which owns this object and \a item is the parent QGraphicsItem,
  if any.
  */
BoxBasedSelector::BoxBasedSelector(const QJsonObject &json,
                                   QSharedPointer<Label> label,
                                   QGraphicsItem *item) :
    SelectItem(label, item) {
    read(json);
}

/*!
  Constructs a BoxBasedSelector object by setting all vertices to be a \a point,
  \a label is the Label which owns this object and \a item is the parent
  QGraphicsItem, if any.
  */
BoxBasedSelector::BoxBasedSelector(QPointF point, QSharedPointer<Label> label,
                                   QGraphicsItem *item) :
    SelectItem(label, item) {
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners(false);
    activePoint = point;
    active = true;
}

/*!
  Constructs a BoxBasedSelector object by setting all vertices to be a \a point,
  \a vertSize indicates the size of the vertex highlight boxes, \a label is the
  Label which owns this object and \a item is the parent QGraphicsItem, if any.
  */
BoxBasedSelector::BoxBasedSelector(QPointF point, qreal vertSize,
                                   QSharedPointer<Label> label,
                                   QGraphicsItem *item) :
    SelectItem(vertSize, label, item) {
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners(false);
    activePoint = point;
    active = true;
}

/*!
  Destructor
  */
BoxBasedSelector::~BoxBasedSelector() {
}

/*-------------------------- Overrides -------------------------*/
/*!
  \reimp
  */
void BoxBasedSelector::clickPoint(const QPointF &point) {
    active = true;

    if (isInsideRect(tl, mapFromScene(point))) {
        setActiveVertex(TOP, LEFT);
        activePoint = myRect.topLeft();
    } else if (isInsideRect(tr, mapFromScene(point))) {
        setActiveVertex(TOP, RIGHT);
        activePoint = myRect.topRight();
    } else if (isInsideRect(bl, mapFromScene(point))) {
        setActiveVertex(BOTTOM, LEFT);
        activePoint = myRect.bottomLeft();
    } else if (isInsideRect(br, mapFromScene(point))) {
        setActiveVertex(BOTTOM, RIGHT);
        activePoint = myRect.bottomRight();
    } else {
        setActiveVertex(0, 0);
    }
    setMirrorVertex(getActiveVertex());
}

/*!
  \reimp
  */
void BoxBasedSelector::removeVertex(int vertex) {
    UNUSED(vertex);
}

/*!
  \reimp
  */
void BoxBasedSelector::resetActiveVertex() {
    setActiveVertex(0);
}

/*!
  \reimp
  */
void BoxBasedSelector::resizeItem(const int vertex, QPointF &oldP, QPointF &newP) {
    setActiveVertex(vertex);
    setMirrorVertex(getActiveVertex());

    if (activeVertex & TOP) {
        if (activeVertex & LEFT) {
            activePoint = myRect.topLeft();
        } else {
            activePoint = myRect.topRight();
        }
    } else if (activeVertex & BOTTOM) {
        if (activeVertex & RIGHT) {
            activePoint = myRect.bottomRight();
        } else {
            activePoint = myRect.bottomLeft();
        }
    }
    QPointF newPoint = mapToScene(getActivePoint()) + (newP - oldP);

    addPoint(newPoint);
}

/*!
  \reimp
  */
void BoxBasedSelector::rotate(const QPointF &from, const QPointF &to) {
    QPointF center = (br.bottomRight() + tl.topLeft()) / 2.;
    setTransformOriginPoint(center);
    QPointF start = from - mapToScene(center);
    QPointF end = to - mapToScene(center);
    myRotation += (atan2(end.y(), end.x()) -
                   atan2(start.y(), start.x())) * 180. / PI;
    setRotation(myRotation);
    rotated = true;
    rotateMirror();
}

/*!
  \reimp
  */
void BoxBasedSelector::read(const QJsonObject &json) {
    QPointF center = QPointF(json[InstaDamJson::CENTERX].toDouble(), json[InstaDamJson::CENTERY].toDouble());
    QSizeF size = QSizeF(json[InstaDamJson::WIDTH].toDouble(), json[InstaDamJson::HEIGHT].toDouble());
    center.setX(center.x() - (size.width()/2.));
    center.setY(center.y() - (size.height()/2.));
    myRect = QRectF(center, size);
    myRotation = json[InstaDamJson::ROTATION].toDouble();
    myID = json[InstaDamJson::OBJECTID].toInt();
    setTransformOriginPoint(myRect.center());
    setRotation(myRotation);
}

/*!
  \reimp
  */
void BoxBasedSelector::write(QJsonObject &json) const {
    json[InstaDamJson::OBJECTID] = myID;
    json[InstaDamJson::ROTATION] = myRotation;
    QPointF ttl = mapToScene(tl.topLeft());
    QPointF tbr = mapToScene(br.bottomRight());
    QPointF center = (ttl + tbr) / 2.;
    json[InstaDamJson::CENTERX] = center.x();
    json[InstaDamJson::CENTERY] = center.y();
    json[InstaDamJson::WIDTH] = abs(tl.topLeft().x() - br.bottomRight().x());
    json[InstaDamJson::HEIGHT] = abs(tl.topLeft().y() - br.bottomRight().y());
}

/*-------------------------- Protected ------------------------*/
/*!
  Calculates the vertex boxes in the corners of the defining rectangle. The
  boxes are used to indicate the BoxBasedSelector is active and to act as points
  to click and drag to resize the item. If the mirror needs to be updated as
  well the \a mir should be \c true.
  */
void BoxBasedSelector::calcCorners(bool mir) {
    tl = QRectF(myRect.topLeft(), myRect.topLeft() + SelectItem::xoffset +
                SelectItem::yoffset);
    bl = QRectF(myRect.bottomLeft() - SelectItem::yoffset, myRect.bottomLeft() +
                SelectItem::xoffset);
    tr = QRectF(myRect.topRight() - SelectItem::xoffset, myRect.topRight() +
                SelectItem::yoffset);
    br = QRectF(myRect.bottomRight() - SelectItem::xoffset -
                SelectItem::yoffset, myRect.bottomRight());
    if (mir)
        setMirrorCorners(tl, bl, tr, br);
}

/*!
  \fn virtual void BoxBasedSelector::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) const = 0;

  Pure virtual function to set the corner vertex boxes of the mirror object.
  \a tlc is the top-left corner, \a blc is the bottom-left corner, \a trc is the
  top-right corner, and \a brc is the bottom right corner.
  */



/*!
  \fn void BoxBasedSelector::insertVertex(const int vertex, const QPointF &point)
  \reimp

  Empty function as this and any derived classes can have two, and only two
  vertices.
  */

/*!
  \fn int BoxBasedSelector::numberOfVertices() const

  \reimp

  Always returns 2, as this and any derived classes can have two, and only two
  vertices.
  */

/*!
  \fn void BoxBasedSelector::setInitial(QRectF rect, int actVertex)
  \reimp

  */

/*!
  \fn void BoxBasedSelector::setRotationAngle(qreal angle)

  Sets the rotation angle of the BoxBasedSelector to \a angle degrees, measured
  from the horizontal going counterclockwise.

  \sa getRotationAngle()
  */

/*!
  \fn qreal BoxBasedSelector::getRotationAngle() const

  Returns a \c qreal containing the rotation angle of the BoxBasedSelector in
  degrees.

  \sa setRotationAngle()
  */


