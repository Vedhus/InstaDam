#include "freeDrawSelect.h"
#include "label.h"
#include <QPainter>
#include <QBuffer>
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
    mytype = SelectItem::Freedraw;
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    updatePen(myPen);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());
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
                myPainter.drawPoint(QPoint(x, y));
            }
            else{
                img.setPixelColor(x,y, Qt::transparent);
                //cout << "T" << endl;
            }
        }
        //cout << endl;
    }
    myPainter.end();

    //myPixmap = QPixmap::fromImage(img);

    setMirrorMap();
    QGraphicsPixmapItem::prepareGeometryChange();
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);
}

FreeDrawSelect::FreeDrawSelect(QPixmap map, QPen pen)
    : QGraphicsPixmapItem(nullptr), SelectItem(nullptr, nullptr){
    mytype = SelectItem::Freedraw;
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
    myPen = pen;
    myPen.setWidth(1);
    updatePen(myPen);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());
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
                myPainter.drawPoint(QPoint(x, y));
            }
            else{
                img.setPixelColor(x,y, Qt::transparent);
                //cout << "T" << endl;
            }
        }
        //cout << endl;
    }
    myPainter.end();

    //myPixmap = QPixmap::fromImage(img);

    setMirrorMap();
    QGraphicsPixmapItem::prepareGeometryChange();
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);

}

/*!
  Constructs a FreeDrawSelect object with no selected points, and a square brush with a size of 2 pixels.
  */
FreeDrawSelect::FreeDrawSelect() : FreeDrawSelect(QPointF(0.,0.), 2, Qt::SquareCap){
}

/*!
  Constructs a FreeDrawSelect object by reading a QJsonObject and setting the internal pixel map
  to the values given in \a json. \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
FreeDrawSelect::FreeDrawSelect(const QJsonObject &json, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    read(json);
    setActiveVertex(0);
    mytype = SelectItem::Freedraw;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    updatePen(myPen);
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());
    QGraphicsPixmapItem::prepareGeometryChange();

    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);

}

/*!
  Constructor used to combine several FreeDrawSelect items given as \a items, into a single instance. This
  is done by merging all of the internal pixel maps into a single entity, discarding duplicate points.
  */
FreeDrawSelect::FreeDrawSelect(const QList<FreeDrawSelect*> items)
    : QGraphicsPixmapItem(nullptr), SelectItem(0.){
    mytype = SelectItem::Freedraw;
    QListIterator<FreeDrawSelect*> it(items);
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());

    while(it.hasNext()){
        myPainter.drawPixmap(QPointF(0.,0.),(it.next()->getPixmap()));
    }
    setMirrorMap();
    QGraphicsPixmapItem::prepareGeometryChange();
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
FreeDrawSelect::FreeDrawSelect(QPointF point, int brushSize, Qt::PenCapStyle brushMode, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    setActiveVertex(0);
    active = true;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(brushSize);
    myPen.setCapStyle(brushMode);
    myPen.setJoinStyle(Qt::MiterJoin);
    myPen.setMiterLimit(0.1);
    mytype = SelectItem::Freedraw;
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());

    //myPen.setWidth(2);
    //fullWidth = brushSize;
    //halfWidth = brushSize/2;
    //updatePen(myPen);
    //lastPoint = point.toPoint();
    //brushType = brushMode;
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.drawPoint(point);
    myPainter.end();
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);
    QGraphicsPixmapItem::prepareGeometryChange();
}

FreeDrawSelect::FreeDrawSelect(QPointF point, QPen pen, QSharedPointer<Label> label, QGraphicsItem *item)
    : QGraphicsPixmapItem(item), SelectItem(label, item){
    setActiveVertex(0);
    active = true;
    if(label != nullptr)
        label->addItem(this);
    myPen = pen;
    mytype = SelectItem::Freedraw;
    //brushType = myPen.capStyle();
    myPixmap = QPixmap(SelectItem::myBounds);
    myPixmap.fill(Qt::transparent);
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.drawPoint(point);
    myPainter.end();
    myRect = QRectF(0.,0.,SelectItem::myBounds.width(), SelectItem::myBounds.height());
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPixmapItem::setFlag(QGraphicsItem::ItemIsMovable);
    QGraphicsPixmapItem::prepareGeometryChange();
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
    return myRect;
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
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.drawLine(oldPos, newPos);
    myPainter.end();
    setMirrorMap();
}

