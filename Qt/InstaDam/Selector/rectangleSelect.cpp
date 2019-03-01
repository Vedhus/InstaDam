#include "rectangleSelect.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>

QString RectangleSelect::baseInstruction = QString("");

RectangleSelect::~RectangleSelect(){

}

//void RectangleSelect::resizeItem(int vertex, QPointF &newPos){
//    setActiveVertex(vertex);
//    addPoint(newPos);
//}
RectangleSelect::RectangleSelect(QPointF point, qreal vertSize, QGraphicsItem *item)
    : BoxBasedSelector(point, vertSize,item), QGraphicsRectItem(item)
{
    setRect(myRect);
    mytype = Rect;
    //QColor color(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));
    setPen(BoxBasedSelector::pen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

RectangleSelect::RectangleSelect(QPointF point, QGraphicsItem *item)
    : BoxBasedSelector(point,item), QGraphicsRectItem(item)
{
    setRect(myRect);
    mytype = Rect;
    setPen(BoxBasedSelector::pen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

QGraphicsScene* RectangleSelect::scene(){
    return SelectItem::scene();
}


//void RectangleSelect::setScene(){
//    QGraphicsRectItem::d_ptr->scene = SelectItem::scene();
//}

void RectangleSelect::addPoint(QPointF &point, int vertex){
    //myRect.setBottomRight(point);
    //cout << "ADD POINT " << point.x() << "," << point.y() << endl;

    sortCorners(myRect, point);
    calcCorners();

    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
}


void RectangleSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(activeVertex != 0){
        resized = true;
        //cout << "RESIZE" << endl;
        addPoint(newPos);
    }
    else{
        moved = true;
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
    }
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
}


bool RectangleSelect::isInside(QPointF &point){
    return QGraphicsRectItem::contains(point);
    //return isInsideRect(myRect, point);
}

QRectF RectangleSelect::boundingRect() const{
    return QGraphicsRectItem::boundingRect();
}

void RectangleSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsRectItem::paint(painter, option, widget);
    if(active){
        painter->setBrush(brush());
        painter->drawRect(tl);
        painter->drawRect(bl);
        painter->drawRect(tr);
        painter->drawRect(br);
    }

}
