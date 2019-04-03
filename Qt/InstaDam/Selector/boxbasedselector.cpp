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
    calcCorners();
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
    if (isInsideRect(tl, point)) {
        setActiveVertex(TOP, LEFT);
        activePoint = myRect.topLeft();
    } else if (isInsideRect(tr, point)) {
        setActiveVertex(TOP, RIGHT);
        activePoint = myRect.topRight();
    } else if (isInsideRect(bl, point)) {
        setActiveVertex(BOTTOM, LEFT);
        activePoint = myRect.bottomLeft();
    } else if (isInsideRect(br, point)) {
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
void BoxBasedSelector::resizeItem(const int vertex, QPointF &newPos) {
    setActiveVertex(vertex);
    setMirrorVertex(getActiveVertex());
    addPoint(newPos);
}

/*!
  \reimp
  */
void BoxBasedSelector::rotate(const QPointF &from, const QPointF &to) {
    setTransformOriginPoint(myRect.center());
    QPointF start = from - myRect.center();
    QPointF end = to - myRect.center();
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
    QPointF tlc = QPointF(json["left"].toDouble(), json["top"].toDouble());
    QPointF brc = QPointF(json["right"].toDouble(), json["bottom"].toDouble());
    myRect = QRectF(tlc, brc);
    myRotation = json["rotation"].toDouble();
    myID = json["objectID"].toInt();
    setTransformOriginPoint(myRect.center());
    setRotation(myRotation);
}

/*!
  \reimp
  */
void BoxBasedSelector::write(QJsonObject &json) const {
    json["objectID"] = myID;
    json["rotation"] = myRotation;
    json["left"] = myRect.left();
    json["right"] = myRect.right();
    json["top"] = myRect.top();
    json["bottom"] = myRect.bottom();
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
  \fn virtual void BoxBasedSelector::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) = 0;

  Pure virtual function to set the corner vertex boxes of the mirror object.
  \a tlc is the top-left corner, \a blc is the bottom-left corner, \a trc is the
  top-right corner, and \a brc is the bottom right corner.
  */

/*!
  \fn virtual void BoxBasedSelector::setRectUnchecked(QRectF rect) = 0;

  Pure virtual function to set the internal rectangle of the BoxBasedSelector to
  \a rect without checking to see if it completely fits in the QGraphicsScene.
  This is used when setting the internal rectangle of the mirror after checking
  has been done in this instance.
  */

/*!
  \fn void BoxBasedSelector::insertVertex(int vertex, QPointF &point)
  \reimp

  Empty function as this and any derived classes can have two, and only two
  vertices.
  */

/*!
  \fn int BoxBasedSelector::numberOfVertices()

  \reimp

  Always returns 2, as this and any derived classes can have two, and only two
  vertices.
  */

/*!
  \fn void BoxBasedSelector::setRotationAngle(qreal angle)

  Sets the rotation angle of the BoxBasedSelector to \a angle degrees, measured
  from the horizontal going counterclockwise.

  \sa getRotationAngle()
  */

/*!
  \fn qreal BoxBasedSelector::getRotationAngle()

  Returns a \c qreal containing the rotation angle of the BoxBasedSelector in
  degrees.

  \sa setRotationAngle()
  */

/*!
  \fn QRectF BoxBasedSelector::getRect()

  Returns a QRectF which denotes the bounding rectanlge of the BoxBasedSelector.
  */