//QPolygonF FreeDrawSelect::getPoints(QPointF offset){
//    QPolygonF poly = QPolygonF::fromList(myMap->values());
//    return poly.translated(-offset);
//}

/*!
  \reimp
  */
void FreeDrawSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    UNUSED(option);
    UNUSED(widget);
    //if(needToPixmap){
        //QPixmap map(int(myRect.width()),int(myRect.height()));
        //map.fill(Qt::transparent);
        //QPainter temp(&map);
        //QBrush brush(myLabel->getColor());
        //cout << 5 << endl;
        //temp.setPen(myLabel->getColor());
        //cout << 6 << endl;
        //temp.setBrush(brush);
        //temp.drawPoints(getPoints(myRect.topLeft()));
        //temp.end();
        setPixmap(myPixmap);
        //setOffset(myRect.topLeft());
        needToPixmap = false;
    //}
    QGraphicsPixmapItem::paint(painter, option, widget);
}
void FreeDrawSelect::setPointsUnchecked(QPixmap map){
    myPixmap = map;
    setMirrorMap();
}
/*!
  \reimp
  */
void FreeDrawSelect::read(const QJsonObject &json){
    myID = json["objectID"].toInt();
    auto const encoded = json["pixmap"].toString().toLatin1();
    myPixmap.loadFromData(QByteArray::fromBase64(encoded), "PNG");
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
    painter->drawPixmap(QPointF(0.,0.), myPixmap);
}

/*!
  \reimp
  */
void FreeDrawSelect::updatePen(QPen pen){
    myPen = pen;
}

/*!
  \reimp
  */
void FreeDrawSelect::write(QJsonObject &json)const{
    json["objectID"] = myID;
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    myPixmap.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    json["pixmap"] = QLatin1String(encoded);
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
void FreeDrawSelect::addPoints(QSharedPointer<QPixmap> points){
    myPainter.begin(&myPixmap);
    myPainter.setPen(myPen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    myPainter.drawPixmap(QPointF(0.,0.), (*points.data()));
    myPainter.end();
    //myMap->unite((*points));
    //needToPixmap = true;
    //calcRect();
    setMirrorMap();
}

/*/*!
  Removes \a point from the internal pixel map, if it exists in the map. \a delHash is used to store
  any removed pixels in order to enable undo and redo of this operation.
  /
void FreeDrawSelect::deletePoint(int point, QSharedPointer<FreeMap> delHash){
    if(myMap->contains(point)){
        delHash->insert(point, (*myMap)[point]);
        myMap->remove(point);
        needToPixmap = true;
    }
}

*!
  Removes pixels defined by \a points from the internal pixel map. \a delHash is used to store
  any removed pixels in order to enable undo and redo of this operation.
  /
void FreeDrawSelect::deletePoints(QVector<int> &points, QSharedPointer<FreeMap> delHash){
    for(int i = 0; i < points.size(); i++){
        deletePoint(points[i], delHash);
    }
    calcRect();
    setMirrorMap();
}
*/
void FreeDrawSelect::deletePoints(QPointF &start, QPointF &end, QPen pen, QSharedPointer<QPixmap> outmap){
    QPixmap temp = myPixmap.copy();
    myPainter.begin(&myPixmap);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    myPainter.setPen(pen);
    myPainter.drawLine(start, end);
    myPainter.end();
    pen.setColor(myPen.color());
    myPainter.begin(&temp);
    //pen.setColor(Qt::yellow);
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    myPainter.drawPixmap(QPointF(0.,0.), myPixmap);
    myPainter.end();
    myPainter.begin(outmap.data());
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    myPainter.drawPixmap(QPointF(0.,0.), temp);
    myPainter.end();

    setMirrorMap();
}

/*!
  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* FreeDrawSelect::scene(){
    return SelectItem::scene();
}
void FreeDrawSelect::deletePoints(QPen &pen, QSharedPointer<QPixmap> map){
    myPainter.begin(&myPixmap);
    pen.setColor(Qt::red);
    myPainter.setPen(pen);
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    myPainter.drawPixmap(QPointF(0.,0.), (*map.data()));
    myPainter.end();
    setMirrorMap();
}


/*------------------------------- Private ---------------------------*/

/*!
  Sets the bounding rectangle and pixel map of the mirror, based on the values of this instance.
  */
void FreeDrawSelect::setMirrorMap(){
    if(mirror != nullptr){
        mirror->myPixmap = myPixmap;
        //mirror->myRect = myRect;
        //myMap = mirror->myMap;
        //myRect = mirror->myRect;
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
