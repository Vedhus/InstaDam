#include "freeDrawSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;

QString FreeDrawSelect::baseInstruction = QString("");
FreeDrawSelect::FreeDrawSelect(QPointF point, int brushSize, Label *label, QGraphicsItem *item)
    : QAbstractGraphicsShapeItem(item), SelectItem(label, item){
    myMap = new FreeMap();
    myMap->insert(pointToInt(point.toPoint()), point.toPoint());
    setActiveVertex(0);
    myRect = QRectF(point, point).adjusted(-2.5,-2.5,2.5,2.5);
    mytype = Freedraw;
    active = true;
    if(label != nullptr)
        label->addItem(this);
    myPen.setWidth(1);
    fullWidth = brushSize;
    halfWidth = brushSize/2;
    updatePen(myPen);
    lastPoint = point.toPoint();

    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsMovable);

}

FreeDrawSelect::~FreeDrawSelect(){
    if(myMap != nullptr)
        delete myMap;
}
void FreeDrawSelect::updatePen(QPen pen){
    setPen(pen);
}

void FreeDrawSelect::init(QPointF &point){
    UNUSED(point);
}

void FreeDrawSelect::rasterizeLine(QPoint &start, QPoint &end){
    //cout << "RR" << endl;

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

void FreeDrawSelect::addPoint(QPointF &point, int vertex ){
    UNUSED(point);
    UNUSED(vertex);
    QAbstractGraphicsShapeItem::prepareGeometryChange();

    myRect = this->boundingRect();
}

QRectF FreeDrawSelect::boundingRect() const{
    return myRect.adjusted(-5, -5, 5, 5);
}

QGraphicsScene* FreeDrawSelect::scene(){
    return SelectItem::scene();
}

void FreeDrawSelect::checkPoint(QPointF &point){
    point.setX(std::min(std::max(0., point.x()), qreal(SelectItem::myBounds.width())));
    point.setY(std::min(std::max(0., point.y()), qreal(SelectItem::myBounds.height())));
}

void FreeDrawSelect::checkPoint(QPoint &point){
    point.setX(std::min(std::max(0, point.x()), SelectItem::myBounds.width()));
    point.setY(std::min(std::max(0, point.y()), SelectItem::myBounds.height()));
}

void FreeDrawSelect::moveItem(QPointF &oldPos, QPointF &newPos){

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


void FreeDrawSelect::resizeItem(int vertex, QPointF &point){
    UNUSED(vertex);
    UNUSED(point);
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

void FreeDrawSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    UNUSED(option);
    UNUSED(widget);
    painter->setPen(myPen);
    painter->setBrush(brush());
    FreeMapIterator it((*myMap));
    painter->drawPoints(QPolygon::fromList(myMap->values()));
}

void FreeDrawSelect::deletePoints(QVector<int> &points, FreeMap *delHash){
    for(int i = 0; i < points.size(); i++){
        deletePoint(points[i], delHash);
    }
}

void FreeDrawSelect::deletePoint(int point, FreeMap *delHash){
    if(myMap->contains(point)){
        delHash->insert(point, (*myMap)[point]);
        myMap->remove(point);
    }
}

void FreeDrawSelect::addPoints(FreeMap *points){
    myMap->unite((*points));
}
