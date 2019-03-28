#include "freeDrawSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QJsonArray>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <fstream>
#include <chrono>
using namespace std::chrono;
#define PI 3.14159265

/*!
  \typedef FreeMap
  \relates QPointF

  Synonym for QHash<int, QPointF>.
  */

/*!
  \typedef FreeMapIterator
  \relates QPointF
  Synonym for QHashIterator<int, QPointF>.
  */
using namespace std;

void rotatePoint(QPointF &point, const qreal angle){
    qreal x = point.x();
    qreal y = point.y();
    point.setX(x*cos(angle) - y*sin(angle));
    point.setY(y*cos(angle) + x*sin(angle));
}

/*!
  \class FreeDrawSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QAbstractGraphicsShapeItem
  \brief The FreeDrawSelect class provides a class for annotating regions by painting them.

  Provides a class for creating selection regions defined by painting on the canvas with a brush.
  The region is defined by the pixels that are painted on.
  */


QString FreeDrawSelect::baseInstruction = QString("");

/*!
  Constructs a FreeDwarSelect object based on the given \a map, Label \a label
  and parent \a item, if any.
  */
FreeDrawSelect::FreeDrawSelect(QPixmap map, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    myMap = QSharedPointer<FreeMap>::create();
    mytype = SelectItem::Freedraw;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    updatePen(myPen);
    QImage img = map.toImage();
    img = img.convertToFormat(QImage::Format_RGB32);
    QRgb *rgb;
    for(int y = 0; y < img.height(); y++){
        //cout << " Y " << y << endl;
        rgb = (QRgb*)img.scanLine(y);
        for(int x = 0; x < img.width(); x++){
            //cout << " " << qRed(rgb[x]);
            if(qRed(rgb[x]) != 0 || qBlue(rgb[x]) != 0 || qGreen(rgb[x] != 0)){
                //cout << "HIT";
                myMap->insert(coordsToInt(x, y), QPoint(x, y));
            }
        }
        //cout << endl;
    }
    needToPixmap = true;
    calcRect();
    setMirrorMap();
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs a FreeDrawSelect object with no selected points, and a square brush with a size of 2 pixels.
  */
FreeDrawSelect::FreeDrawSelect() : FreeDrawSelect(QPointF(0.,0.), 2, 1){

}

/*!
  Constructs a FreeDrawSelect object by reading a QJsonObject and setting the internal pixel map
  to the values given in \a json. \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
FreeDrawSelect::FreeDrawSelect(const QJsonObject &json, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    myMap = QSharedPointer<FreeMap>::create();
    read(json);
    setActiveVertex(0);
    mytype = SelectItem::Freedraw;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    updatePen(myPen);

    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);

}

/*!
  Constructor used to combine several FreeDrawSelect items given as \a items, into a single instance. This
  is done by merging all of the internal pixel maps into a single entity, discarding duplicate points.
  */
FreeDrawSelect::FreeDrawSelect(const QList<FreeDrawSelect*> items)
    : QGraphicsPixmapItem(nullptr), SelectItem(0.){
    myMap = QSharedPointer<FreeMap>::create();
    QListIterator<FreeDrawSelect*> it(items);
    while(it.hasNext()){
        myMap->unite((*it.next()->myMap));
    }
    calcRect();
    setMirrorMap();
}

/*!
  Constructs a FreeDrawSelect object with an initial selected point \a point, with a brush size of \a brushSize
  pixels, brush type of \a brushMode, \a label as the Label which owns this object and \a item is the
  parent QGraphicsItem, if any. Acceptable values for the \a brushMode are:

  \list
  \li Qt::SquareCap
  \li Qt::RoundCap
  \endlist
  */
FreeDrawSelect::FreeDrawSelect(QPointF point, int brushSize, int brushMode, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    myMap = QSharedPointer<FreeMap>::create();
    myMap->insert(pointToInt(point.toPoint()), point.toPoint());
    setActiveVertex(0);
    myRect = QRectF(point, point).adjusted(-2.5,-2.5,2.5,2.5);
    mytype = SelectItem::Freedraw;
    active = true;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(2);
    fullWidth = brushSize;
    halfWidth = brushSize/2;
    updatePen(myPen);
    lastPoint = point.toPoint();
    brushType = brushMode;

    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);

}

