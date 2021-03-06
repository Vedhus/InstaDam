#include <QtWidgets>
#include <QPointF>
#include <algorithm>
#include <iostream>

#include "selectItem.h"
#include "label.h"
/*!
  \class SelectItem
  \ingroup Selector
  \inmodule InstaDam
  \inherits QGraphicsItem
  \brief The SelectItem class provides a base class for all annotating items.

  Provides a foundation for all annotation type objects. Common funtions that
  do not depend on the type of object are implemented in this class, but every
  subclass of this class must implement all virtual functions.

  Current subclasses are:

  \list
  \li RectangleSelect provides selection fo rectanular areas
  \li EllipseSelect provides selection of elliptical regions
  \li PolygonSelect provides selection of irregularly shaped regions
  \li FreeDrawSelect provides selection of regions by brush stroke
  \li FreeDrawErase is used to erase sections of FreeDrawSelect regions
  \endlist

  SelectItem subclasses must have a Label and a PhotoScene owning them.
*/
qreal SelectItem::vertexSize = 10.;
QPointF SelectItem::xoffset = QPointF(SelectItem::vertexSize, 0.);
QPointF SelectItem::yoffset = QPointF(0., SelectItem::vertexSize);
int SelectItem::ID = 0;
QSize SelectItem::myBounds = QSize(0, 0);

/*!
   Constructs a SelectItem, using \a vertSize as the vertex box size, \a label
   as the Label that owns this instance, and \a item as the parent
   QGraphicsItem.
*/
SelectItem::SelectItem(qreal vertSize, QSharedPointer<Label> label,
                       QGraphicsItem *item) : QGraphicsItem(item) {
    SelectItem::setVertexSize(vertSize);
    myID = SelectItem::ID;
    SelectItem::ID++;
    setLabel(label, true);
}

/*!
   Constructs a SelectItem, using \a label as the Label that owns this instance,
   and \a item as the parent QGraphicsItem.The vertex box size defaults to 10
   pixels.
*/
SelectItem::SelectItem(QSharedPointer<Label> label,
                       QGraphicsItem *item) :
    SelectItem(10., label, item) {
}

/*!
   Returns the QGraphicItem which is the parent to this item.
*/
QGraphicsItem* SelectItem::getParentItem() const {
    return parentItem();
}

/*!
  Determines what color is the "opposite" of the current pen color. This is used
  to find a color for highlighting vertices.
  */
void SelectItem::invertColorForPen() {
    QColor color = myPen.color();
    color.setRgbF(color.redF() > 0.5 ? 0 : 1, color.greenF() > 0.5 ? 0 : 1,
                  color.blueF() > 0.5 ? 0 : 1);
    highlightPen = QPen(color);
    highlightPen.setWidth(5);
}

/*!
  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* SelectItem::scene() const {
    return QGraphicsItem::scene();
}

/*!
  Sets the internal variables to associate this object with the given Label
  \a label. \a init is used to indicate whether this is an initialization of the
  base class or of a derived class.
  */
void SelectItem::setLabel(QSharedPointer<Label> label, bool init) {
    myLabel = label;
    if (label != nullptr) {
        myPen = QPen(label->getColor());
        myPen.setWidth(2);
        if (!init)
            updatePen(myPen);
    }
}

/*!
  Determines the corners of the vertex boxes for plotting by using \a size.
  */
void SelectItem::setVertexSize(qreal size) {
    SelectItem::vertexSize = size;
    SelectItem::xoffset = QPointF(SelectItem::vertexSize, 0.);
    SelectItem::yoffset = QPointF(0., SelectItem::vertexSize);
}

