#include "freeDrawSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QJsonArray>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <fstream>
#define PI 3.14159265

using namespace std;

void rotatePoint(QPointF &point, const qreal angle){
    qreal x = point.x();
    qreal y = point.y();
    point.setX(x*cos(angle) - y*sin(angle));
    point.setY(y*cos(angle) + x*sin(angle));
}

QString FreeDrawSelect::baseInstruction = QString("");

FreeDrawSelect::FreeDrawSelect() : FreeDrawSelect(QPointF(0.,0.), 2, 1){

}

FreeDrawSelect::FreeDrawSelect(const QJsonObject &json, Label *label, QGraphicsItem *item)
    : QAbstractGraphicsShapeItem(item), SelectItem(label, item){
    myMap = new FreeMap();
    read(json);
    setActiveVertex(0);
    mytype = FreedrawObj;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    updatePen(myPen);

    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsMovable);

}

FreeDrawSelect::FreeDrawSelect(const QList<FreeDrawSelect*> items)
    : QAbstractGraphicsShapeItem(nullptr), SelectItem(0.){
    myMap = new FreeMap();
    QListIterator<FreeDrawSelect*> it(items);
    while(it.hasNext()){
        myMap->unite((*it.next()->myMap));
    }
}

FreeDrawSelect::FreeDrawSelect(QPointF point, int brushSize, int brushMode, Label *label, QGraphicsItem *item)
    : QAbstractGraphicsShapeItem(item), SelectItem(label, item){
    myMap = new FreeMap();
    myMap->insert(pointToInt(point.toPoint()), point.toPoint());
    setActiveVertex(0);
    myRect = QRectF(point, point).adjusted(-2.5,-2.5,2.5,2.5);
    mytype = FreedrawObj;
    active = true;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    fullWidth = brushSize;
    halfWidth = brushSize/2;
    updatePen(myPen);
    lastPoint = point.toPoint();
    brushType = brushMode;

    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsMovable);

}

FreeDrawSelect::~FreeDrawSelect(){
    if(myMap != nullptr)
        delete myMap;
}

/*---------------------------- Overrides ------------------------*/
void FreeDrawSelect::addPoint(QPointF &point, int vertex ){
    UNUSED(point);
    UNUSED(vertex);
    QAbstractGraphicsShapeItem::prepareGeometryChange();

    myRect = this->boundingRect();
}

QRectF FreeDrawSelect::boundingRect() const{
    return myRect.adjusted(-5, -5, 5, 5);
}

void FreeDrawSelect::clickPoint(QPointF &point){
    UNUSED(point);
    active = true;
    activeVertex = UNSELECTED;
}

bool FreeDrawSelect::isInside(QPointF &point){
    UNUSED(point);
    return false;
}

void FreeDrawSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(brushType == Qt::SquareCap){
        drawWithSquare(oldPos, newPos);
    }
    else{
        drawWithCircle(oldPos, newPos);
    }
    calcRect();
    setMirrorMap();
    //cout << " _____ " << myRect.left() << "," << myRect.top() << "  " << myRect.right() << "," << myRect.bottom() << endl;
}

void FreeDrawSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    UNUSED(option);
    UNUSED(widget);
    painter->setPen(myPen);
    painter->setBrush(brush());
    //FreeMapIterator it((*myMap));
    painter->drawPoints(QPolygon::fromList(myMap->values()));
}

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
}

void FreeDrawSelect::resizeItem(int vertex, QPointF &point){
    UNUSED(vertex);
    UNUSED(point);
}

void FreeDrawSelect::updatePen(QPen pen){
    setPen(pen);
}

void FreeDrawSelect::write(QJsonObject &json)const{
    json["objectID"] = myID;
    QJsonArray points;
    QList<QPoint> pointList = myMap->values();
    QPoint pnt;
    while(!pointList.isEmpty()){
        pnt = pointList.takeFirst();
        points.append(pnt.x());
        points.append(pnt.y());
    }
    json["points"] = points;
}

/*--------------------------------- Mirror ----------------------------*/
void FreeDrawSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

void FreeDrawSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
}

void FreeDrawSelect::setMirror(SelectItem *item){
    mirror = dynamic_cast<FreeDrawSelect*>(item);
}

void FreeDrawSelect::updateMirrorScene(){
    //cout << "UMS" << endl;
    if(mirror != nullptr)
        mirror->scene()->update();
}
/*---------------------------- End Mirror ------------------------------*/