/*!
  Destructor
  */
FreeDrawSelect::~FreeDrawSelect(){
}

/*---------------------------- Overrides ------------------------*/
/*!
  \reimp
  */
void FreeDrawSelect::addPoint(QPointF &point, int vertex ){
    UNUSED(point);
    UNUSED(vertex);
}

/*!
  \reimp
  */
QRectF FreeDrawSelect::boundingRect() const{
    return myRect.adjusted(-2, -2, 2, 2);
}

/*!
  \reimp
  */
void FreeDrawSelect::clickPoint(QPointF &point){
    UNUSED(point);
    active = true;
    activeVertex = UNSELECTED;
}

/*!
  \reimp
  */
bool FreeDrawSelect::isInside(QPointF &point){
    UNUSED(point);
    return false;
}

/*!
  \reimp
  */
void FreeDrawSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    //cout << " MOVE" << endl;
    if(brushType == Qt::SquareCap){
        drawWithSquare(oldPos, newPos);
    }
    else{
        drawWithCircle(oldPos, newPos);
    }
    needToPixmap = true;
    calcRect();
    setMirrorMap();
}

QPolygonF FreeDrawSelect::getPoints(QPointF offset){
    QPolygonF poly = QPolygonF::fromList(myMap->values());
    return poly.translated(-offset);
}

/*!
  \reimp
  */
void FreeDrawSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    UNUSED(option);
    UNUSED(widget);
    if(needToPixmap){
        QPixmap map(int(myRect.width()),int(myRect.height()));
        map.fill(Qt::transparent);
        QPainter temp(&map);
        QBrush brush(myLabel->getColor());
        //cout << 5 << endl;
        temp.setPen(myLabel->getColor());
        //cout << 6 << endl;
        temp.setBrush(brush);
        temp.drawPoints(getPoints(myRect.topLeft()));
        temp.end();
        setPixmap(map);
        setOffset(myRect.topLeft());
        needToPixmap = false;
    }
    QGraphicsPixmapItem::paint(painter, option, widget);
}

/*!
  \reimp
  */
void FreeDrawSelect::read(const QJsonObject &json){
    fullWidth = 2;
    halfWidth = 1;
    brushType = 1;
    QJsonArray pointArray = json["points"].toArray();
    for(int i = 0; i < pointArray.size(); i += 2){
        int x = pointArray[i].toInt();
        int y = pointArray[i+1].toInt();
        myMap->insert(coordsToInt(int(x), int(y)), QPoint(int(x), int(y)));
    }
    myID = json["objectID"].toInt();
    calcRect();
    setMirrorMap();
}

/*!
  \reimp
  */
void FreeDrawSelect::resizeItem(int vertex, QPointF &point){
    UNUSED(vertex);
    UNUSED(point);
}

/*!
  \reimp
  */
void FreeDrawSelect::toPixmap(QPainter *painter){
    painter->drawPoints(getPoints());
}

/*!
  \reimp
  */
void FreeDrawSelect::updatePen(QPen pen){
    UNUSED(pen);
}

/*!
  \reimp
  */
void FreeDrawSelect::write(QJsonObject &json)const{
    json["objectID"] = myID;
    QJsonArray points;
    QList<QPointF> pointList = myMap->values();
    QPointF pnt;
    while(!pointList.isEmpty()){
        pnt = pointList.takeFirst();
        points.append(pnt.x());
        points.append(pnt.y());
    }
    json["points"] = points;
}

/*--------------------------------- Mirror ----------------------------*/
/*!
  \reimp
  */
void FreeDrawSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void FreeDrawSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void FreeDrawSelect::setMirror(SelectItem *item){
    mirror = dynamic_cast<FreeDrawSelect*>(item);
}

/*!
  \reimp
  */
void FreeDrawSelect::updateMirrorScene(){
    //cout << "UMS" << endl;
    if(mirror != nullptr)
        mirror->scene()->update();
}
/*---------------------------- End Mirror ------------------------------*/

/*!
  Adds \a points to the internal pixel map of selected points.
  */
void FreeDrawSelect::addPoints(QSharedPointer<FreeMap> points){
    myMap->unite((*points));
    needToPixmap = true;
    calcRect();
    setMirrorMap();
}