/*!
  Determines the bounding corners of a rectangle that is being resized. This is
  done by using the activeVertex member variable (indicating the currently
  selected vertex) along with the given QRect \a rect and QPointF \a newPoint to
  determine the position of the moving vertex as well as the new top left and
  bottom right corners of the rectangle.
*/
void SelectItem::sortCorners(QRectF &rect, QPointF &newPoint) {
    if (activeVertex & TOP) {
        if (activeVertex & LEFT) {
            rect.setTopLeft(mapFromScene(newPoint));
        } else {
            rect.setTopRight(mapFromScene(newPoint));
        }
    } else if (activeVertex & BOTTOM) {
        if (activeVertex & RIGHT) {
            rect.setBottomRight(mapFromScene(newPoint));
        } else {
            rect.setBottomLeft(mapFromScene(newPoint));
        }
    }
    activePoint = mapFromScene(newPoint);
    QPointF tlc = rect.topLeft();
    QPointF brc = rect.bottomRight();
    if (tlc.y() > brc.y()) {
        rect.setTop(brc.y());
        rect.setBottom(tlc.y());
        flipH();
    }
    if (tlc.x() > brc.x()) {
        rect.setLeft(brc.x());
        rect.setRight(tlc.x());
        flipV();
    }
}

/*!
  \overload QGraphicsItem::type() const

  This function returns the type of this item.

  \sa QGraphicsItem::type()
  */
int SelectItem::type() const {
    return mytype;
}

/*---------------- Protected members ---------------------------*/
/*!
  Checks the boundaries of the object being moved so that it is constrained to
  the canvas. \a rect is the bounding rectangle of the object and \a shift is
  the distance the rectangle is being moved. If the movement (\a rect +
  \a shift) would place one or more of the corners outside of the canvas the
  movement is halted at the last pixel before the vertex would disappear.
  */
void SelectItem::checkBoundaries(const QPointF &shift, QRectF &rect) {
    QPointF tlc = rect.topLeft() + shift;
    QPointF brc = rect.bottomRight() + shift;

    if (tlc.x() < 0 || tlc.y() < 0) {
        tlc.setY(std::max(tlc.y(), 0.));
        tlc.setX(std::max(tlc.x(), 0.));
        rect.moveTopLeft(tlc);
    } else if (brc.x() >= SelectItem::myBounds.width() ||
               brc.y() >= SelectItem::myBounds.height()) {
        brc.setY(std::min(static_cast<int>(brc.y()),
                          SelectItem::myBounds.height() - 1));
        brc.setX(std::min(static_cast<int>(brc.x()),
                          SelectItem::myBounds.width() - 1));
        rect.moveBottomRight(brc);
    } else {
        rect.moveTopLeft(tlc);
    }
}

/*!
  Determines whether \a point is inside of \a rect. Returns true if it is.
  */
bool SelectItem::isInsideRect(const QRectF &rect, const QPointF &point) const {
    if (point.y() >= rect.top() && point.y() <= rect.bottom()) {
        if (point.x() <= rect.right() && point.x() >= rect.left()) {
            return true;
        }
    }
    return false;
}

