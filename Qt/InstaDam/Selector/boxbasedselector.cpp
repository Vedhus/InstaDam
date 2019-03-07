#include "boxbasedselector.h"
#include <QPainter>
#include <iostream>
#include <math.h>

#define PI 3.14159265

using namespace std;

BoxBasedSelector::BoxBasedSelector(QPointF point, Label *label, QGraphicsItem *item) : SelectItem(label, item){
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners(false);
    active = true;
    //pen = QPen(Qt::blue);
    //pen.setWidth(5);
}

BoxBasedSelector::BoxBasedSelector(QPointF point, qreal vertSize, Label *label, QGraphicsItem *item ) :
    SelectItem(vertSize, label, item){
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners();
    active = true;
    //pen = QPen(Qt::blue);
    //pen.setWidth(5);
}

BoxBasedSelector::~BoxBasedSelector(){

}
void BoxBasedSelector::resetActiveVertex(){
    setActiveVertex(0);
}

void BoxBasedSelector::removeVertex(int vertex){
    UNUSED(vertex);
}

void BoxBasedSelector::clickPoint(QPointF &point){
    active = true;
    if(isInsideRect(tl, point)){
        setActiveVertex(TOP, LEFT);
        activePoint = myRect.topLeft();
    }
    else if(isInsideRect(tr, point)){
        setActiveVertex(TOP, RIGHT);
        activePoint = myRect.topRight();
    }
    else if(isInsideRect(bl, point)){
        setActiveVertex(BOTTOM, LEFT);
        activePoint = myRect.bottomLeft();
    }
    else if(isInsideRect(br, point)){
        setActiveVertex(BOTTOM, RIGHT);
        activePoint = myRect.bottomRight();
    }
    else{
        setActiveVertex(0, 0);
    }
    setMirrorVertex(getActiveVertex());
}
void BoxBasedSelector::resizeItem(int vertex, QPointF &newPos){
    setActiveVertex(vertex);
    setMirrorVertex(getActiveVertex());
    addPoint(newPos);

}
void BoxBasedSelector::calcCorners(bool mir){
    tl = QRectF(myRect.topLeft(), myRect.topLeft() + SelectItem::xoffset + SelectItem::yoffset);
    bl = QRectF(myRect.bottomLeft() - SelectItem::yoffset, myRect.bottomLeft() + SelectItem::xoffset);
    tr = QRectF(myRect.topRight() - SelectItem::xoffset, myRect.topRight() + SelectItem::yoffset);
    br = QRectF(myRect.bottomRight() - SelectItem::xoffset - SelectItem::yoffset, myRect.bottomRight());
    if(mir)
        setMirrorCorners(tl, bl, tr, br);
}

void BoxBasedSelector::rotate(QPointF &from, QPointF &to){
    setTransformOriginPoint(myRect.center());
    QPointF start = from - myRect.center();
    QPointF end = to - myRect.center();
    myRotation += (std::atan2(end.y(), end.x()) - std::atan2(start.y(), start.x())) * 180./PI;
    setRotation(myRotation);
    rotated = true;
    rotateMirror();
}