void FreeDrawSelect::addPoints(FreeMap *points){
    myMap->unite((*points));
    calcRect();
    setMirrorMap();
}

void FreeDrawSelect::checkPoint(QPointF &point){
    point.setX(std::min(std::max(0., point.x()), qreal(SelectItem::myBounds.width())));
    point.setY(std::min(std::max(0., point.y()), qreal(SelectItem::myBounds.height())));
}

void FreeDrawSelect::deletePoint(int point, FreeMap *delHash){
    if(myMap->contains(point)){
        delHash->insert(point, (*myMap)[point]);
        myMap->remove(point);
    }
}

void FreeDrawSelect::deletePoints(QVector<int> &points, FreeMap *delHash){
    for(int i = 0; i < points.size(); i++){
        deletePoint(points[i], delHash);
    }
    calcRect();
    setMirrorMap();
}

void FreeDrawSelect::drawWithCircle(QPointF &oldPos, QPointF &newPos){
    //cout << "CIRCLE " << oldPos.x() << "," << oldPos.y() << "  " << newPos.x() << "," << newPos.y() << endl;
    QPointF shift = newPos - oldPos;
    qreal angle = std::atan2(shift.y(), shift.x()) - PI/2.;
    //cout << "ANG " << angle*180./PI << endl;
    qreal rotate = -PI/(fullWidth - 1);
    //cout << "ROT " << rotate*180./PI << endl;
    QPointF addOnStart = QPointF(halfWidth, 0.);
    QPointF addOnEnd = QPointF(halfWidth, 0.);
    rotatePoint(addOnStart, angle);
    rotatePoint(addOnEnd, angle);
    //addOn.setX(addOn.x()*cos(angle) - addOn.y()*sin(angle));
    //addOn.setY(addOn.y()*cos(angle) + addOn.x()*sin(angle));
    QPoint start;
    QPoint end;
    //angle -= PI/2.;
    for(int i = 0; i < fullWidth; i++){
        rotatePoint(addOnStart, rotate);
        rotatePoint(addOnEnd, -rotate);
        start = (oldPos + addOnStart).toPoint();
        end = (newPos + addOnEnd).toPoint();
        //cout << "   " << i << " " << int(rotate*i*180./PI) << "_" << int(addOnStart.x()) << "," << int(addOnStart.y()) << "  " << int(-rotate*i*180./PI) << "_"<< int(addOnEnd.x()) << "," << int(addOnEnd.y()) << endl;
        rasterizeLine(start, end);
    }
}

void FreeDrawSelect::drawWithSquare(QPointF &oldPos, QPointF &newPos){
    QPoint start = oldPos.toPoint();
    QPoint end = newPos.toPoint();
    int sdx, sdy, edx, edy;
    if(start.x() == end.x()){
        sdx = edx = 1;
        start.rx() -= halfWidth;
        end.rx() -= halfWidth;
        sdy = edy = 0;
    }
    else if(start.y() == end.y()){
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

QGraphicsScene* FreeDrawSelect::scene(){
    return SelectItem::scene();
}

/*---------------------------- Protected ---------------------------*/
void FreeDrawSelect::rasterizeLine(QPoint &start, QPoint &end){
    //cout << "       RR " << start.x() << "," << start.y() << "  " << end.x() << "," << end.y() << endl;

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
        myMap->insert(coordsToInt(int(x), int(y)), QPoint(int(x), int(y)));
        x += dx;
        y += dy;
    }
}

/*------------------------------- Private ---------------------------*/
void FreeDrawSelect::calcRect(){
    return;
    int t = 1000000;
    int l = 1000000;
    int b = 0;
    int r = 0;
    FreeMapIterator it((*myMap));
    while(it.hasNext()){
        it.next();
        t = min(t, it.value().y());
        l = min(l, it.value().x());
        b = max(b, it.value().y());
        r = max(r, it.value().x());
    }
    myRect = QRectF(QPointF(t,l), QPointF(b,r));
}

void FreeDrawSelect::checkPoint(QPoint &point){
    point.setX(std::min(std::max(0, point.x()), SelectItem::myBounds.width()));
    point.setY(std::min(std::max(0, point.y()), SelectItem::myBounds.height()));
}

void FreeDrawSelect::setMirrorMap(){
    if(mirror != nullptr){
        cout << "IUP"<< endl;
        mirror->myMap = myMap;
        mirror->myRect = myRect;
        myMap = mirror->myMap;
        myRect = mirror->myRect;
    }
}