/*!
    \fn virtual void SelectItem::addPoint(QPointF &point, const int vertex = UNSELECTED) = 0

    This pure virtual function adds a point to object being drawn. It is
    up to the reimplimentation of this function to determine how to handle
    the input \a point  and \a vertex, as not all SelectItem subclasses have
    vertices.

    \sa removeVertex(), insertVertex()
*/
/*!
    \fn virtual void SelectItem::clickPoint(const QPointF &point) = 0

    This pure virtual function is used to signal an object that the mouse
    has been clicked within its boundaries. It is up to the individual
    reimplementations to determine what to do with the information. \a point
    indicated the coordinates (on the canvas) where the click took place.
*/
/*!
    \fn virtual void SelectItem::insertVertex(const int vertex, const QPointF &point) = 0

    This pure virtual function inserts a new vertex, given as \a point, at
    position \a vertex in the internal list of vertices. It is up to the
    reimplementation of this function to determine if and how this is done.

    \sa addPoint(), removeVertex()
*/
/*!
    \fn virtual void SelectItem::moveItem(const QPointF &oldPos, QPointF &newPos) = 0

    This pure virtual function moves the SelectItem on the canvas. \a oldPos
    indicates the position of the mouse at the begining of the move and
    \a newPos indicates the mouse position at the end of the move. It is up to
    the individual reimplementations to determine how this movement is
    interpreted and implemented.

    \sa rotate(), resizeItem()
*/
/*!
    \fn virtual void SelectItem::removeVertex(const int vertex = UNSELECTED) = 0

    This pure virtual function removes vertex \a vertex from the internal
    list of vertices. It is up to the individual reimplementations to determine
    if and how this is done.

    \sa addPoint(), insertVertex()
*/
/*!
    \fn virtual void SelectItem::resetActiveVertex() = 0

    This pure virtual function is used to reset the active vertex to the default
    value of SelectItem::UNSELECTED.
*/
/*!
    \fn virtual void SelectItem::resizeItem(const int vertex, QPointF &oldP, QPointF &newP) = 0

    This pure virtual function is used to resize the item. \a vertex indicates
    which vertex is moving from point \a oldP to point \a newP. It is up to the
    individual reimplementations to determine how to do this for each derived class.

    \sa moveItem(), rotate()
*/
/*!
    \fn virtual void SelectItem::rotate(const QPointF &from, const QPointF &to) = 0

    This pure virtual funcion is used to rotate a SelectItem. \a from indicates
    the starting position of the rotation and \a to indicates the end of the
    rotation in scene coordinates. It is up to the individual reimplementations
    to calculate the center of rotation and angle of rotation the SelectItem is
    undergoing.

    \sa moveItem(), resizeItem()
*/
/*!
    \fn virtual void SelectItem::updatePen(QPen pen) = 0

    This pure virtual function is used to update the QGraphicsItem's pen with
    \a pen.
*/
/*!
    \fn virtual void SelectItem::read(const QJsonObject &json) = 0

    This pure virtual function determines how QJsonObject \a json is converted
    into the appropriate SelectItem subclass. It is up to the individual
    reimplementations to determine what each QJsonObject attribute translates
    to. Any changes to this function must be mirrored in the write() function or
    data corruption can occur.

    \sa write()
*/
/*!
    \fn virtual void SelectItem::write(QJsonObject &json) const = 0

    This pure virtual function determines the SelectItem is converted into a
    QJsonObject representation \a json. It is up to the individual
    reimplementations to determine what attributes of the derived classes need
    to be saved in order to completely reconstruct the state of the current
    SelectItem. Any changes to this function must be mirrored in the read()
    function or data corruption can occur.

    \sa read()
*/
/*!
    \fn virtual QString SelectItem::baseInstructions() const = 0

    Returns a QString representing the instructions on how to draw this
    SelectItem type on the scene. It is up to the individual reimplementations
    to construct the string.
*/
/*!
    \fn virtual bool SelectItem::isInside(const QPointF &point) const = 0

    Returns a bool indicating whether \a point is inside the boundaries of the
    SelectItem. \c true indicates the QPointF is within the bounds, \c false
    otherwise. It is up to the individual reimplementations to determine how
    this is done.
*/
/*!
    \fn virtual int SelectItem::numberOfVertices() const = 0

    Returns an int indicating the number of vertices in the SelectItem. It is
    up to the individual reimplementations to do the calculation.
*/
/*!
    \fn virtual SelectItem* SelectItem::getMirror() const = 0

    Returns a SelectItem* to the mirror of this SelectItem. If this SelectItem
    is a member of the photoScene then it will return the corresponding
    member from the maskScene.
*/
/*!
    \fn virtual void SelectItem::mirrorHide() const = 0

    Pure virtual function to hide the mirror item on its scene.

    \sa QGraphicsItem::hide()
*/
/*!
    \fn virtual void SelectItem::mirrorShow() const = 0

    Pure virtual function to show the mirror item on its scene.

    \sa QGraphicsItem::show()
*/
/*!
    \fn virtual void SelectItem::rotateMirror() const = 0

    Pure virtual function to indicate rotation to the mirror SelectItem.
*/
/*!
    \fn virtual void SelectItem::setMirror(SelectItem *item) = 0

    Pure virtual function which sets the mirror for this object, as given by
    \a item. See the \l{mirroritems.html}{mirror documentation} for a
    description of mirrors.
*/
/*!
    \fn virtual void SelectItem::setMirrorActive() const = 0

    Pure virtual function which sets the mirror SelectItem to be active.
*/
/*!
    \fn virtual void SelectItem::setMirrorAdded() const = 0

    Pure virtual function which adds the mirror SelectItem to its corresponding
    scene.
*/
/*!
    \fn virtual void SelectItem::setMirrorMoved() const = 0

    Pure virtual function which sets the \c moved attribute of the SelectItem's
    mirror.
*/
/*!
    \fn virtual void SelectItem::setMirrorResized() const = 0

    Pure virtual function which sets the resized attribute of the SelectItem's
    mirror.
*/
/*!
    \fn virtual void SelectItem::setMirrorVertex(int vertex) const = 0

    Pure virtual function which sets the active vertex of the SelectItem's
    mirror to \a vertex.
*/
/*!
    \fn virtual void SelectItem::updateMirrorScene() const = 0

    Pure virtual function which requests an update of the QGraphicsScene which
    owns the mirror.
*/
/*!
    \fn QSharedPointer<Label> SelectItem::getLabel() const

    Returns a Label*, pointing to the Label that owns the SelectItem.

    \sa setLabel()
*/
/*!
    \fn void SelectItem::flipH()

    Convenience function which flips the horizontal component of the
    activeVertex (e.g. if the horizontal component of the activeVertex is
    SelectItem::TOP it becomes SelectItem::BOTTOM and vice versa).

    \sa setActiveVertex(), flipV()
*/
/*!
    \fn void SelectItem::flipV()

    Convenience function which flips the vertical component of the activeVertex
    (e.g. if the vertical component of the activeVertex is SelectItem::LEFT it
    becomes SelectItem::RIGHT and vice versa).

    \sa setActiveVertex(), flipH()
*/
/*!
    \fn QPointF SelectItem::getActivePoint() const

    Returns a QPointF which contains the QGraphicsScene coordinates of the
    currently active vertex.
*/
/*!
    \fn int SelectItem::getActiveVertex() const

    Returns an \c int indicating the currently active vertex. Returns
    SelectItem::UNSELECTED if there is no currently active vertex.

    \sa setActiveVertex(), getActivePoint()
*/
/*!
    \fn bool SelectItem::isItemActive() const

    Returns a bool indicating whether the SelectItem is active (\c true) or not
    (\c false).

    \sa setItemActive(), setInactive(), resetState()
*/
/*!
    \fn bool SelectItem::isItemAdded() const

    Returns a bool indicating whether the SelectItem has been added (\c true) to
    a QGraphicsScene or not (\c false).

    \sa itemWasAdded()
*/
/*!
    \fn void SelectItem::itemWasAdded()

    Sets the hasBeenAdded attribute to be true for the SelectItem and its mirror.

    \sa isItemAdded()
*/
/*!
    \fn void SelectItem::resetState()

    Resets the state of the SelectItem, specifically sets the moved, resized,
    pointAdded, and rotated attributes to \c false (along with the mirror
    counterparts), and sets the activeVertex to SelectItem::UNSELECTED.

    \sa wasMoved(), wasPointAdded(), wasResized(), wasRotated(), setActiveVertex(), getActiveVertex(), setInactive(), setItemActive()
*/
/*!
    \fn void SelectItem::setActiveVertex(int h, int v = NONE)

    Sets the activeVertex (the one currently being manipulated) to the given
    value. For rectangular based objects (RectangleSelect and EllipseSelect)
    \a h and \a v can be used to set the horizontal and vertical components
    seperately or they can be logically combined beforehand:

    \code
    setActiveVertex(SelectItem::TOP, SelectItem::Bottom);

    setActiveVertex(SelectItem::TOP | SelectItem::Bottom);
    \endcode

    For other subclasses of SelectItem, just the vertex index is needed:

    \code
    setActiveVertex(5);
    \endcode

    \code
    setActiveVertex(SelectItem::UNSELECTED);
    \endcode
    indicates that there is no currently active vertex.

    \sa getActiveVertex(), resetState()
*/