/*!
  Removes \a point from the internal pixel map, if it exists in the map. \a delHash is used to store
  any removed pixels in order to enable undo and redo of this operation.
  */
void FreeDrawSelect::deletePoint(int point, QSharedPointer<FreeMap> delHash){
    if(myMap->contains(point)){
        delHash->insert(point, (*myMap)[point]);
        myMap->remove(point);
        needToPixmap = true;
    }
}

/*!
  Removes pixels defined by \a points from the internal pixel map. \a delHash is used to store
  any removed pixels in order to enable undo and redo of this operation.
  */
void FreeDrawSelect::deletePoints(QVector<int> &points, QSharedPointer<FreeMap> delHash){
    for(int i = 0; i < points.size(); i++){
        deletePoint(points[i], delHash);
    }
    calcRect();
    setMirrorMap();
}

/*!
  Determines which pixels define a brush stroke from \a oldPos to \a newPos, using the fullWidth of the brush,
  and a round brush pattern.

  \sa drawWithSquare()
*/
void FreeDrawSelect::drawWithCircle(QPointF &oldPos, QPointF &newPos){
    QPointF shift = newPos - oldPos;
    qreal angle = std::atan2(shift.y(), shift.x()) - PI/2.;
    //cout << "ANG " << angle*180./PI << endl;
    qreal rotate = (-PI/(fullWidth - 1))/5.;
    //cout << "ROT " << rotate*180./PI << endl;
    QPointF addOnStart = QPointF(halfWidth, 0.);
    QPointF addOnEnd = QPointF(halfWidth, 0.);
    rotatePoint(addOnStart, angle);
    rotatePoint(addOnEnd, angle);
    QPointF start;
    QPointF end;
    //angle -= PI/2.;
    for(int i = 0; i < fullWidth*5; i++){
        rotatePoint(addOnStart, rotate);
        rotatePoint(addOnEnd, -rotate);
        start = (oldPos + addOnStart);
        end = (newPos + addOnEnd);
        rasterizeLine(start, end);
    }
}

/*!
  Determines which pixels define a brush stroke from \a oldPos to \a newPos, using the fullWidth of the brush,
  and a square brush pattern.

  \sa drawWithCircle()
  */
void FreeDrawSelect::drawWithSquare(QPointF &oldPos, QPointF &newPos){
    QPointF start = oldPos;
    QPointF end = newPos;
    int sdx, sdy, edx, edy;
    if(start.toPoint().x() == end.toPoint().x()){
        sdx = edx = 1;
        start.rx() -= halfWidth;
        end.rx() -= halfWidth;
        sdy = edy = 0;
    }
    else if(start.toPoint().y() == end.toPoint().y()){
        sdy = edy = 1;
        start.ry() -= halfWidth;
        end.ry() -= halfWidth;
        sdx = edx = 0;
    }
    else if(start.x() > end.x()){
        sdx = -1;
        edx = 1;
        start.rx() += halfWidth;
        end.rx() -= halfWidth;
        if(start.y() > end.y()){
            sdy = -1;
            edy = 1;
            start.ry() += halfWidth;
            end.ry() -= halfWidth;
        }
        else{
            sdy = 1;
            edy = -1;
            start.ry() -= halfWidth;
            end.ry() += halfWidth;
        }
    }
    else{
        sdx = 1;
        edx = -1;
        start.rx() -= halfWidth;
        end.rx() += halfWidth;
        if(start.y() > end.y()){
            start.ry() += halfWidth;
            end.ry() -= halfWidth;
            sdy = -1;
            edy = 1;
        }
        else{
            sdy = 1;
            edy = -1;
            start.ry() -= halfWidth;
            end.ry() += halfWidth;
        }
    }
    if(sdx == 0){
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.ry() += sdy;
            end.ry() += edy;
        }
    }
    else if(sdy == 0){
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.rx() += sdx;
            end.rx() += edx;
        }
    }
    else{
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.rx() += sdx;
            end.ry() += edy;
        }
        start.rx() -= (fullWidth - 1) * sdx;
        end.ry() -= (fullWidth - 1) *edy;
        start.ry() += sdy;
        end.rx() += edx;
        for(int i = 1; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.ry() += sdy;
            end.rx() += edx;
        }
    }
}

