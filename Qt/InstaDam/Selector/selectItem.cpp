#include <QtWidgets>

#include "selectItem.h"
#include <algorithm>
#include <iostream>
using namespace std;


qreal SelectItem::cornerSize = 10.;
QPointF SelectItem::xoffset = QPointF(SelectItem::cornerSize, 0.);
QPointF SelectItem::yoffset = QPointF(0., SelectItem::cornerSize);

SelectType SelectItem::getType(){
    return selectType;
}

void SelectItem::setCornerSize(qreal size){
    SelectItem::cornerSize = size;
    SelectItem::xoffset = QPointF(SelectItem::cornerSize, 0.);
    SelectItem::yoffset = QPointF(0., SelectItem::cornerSize);
}

SelectItem::SelectItem(qreal corner, QGraphicsItem *item) : QGraphicsItem(item){
    SelectItem::setCornerSize(corner);
}

SelectItem::SelectItem(QGraphicsItem *item) : QGraphicsItem(item){
    SelectItem::setCornerSize(10.);
}

void SelectItem::sortCorners(QRectF &rect, QPointF &newPoint){
    if(activeCorner & TOP){
        if(activeCorner & LEFT){
            rect.setTopLeft(newPoint);
        }
        else{
            rect.setTopRight(newPoint);
        }
    }
    else if(activeCorner & BOTTOM){
        if(activeCorner & RIGHT){
            rect.setBottomRight(newPoint);
        }
        else{
            rect.setBottomLeft(newPoint);
        }
    }
    QPointF tlc = rect.topLeft();
    QPointF brc = rect.bottomRight();
    if(tlc.y() > brc.y()){
        rect.setTop(brc.y());
        rect.setBottom(tlc.y());
        flipH();
    }
    if(tlc.x() > brc.x()){
        rect.setLeft(brc.x());
        rect.setRight(tlc.x());
        flipV();
    }
}

int SelectItem::type() const{
    return mytype;
}

bool SelectItem::isInsideRect(QRectF &rect, QPointF &point){
    if(point.y() >= rect.top() && point.y() <= rect.bottom()){
        if(point.x() <= rect.right() && point.x() >= rect.left()){
            return true;
        }
    }
    return false;
}

QGraphicsScene* SelectItem::scene(){
    //cout << "SCENE2" << endl;
    return QGraphicsItem::scene();
}

QGraphicsItem* SelectItem::getParentItem(){
    cout << "    L  " << parentItem() << "  " << QGraphicsItem::parentItem() << endl;
    return parentItem();
}

void SelectItem::checkBoundaries(QPointF &shift, QRectF &rect){
    QPointF tlc = rect.topLeft() + shift;
    QPointF brc = rect.bottomRight() + shift;
    //cout << "MOVE" << endl;

    //cout << SelectItem::scene()->width() << endl;
    //cout << "SDFSDF" << endl;
    if(tlc.x() < 0 || tlc.y() < 0){
        //cout << "X1" << endl;
        tlc.setY(std::max(tlc.y(), 0.));
        tlc.setX(std::max(tlc.x(), 0.));
        rect.moveTopLeft(tlc);
    }
    else if(brc.x() >= scene()->width() || brc.y() >= scene()->height()){
        //cout << "X2" << endl;
        brc.setY(std::min(brc.y(), scene()->height() - 1));
        brc.setX(std::min(brc.x(), scene()->width() - 1));
        rect.moveBottomRight(brc);
    }
    else{
        //cout << "X3" << endl;
        rect.moveTopLeft(tlc);
    }
}
