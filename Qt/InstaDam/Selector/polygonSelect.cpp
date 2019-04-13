#include <QPainter>
#include <QGraphicsScene>
#include <QJsonArray>
#include <algorithm>

#include "polygonSelect.h"
#include "label.h"

/*!
  \class PolygonSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QGraphicsPolygonItem
  \brief The PolygonSelect class provides a class for annotating irregular
         regions.

  Provides a class for annotating irregular regions in InstaDam. The region is
  described by a set of vertices.
  */

/*!
  Constructs a PolygonSelect object with the first vertex at 0,0 in scene
  coordinates
  */
PolygonSelect::PolygonSelect() : PolygonSelect(QPointF(0., 0.)) {
}

/*!
  Constructs a PolygonSelect object by reading a QJsonObject and setting the
  internal vertex list to the values given in \a json. \a label is the Label
  which owns this object and \a item is the parent QGraphicsItem, if any.
  */
PolygonSelect::PolygonSelect(const QJsonObject &json,
                             QSharedPointer<Label> label, QGraphicsItem *item)
    : SelectItem(label, item), QGraphicsPolygonItem(item) {
    read(json);
    myRect = QGraphicsPolygonItem::boundingRect();
    mytype = SelectItem::Polygon;
    active = true;
    if (label)
        label->addItem(this);
    updatePen(myPen);
    QGraphicsPolygonItem::prepareGeometryChange();

    invertColorForPen();
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs a PolygonSelect object by setting the first vertex to \a point,
  \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
PolygonSelect::PolygonSelect(QPointF point, QSharedPointer<Label> label,
                             QGraphicsItem *item)
    : SelectItem(label, item), QGraphicsPolygonItem(item) {
    myPoints.push_back(point);
    setActiveVertex(0);
    activePoint = point;
    polygon << point << point;

    myVertices.push_back(makeVertex(point));
    setPolygon(polygon);
    myRect = QGraphicsPolygonItem::boundingRect();
    mytype = SelectItem::Polygon;
    active = true;
    if (label)
        label->addItem(this);
    updatePen(myPen);

    invertColorForPen();
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
 * Sets the opacity of the label to \a val
*/
void PolygonSelect::setOpacity(qreal val) {
    SelectItem::setOpacity(val);
}

/*!
  Destructor
  */
PolygonSelect::~PolygonSelect() {
}

/*---------------------------- Overrides ------------------------------*/
/*!
  \reimp
  */
void PolygonSelect::addPoint(QPointF &point, const int vertex) {
    if (vertex != UNSELECTED) {
        myPoints.insert(vertex, point);
        myVertices.insert(vertex, makeVertex(point));
        setActiveVertex(vertex);
        activePoint = point;
        refresh();
        pointAdded = true;
        setMirrorVertex(vertex);
    } else if (getActiveVertex() != UNSELECTED) {
        movePoint(point);
        activePoint = point;
        setMirrorActivePoint(point);
    } else {
        active = true;
        pointAdded = false;
        activeVertex = myPoints.size();
        activePoint = point;
        myPoints.push_back(point);
        polygon << polygon[0];
        polygon[activeVertex] = point;
        pointAdded = true;

        myVertices.push_back(makeVertex(point));
    }
    QGraphicsPolygonItem::prepareGeometryChange();
    setPolygon(polygon);
    setMirrorPolygon(activeVertex);
    myRect = QGraphicsPolygonItem::boundingRect();
}

/*!
  \reimp
  */
QRectF PolygonSelect::boundingRect() const {
    return QRectF(0., 0., SelectItem::myBounds.width(),
                  SelectItem::myBounds.height());
}

/*!
  \reimp
  */
void PolygonSelect::clickPoint(const QPointF &point) {
    active = true;
    selected = true;
    activeVertex = UNSELECTED;
    activePoint = QPointF(0.,0.);

    for (int i = 0; i < myVertices.size(); i++) {
        if (isInsideRect(myVertices[i], point)) {
            activeVertex = i;
            setMirrorVertex(i);
            activePoint = myPoints[i];
            setMirrorActivePoint(myPoints[i]);
            break;
        }
    }
}

/*!
  \reimp
  */
void PolygonSelect::insertVertex(const int vertex, const QPointF &point) {
    myPoints.insert(vertex + 1, point);
    myVertices.insert(vertex + 1, makeVertex(point));
    refresh();
    pointAdded = true;
    setActiveVertex(vertex + 1);
    activePoint = point;
    setMirrorPolygon(vertex + 1);
}

/*!
  \reimp
  */
bool PolygonSelect::isInside(const QPointF &point) const {
    if (QGraphicsPolygonItem::contains(point)) {
        return true;
    } else {
        for (int i = 0; i < myVertices.size(); i++) {
            if (isInsideRect(myVertices[i], point)) {
                return true;
            }
        }
    }
    return false;
}

/*!
  \reimp
  */
void PolygonSelect::moveItem(const QPointF &oldPos, QPointF &newPos) {
    if (activeVertex == UNSELECTED) {
        moved = true;
        setMirrorMoved();
        QPointF shift = newPos - oldPos;
        QPointF tlcShift = QGraphicsPolygonItem::boundingRect().topLeft() +
                           shift;
        QPointF brcShift = QGraphicsPolygonItem::boundingRect().bottomRight() +
                           shift;
        qreal initialMag = magnitude(shift);
        checkPoint(tlcShift);
        checkPoint(brcShift);
        tlcShift -= boundingRect().topLeft();
        brcShift -= boundingRect().bottomRight();
        qreal tlcM = magnitude(tlcShift);
        qreal brcM = magnitude(brcShift);
        if (initialMag > tlcM || initialMag > brcM) {
            if (tlcM > brcM) {
                shift = brcShift;
            } else {
                shift = tlcShift;
            }
        }
        polygon.clear();
        for (int i = 0; i < myPoints.size(); i++) {
            myPoints[i] += shift;
            myVertices[i] = makeVertex(myPoints[i]);
            polygon << myPoints[i];
        }
        QGraphicsPolygonItem::prepareGeometryChange();
        setPolygon(polygon);
        setMirrorPolygon(UNSELECTED);
    } else {
        resized = true;
        setMirrorResized();
        QPointF shift = myPoints[activeVertex] + (newPos - oldPos);
        checkPoint(shift);
        movePoint(shift);
    }
}

/*!
  \reimp
  */
int PolygonSelect::numberOfVertices() const {
    return myPoints.size();
}

/*!
  \reimp
  */
void PolygonSelect::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget) {
    QGraphicsPolygonItem::paint(painter, option, widget);
    if (active) {
        painter->setBrush(brush());
        for (int i = 0; i < myVertices.size(); i++) {
            if (activeVertex == i || (activeVertex == UNSELECTED &&
                                      i == myVertices.size()-1)) {
                painter->setPen(highlightPen);
                painter->drawRect(myVertices[i]);
                painter->setPen(myPen);
            } else {
                painter->drawRect(myVertices[i]);
            }
        }
    }
}

/*!
  \reimp
  */
void PolygonSelect::read(const QJsonObject &json) {
    QJsonArray pointArray = json["points"].toArray();
    myPoints.clear();
    myVertices.clear();
    for (int i = 0; i < pointArray.size(); i += 2) {
        QPointF temp = QPointF(pointArray[i].toDouble(),
                               pointArray[i+1].toDouble());
        myPoints.push_back(temp);
        myVertices.push_back(makeVertex(temp));
    }
    refresh();
    myID = json["objectID"].toInt();
}

/*!
  \reimp
  */
void PolygonSelect::removeVertex(int vertex) {
    if (activeVertex != UNSELECTED) {
        vertex = activeVertex;
    } else if (vertex == UNSELECTED) {
        vertex = myPoints.size() - 1;
        if (vertex < 0) {
            vertex = 0;
        }
    }
    myPoints.removeAt(vertex);
    myVertices.removeAt(vertex);
    refresh();
    setActiveVertex(UNSELECTED);
    activePoint = QPointF(0., 0.);
    setMirrorPolygon(UNSELECTED);
}

/*!
  \reimp
  */
void PolygonSelect::resetActiveVertex() {
    setActiveVertex(UNSELECTED);
    activePoint = QPointF(0., 0.);
}

/*!
  \reimp
  */
void PolygonSelect::resizeItem(const int vertex, QPointF &point) {
    setActiveVertex(vertex);
    movePoint(point);
}

/*!
  \reimp
  */
void PolygonSelect::toPixmap(QPainter *painter) {
    painter->drawPolygon(getPolygon());
}

/*!
  \reimp
  */
void PolygonSelect::updatePen(QPen pen) {
    setPen(pen);
    QColor col = pen.color();
    col.setAlphaF(0.25);
    QBrush brush(col);
    setBrush(brush);
}

/*!
  \reimp
  */
void PolygonSelect::write(QJsonObject &json) const {
    json["objectID"] = myID;
    QJsonArray points;
    for (int i = 0; i < myPoints.size(); i++) {
        points.append(myPoints[i].x());
        points.append(myPoints[i].y());
    }
    json["points"] = points;
}

/*------------------------ Mirror -------------------------------*/
/*!
  \reimp
  */
void PolygonSelect::mirrorHide() const {
    if (mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void PolygonSelect::mirrorShow() const {
    if (mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void PolygonSelect::setMirror(SelectItem *item) {
    mirror = dynamic_cast<PolygonSelect*>(item);
}

/*!
  \reimp
  */
void PolygonSelect::setMirrorActive() const {
    if (mirror != nullptr)
        mirror->active = true;
}

/*!
  Sets the PolygonSelect object's active vertex to \a point.
  */
void PolygonSelect::setMirrorActivePoint(QPointF point) const {
    if (mirror != nullptr) {
        mirror->activePoint = point;
    }
}

/*!
  \reimp
  */
void PolygonSelect::setMirrorMoved() const {
    if (mirror != nullptr)
        mirror->moved = moved;
}

/*!
  \reimp
  */
void PolygonSelect::setMirrorResized() const {
    if (mirror != nullptr)
        mirror->resized = resized;
}

/*!
  \reimp
  */
void PolygonSelect::setMirrorVertex(int vertex) const {
    if (mirror != nullptr) {
        mirror->setActiveVertex(vertex);
        mirror->activePoint = activePoint;
    }
}

/*!
  \reimp
  */
void PolygonSelect::updateMirrorScene() const {
    if (mirror != nullptr)
        mirror->scene()->update();
}

/*----------------------- End Mirror ----------------------------*/
/*!
  Checks whether \a point is inside of the QGraphicsScene which owns this
  PolygonSelect object. If it is outside the scene then \a point is adjusted so
  that it is inside.
  */
void PolygonSelect::checkPoint(QPointF &point) {
    point.setX(std::min(std::max(0., point.x()),
                        static_cast<qreal>(SelectItem::myBounds.width())));
    point.setY(std::min(std::max(0., point.y()),
                        static_cast<qreal>(SelectItem::myBounds.height())));
}

/*!
  Moves the currently active vertex to \a point in scene coordinates
  */
void PolygonSelect::movePoint(const QPointF &point) {
    myPoints[activeVertex] = point;
    activePoint = point;
    myVertices[activeVertex] = makeVertex(point);
    polygon.clear();
    for (int i = 0; i < myPoints.size(); i++) {
        polygon << myPoints[i];
    }
    QGraphicsPolygonItem::prepareGeometryChange();
    setPolygon(polygon);
    setMirrorPolygon(activeVertex);
}

/*!
  \overload SelectItem::scene()

  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* PolygonSelect::scene() const {
    return SelectItem::scene();
}

/*--------------------------- Private -------------------------------*/
/*!
  Creates a QRectF for painting a box at the vertex given by \a point.
  */
QRectF PolygonSelect::makeVertex(const QPointF &point) const {
    return QRectF(point - (SelectItem::xoffset/2.) - (SelectItem::yoffset/2.),
                  point + (SelectItem::yoffset/2.) + (SelectItem::xoffset/2.));
}

/*!
  Updates the internal list of vertices.
  */
void PolygonSelect::refresh() {
    polygon.clear();
    for (int i = 0; i < myPoints.size(); i++) {
        polygon << myPoints[i];
    }
    if (myPoints.size() > 1)
        polygon << myPoints[0];
    setPolygon(polygon);
}

/*!
  Updates the mirror object and sets \a actVert as the active vertex.
  */
void PolygonSelect::setMirrorPolygon(int actVert) {
    if (mirror != nullptr) {
        mirror->setPolygon(polygon);
        mirror->setActiveVertex(actVert);
        mirror->activePoint = activePoint;
        mirror->myPoints = myPoints;
        mirror->myVertices = myVertices;
        mirror->QGraphicsPolygonItem::prepareGeometryChange();
    }
}

/*!
  \fn QString PolygonSelect::baseInstructions() const

  \reimp
  */

/*!
  \fn void PolygonSelect::rotate(const QPointF &from, const QPointF &to)
  \reimp
  Empty function as a PolygonSelect cannot be rotated.
  */

/*!
  \fn PolygonSelect* PolygonSelect::getMirror() const
  \reimp
  */

/*!
  \fn void PolygonSelect::rotateMirror() const
  \reimp
  Empty funtion as a PolygonSelect cannot be rotated.
  */

/*!
  \fn void PolygonSelect::setMirrorAdded() const
  \reimp
  */

/*!
  \fn bool PolygonSelect::isVisible() const

  Returns a \c bool indicating whether the PolygonSelect object is visible
  (\c true) in its owning QGraphicsScene or not (\c false).
  */

/*!
  \fn qreal PolygonSelect::magnitude(QPointF point) const

  Returns a \c qreal indicating the absolute magnitude of a vector from 0,0 to
  \a point.
  */

/*!
  \fn QPolygonF PolygonSelect::getPolygon() const
  Returns the PolygonSelect object's internal QPolygonF.
  */
