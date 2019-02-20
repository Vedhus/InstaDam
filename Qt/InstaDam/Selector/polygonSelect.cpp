#include "polygonSelect.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>

PolygonSelect::PolygonSelect(QPointF point, QGraphicsItem *item)
    : SelectItem(item), QGraphicsPolygonItem(item){
    std::cout << "INIT" << std::endl;
    myPoints.push_back(point);
    setActiveVertex(0);
    activeVertex = 0;
    polygon << point << point;
    constant.push_back(0.);
    multiple.push_back(0.);
    myVertices.push_back(makeVertex(point));
    preCalc();
    setPolygon(polygon);
    myRect = QGraphicsPolygonItem::boundingRect();
    mytype = Polygon;
    active = true;
    for(int i = 0; i < myPoints.size(); i++){
        std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << std::endl;
    }
    for(int i = 0; i < polygon.size(); i++){
        std::cout << "  POINT2 " << i << "  " << polygon[i].x() << "," << polygon[i].y() << std::endl;
    }

    QPen pen(Qt::green);
    pen.setWidth(5);
    setPen(pen);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);

}
//void updateCorner(QPointF point);
void PolygonSelect::addPoint(QPointF &point){
    active = true;
    std::cout << "NEW POINT " << point.x() << "," << point.y() << std::endl;
    //std::cout << "S1 " << myPoints.size() << std::endl;
    activeVertex = myPoints.size();
    myPoints.push_back(point);
    //std::cout << "S2 " << myPoints.size() << std::endl;

    polygon << polygon[0];
    polygon[activeVertex] = point;
    std::cout << myPoints.size() << std::endl;
    for(int i = 0; i < myPoints.size(); i++){
        std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << std::endl;
    }
    for(int i = 0; i < polygon.size(); i++){
        std::cout << "  POINT2 " << i << "  " << polygon[i].x() << "," << polygon[i].y() << std::endl;
    }
    std::cout << std::endl;
    myVertices.push_back(makeVertex(point));
    constant.push_back(0.);
    multiple.push_back(0.);
    preCalc();
    QGraphicsPolygonItem::prepareGeometryChange();
    setPolygon(polygon);
    myRect = QGraphicsPolygonItem::boundingRect();
}

QGraphicsScene* PolygonSelect::scene(){
    return SelectItem::scene();
}

void PolygonSelect::checkPoint(QPointF &point){
    std::cout << "CHECKPOINT" << std::endl;
    point.setX(std::min(std::max(0., point.x()), scene()->width()));
    point.setY(std::min(std::max(0., point.y()), scene()->height()));
}

void PolygonSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    std::cout << "MOVED" << std::endl;
    if(activeVertex == UNSELECTED){
        QPointF shift = newPos - oldPos;
        QPointF tlcShift = boundingRect().topLeft() + shift;
        QPointF brcShift = boundingRect().bottomRight() + shift;
        qreal initialMag = magnitude(shift);
        checkPoint(tlcShift);
        checkPoint(brcShift);
        tlcShift -= boundingRect().topLeft();
        brcShift -= boundingRect().bottomRight();
        qreal tlcM = magnitude(tlcShift);
        qreal brcM = magnitude(brcShift);
        if(initialMag > tlcM || initialMag > brcM){
            if(tlcM > brcM){
                shift = brcShift;
            }
            else{
                shift = tlcShift;
            }
        }
        polygon.clear();
        for(int i = 0; i < myPoints.size(); i++){
            myPoints[i] += shift;
            myVertices[i] = makeVertex(myPoints[i]);
            polygon << myPoints[i];
        }
        preCalc();
        QGraphicsPolygonItem::prepareGeometryChange();
        setPolygon(polygon);
    }
    else{
        checkPoint(newPos);
        movePoint(newPos);
    }
}

void PolygonSelect::movePoint(QPointF &point){
    std::cout << "MOVE P " << std::endl;
    myPoints[activeVertex] = point;
    myVertices[activeVertex] = makeVertex(point);
    polygon.clear();
    for(int i = 0; i < myPoints.size(); i++){
        polygon << myPoints[i];
    }
    preCalc();
    QGraphicsPolygonItem::prepareGeometryChange();
    setPolygon(polygon);
}