/*!
  \fn virtual void SelectItem::setInitial(QRectF rect, int actVertex) = 0;

  Pure virtual function that has different roles depending on the inherited type
  For box based selectors, it is used to set the internal rectangle of the BoxBasedSelector to
  \a rect without checking to see if it completely fits in the QGraphicsScene.
  This is used when setting the internal rectangle of the mirror after checking
  has been done in this instance.
  */
/*!
  \fn QRectF SelectItem::getRect() const

  Returns a QRectF which denotes the bounding rectanlge of the BoxBasedSelector.
  */

/*!
    \fn void SelectItem::setInactive()

    Sets the SelectItem to be inactive (i.e. not the one currently being
    manipulated in the scene).

    \sa resetState(), setItemActive()
*/
/*!
    \fn void SelectItem::setItemActive()

    Sets the SelectItem to be active (i.e. the one currently being manipulated
    in the scene).

    /sa resetState(), setInactive()
*/
/*!
    \fn bool SelectItem::wasMoved() const

    Returns a bool indicating whether the SelectItem was moved (\c true) or not
    (\c false).

    \sa resetState(), wasPointAdded(), wasResized(), wasRotated()
*/
/*!
    \fn bool SelectItem::wasPointAdded() const

    Returns a bool indicating whether a point was added (\c true) or not
    (\c false) to the SelectItem.

    \sa resetState(), wasMoved(), wasResized(), wasRotated()
*/
/*!
    \fn bool SelectItem::wasResized() const

    Returns a bool indicating whether the SelectItem was resized (\c true) or
    not (\c false).

    \sa resetState(), wasMoved(), wasPointAdded(), wasRotated()
*/