/*!
  \overload SelectItem::scene()

  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* FreeDrawSelect::scene(){
    return SelectItem::scene();
}

/*!
  Sets the internal map to \a map, without doing any bounds checking.
  */
void FreeDrawSelect::setPointsUnchecked(QSharedPointer<FreeMap> map){
    myMap = map;
    needToPixmap = true;
    calcRect();
    setMirrorMap();
}

/*---------------------------- Protected ---------------------------*/
/*!
  Creates a rasterized verion of the line defined by \a start and \a end. The pixels on this line
  are added to the internal pixel map.
  */
void FreeDrawSelect::rasterizeLine(QPointF &start, QPointF &end){
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    qreal step;
    qreal x, y;
    if(abs(dx) >= abs(dy)){
        step = abs(dx);
    }
    else{
        step = abs(dy);
    }
    dx = dx / step;
    dy = dy / step;
    x = start.x();
    y = start.y();
    for(int i = 1.; i <= step; i++){
        if(x >= 0 && y >= 0 && x < SelectItem::myBounds.width() && y < SelectItem::myBounds.height())
        myMap->insert(coordsToInt(int(x), int(y)), QPoint(int(x), int(y)));
        x += dx;
        y += dy;
    }
}

/*------------------------------- Private ---------------------------*/
/*!
  Function to calculate the bounding rectangle which encompases all of the selected points.
*/
void FreeDrawSelect::calcRect(){
    qreal t = 1000000.;
    qreal l = 1000000.;
    qreal b = 0.;
    qreal r = 0.;
    FreeMapIterator it((*myMap));
    while(it.hasNext()){
        it.next();
        t = max(0., min(t, it.value().y()));
        l = max(0., min(l, it.value().x()));
        b = min(qreal(SelectItem::myBounds.height()), max(b, it.value().y()));
        r = min(qreal(SelectItem::myBounds.width()), max(r, it.value().x()));
    }
    myRect = QRectF(QPointF(l,t), QPointF(r,b));
}

/*!
  Sets the bounding rectangle and pixel map of the mirror, based on the values of this instance.
  */
void FreeDrawSelect::setMirrorMap(){
    if(mirror != nullptr){
        mirror->myMap = myMap;
        mirror->myRect = myRect;
        myMap = mirror->myMap;
        myRect = mirror->myRect;
        mirror->setRecalc();
    }
}

/*!
  \fn QString FreeDrawSelect::baseInstructions()

  \reimp
  */

/*!
  \fn void FreeDrawSelect::insertVertex(int vertex, QPointF &point)
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn int FreeDrawSelect::numberOfVertices()
  \reimp

  Returns 0 since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::removeVertex(int vertex=UNSELECTED)
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::resetActiveVertex()
  \reimp

  Empty function since FreeDrawSelect items have no vertices.
  */

/*!
  \fn void FreeDrawSelect::rotate(QPointF &from, QPointF &to)
  \reimp

  Empty function since FreeDrawSelect items cannot be rotated.
  */

/*!
  \fn FreeDrawSelect* FreeDrawSelect::getMirror()
  \reimp
  */

/*!
  \fn void FreeDrawSelect::rotateMirror()
  \reimp

  Empty function since FreeDrawSelect items cannot be rotated.
  */

/*!
  \fn void FreeDrawSelect::setMirrorActive()
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorAdded()
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorMoved()
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorResized()
  \reimp
  */

/*!
  \fn void FreeDrawSelect::setMirrorVertex(int vertex)
  \reimp
  */

/*!
  \fn QPolygonF FreeDrawSelect::getPoints()

  Returns a QPolygonF containing all of the points in the pixel map.
  */

/*!
  \fn bool FreeDrawSelect::isVisible()

  Returns whether the FreeDrawSelect object is visible (\c true) on the QGraphicsScene or not (\c false).
  */

/*!
  \fn int FreeDrawSelect::coordsToInt(int x, int y)

  Converts the \a x and \a y coordinates into an integer representation of the pixel.

  \sa pointToInt()
  */

/*!
  \fn int FreeDrawSelect::pointToInt(QPoint point)

  Converts \a point into an integer representation of the pixel.

  \sa coordsToInt()
  */

/*!
  \fn QSharedPointer<FreeMap> FreeDrawSelect::getMap()

  Returns the internal map of points.
  */