void PolygonSelect::resizeItem(int vertex, QPointF &point){
    std::cout << "RESIZEITEM" << std::endl;
    setActiveVertex(vertex);
    movePoint(point);
}

void PolygonSelect::clickPoint(QPointF &point){
    std::cout << "CLICKPOINT" << std::endl;
    active = true;
    for(int i = 0; i < myVertices.size(); i++){
        if(isInsideRect(myVertices[i], point)){
            activeVertex = i;
            break;
        }
    }
    activeVertex = UNSELECTED;
}
//void setScene() override;
QRectF PolygonSelect::boundingRect() const{
    return QGraphicsPolygonItem::boundingRect();
}

void PolygonSelect::preCalc(){
    std::cout << "PRECALC " << myPoints.size() << std::endl;
    int   i, j=myPoints.size()-1 ;

    for(i=0; i<myPoints.size(); i++) {
        if(myPoints[j].y() == myPoints[i].y()) {
            constant[i] = myPoints[i].x();
            multiple[i]=0; }
        else {
            constant[i] = myPoints[i].x() - (myPoints[i].y() * myPoints[j].x())/
                    (myPoints[j].y() - myPoints[i].y()) + (myPoints[i].y() * myPoints[i].x())/
                    (myPoints[j].y() - myPoints[i].y());
            multiple[i] = (myPoints[j].x() - myPoints[i].x())/
                    (myPoints[j].y()-myPoints[i].y());
        }
        j=i;
    }
    std::cout << "PRECALC " << myPoints.size() << std::endl;

}

bool PolygonSelect::isInside(QPointF &point){
    std::cout << "INSIDE " << myPoints.size() << std::endl;



    //  Globals which should be set before calling these functions:
    //
    //  int    polyCorners  =  how many corners the polygon has (no repeats)
    //  float  polyX[]      =  horizontal coordinates of corners
    //  float  polyY[]      =  vertical coordinates of corners
    //  float  x, y         =  point to be tested
    //
    //  The following global arrays should be allocated before calling these functions:
    //
    //  float  constant[] = storage for precalculated constants (same size as polyX)
    //  float  multiple[] = storage for precalculated multipliers (same size as polyX)
    //
    //  (Globals are used in this example for purposes of speed.  Change as
    //  desired.)
    //
    //  USAGE:
    //  Call precalc_values() to initialize the constant[] and multiple[] arrays,
    //  then call pointInPolygon(x, y) to determine if the point is in the polygon.
    //
    //  The function will return YES if the point x,y is inside the polygon, or
    //  NO if it is not.  If the point is exactly on the edge of the polygon,
    //  then the function may return YES or NO.
    //
    //  Note that division by zero is avoided because the division is protected
    //  by the "if" clause which surrounds it.

    //void precalc_values() {


    //unsigned long   i, j=myPoints.size();
    //bool  oddNodes=false;

    //for (i=0; i<polyCorners; i++) {
    //    if ((polyY[i]< y && polyY[j]>=y
    //         ||   polyY[j]< y && polyY[i]>=y)) {
    //        oddNodes^=(y*multiple[i]+constant[i]<x); }
    //    j=i; }

    //  return oddNodes; }


    bool oddNodes=false;
    bool current = myPoints[myPoints.size()-1].y() > point.y();
    bool previous;
    for (int i = 0; i < myPoints.size(); i++) {
        previous = current;
        current = myPoints[i].y() > point.y();
        if (current != previous) oddNodes^=point.y()*multiple[i]+constant[i]<point.x();
    }

    return oddNodes;
}
void PolygonSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsPolygonItem::paint(painter, option, widget);
    if(active){
        painter->setBrush(brush());
        for(QVector<QRectF>::iterator it = myVertices.begin() ; it != myVertices.end(); ++it){
            painter->drawRect(*it);
        }
    }
}

QRectF PolygonSelect::makeVertex(QPointF &point){
    return QRectF(point + SelectItem::xoffset + SelectItem::yoffset,
                  point - SelectItem::yoffset - SelectItem::xoffset);
}