/*!
    \fn bool SelectItem::wasRotated() const

    Returns a bool indicating whether the SelectItem was rotates (\c true) or
    not (\c false).

    \sa resetState(), wasMoved(), wasPointAdded(), wasResized()
*/

/*!
  \fn void SelectItem::toPixmap(QPainter* painter)

  Rasterizes the SelectItem and draws it on the given \a painter.
  */

/*!
  \fn bool SelectItem::getFromMaskScene() const

  Returns whether this item was generated by the mask scene

  \sa setFromMaskScene()
  */

/*!
  \fn void SelectItem::setFromMaskScene(bool value)

  Sets the \c fromMaskScene variable to \a value, indicating whether (\c true)
  or not (\c false) the item was generated from the mask scene.

  \sa getOnMaskScene()
  */

/*!
  \fn bool SelectItem::getOnMaskScene() const

  Returns the value of \c onMaskScene.

  \sa setFromMaskScene()
  */

/*!
  \fn void SelectItem::setOnMaskScene(bool value)

  Sets the \c onMaskScene to \a value, indicating whether (\c true) or not
  (\c false) the item is on the mask scene.

  \sa getFromMaskScene()
  */

/*!
  \fn void SelectItem::hideMask()

  Hides any items on the mask scene that were generated on the photo scene.

  \sa showMask()
  */

/*!
  \fn void SelectItem::showMask()

  Shows any items on the mask scene that were generated on the photo scene.

  \sa hideMask()
  */

/*!
  \fn void SelectItem::setMoved(bool val)

  Set the internal moved variable to \a val, indicating whether (\c true) or
  not (\c false) the item was moved.

  \sa wasMoved()
  */

/*!
  \fn void SelectItem::setRotated(bool val)

  Set the internal rotated variable to \a val, indicating whether (\c true) or
  not (\c false) the item was rotated.

  \sa wasRotated()
  */

/*!
  \fn void SelectItem::setOpacity(qreal val)

  Set the visual opacity of the item to \a val.

  */

/*!
  \enum SelectItem::SelectType

  This enum is used to define the type of the SelectItem:

  \value Rectangle
       A RectangleSelect object
  \value Ellipse
       An EllipseSelect object
  \value Polygon
       A PolygonSelect object
  \value Freedraw
       A FreeDraeSelect object
  \value Freeerase
       A FreeEraseSelect object
*/
/*!
  \enum SelectItem::Vertex

  This enum is used to set the active corner of select regions. A vertical and a
  horizontal selection are combined to indicate the specific corner

  \code
  SelectItem::TOP | SelectItem::LEFT
  \endcode

  indicates the top-left corner.

  \value NONE Indicates no side of the object
  \value TOP Indicates the top of the object
  \value BOTTOM Indicates the bottom of the object
  \value LEFT Indicates the left of the object
  \value RIGHT Indicates the right of the object
  \value UNSELECTED Indicates that no vertex has been selected
  */
