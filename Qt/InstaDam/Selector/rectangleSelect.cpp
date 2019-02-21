#include "rectangleSelect.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>

void RectangleSelect::calcCorners(){
    tl = QRectF(myRect.topLeft(), myRect.topLeft() + SelectItem::xoffset + SelectItem::yoffset);
    bl = QRectF(myRect.bottomLeft() - SelectItem::yoffset, myRect.bottomLeft() + SelectItem::xoffset);
    tr = QRectF(myRect.topRight() - SelectItem::xoffset, myRect.topRight() + SelectItem::yoffset);
    br = QRectF(myRect.bottomRight() - SelectItem::xoffset - SelectItem::yoffset, myRect.bottomRight());

}

RectangleSelect::~RectangleSelect(){

}

RectangleSelect::RectangleSelect(QPointF point, qreal vertSize, QGraphicsItem *item)
    : SelectItem(vertSize,item), QGraphicsRectItem(item)
{
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners();
    setRect(myRect);
    mytype = Rect;
    //myRect = rect;
    active = true;
    //QColor color(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));
    QPen pen(Qt::blue);
    pen.setWidth(5);
    setPen(pen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

RectangleSelect::RectangleSelect(QPointF point, QGraphicsItem *item)
    : SelectItem(item), QGraphicsRectItem(item)
{
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners();
    setRect(myRect);
    mytype = Rect;
    //myRect = rect;
    active = true;
    //QColor color(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));
    QPen pen(Qt::blue);
    pen.setWidth(5);
    setPen(pen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

QGraphicsScene* RectangleSelect::scene(){
    return SelectItem::scene();
}

void RectangleSelect::resizeItem(int vertex, QPointF &newPos){
    setActiveVertex(vertex);
    addPoint(newPos);
}
//void RectangleSelect::setScene(){
//    QGraphicsRectItem::d_ptr->scene = SelectItem::scene();
//}


void RectangleSelect::addPoint(QPointF &point){
    //myRect.setBottomRight(point);
    //cout << "ADD POINT " << point.x() << "," << point.y() << endl;

    sortCorners(myRect, point);
    calcCorners();

    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
}

void RectangleSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(activeVertex != 0){
        //cout << "RESIZE" << endl;
        addPoint(newPos);
    }
    else{
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
    }
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
}

void RectangleSelect::clickPoint(QPointF &point){
    active = true;
    if(isInsideRect(tl, point)){
        setActiveVertex(TOP, LEFT);
    }
    else if(isInsideRect(tr, point)){
        setActiveVertex(TOP, RIGHT);
    }
    else if(isInsideRect(bl, point)){
        setActiveVertex(BOTTOM, LEFT);
    }
    else if(isInsideRect(br, point)){
        setActiveVertex(BOTTOM, RIGHT);
    }
    else{
        setActiveVertex(0, 0);
    }
    //cout << "  CLIK POINT" << endl;
}

bool RectangleSelect::isInside(QPointF &point){
    return isInsideRect(myRect, point);
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
