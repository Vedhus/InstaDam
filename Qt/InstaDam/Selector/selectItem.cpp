#include <QtWidgets>

#include "selectItem.h"
#include "label.h"
#include <algorithm>
#include <iostream>
using namespace std;


qreal SelectItem::vertexSize = 10.;
QPointF SelectItem::xoffset = QPointF(SelectItem::vertexSize, 0.);
QPointF SelectItem::yoffset = QPointF(0., SelectItem::vertexSize);
int SelectItem::ID = 0;
QSize SelectItem::myBounds = QSize(0,0);


SelectItem::SelectItem(qreal vertSize, Label *label, QGraphicsItem *item) : QGraphicsItem(item){
    SelectItem::setVertexSize(vertSize);
    myID = SelectItem::ID;
    SelectItem::ID++;
    setLabel(label);
}

SelectItem::SelectItem(Label *label, QGraphicsItem *item) : SelectItem(10., label, item){
}

QGraphicsItem* SelectItem::getParentItem(){
    return parentItem();
}

void SelectItem::invertColorForPen(){
    QColor color = myPen.color();
    color.setRgbF(color.redF() > 0.5 ? 0 : 1, color.greenF() > 0.5 ? 0 : 1, color.blueF() > 0.5 ? 0 : 1);
    highlightPen = QPen(color);
    highlightPen.setWidth(5);
}

QGraphicsScene* SelectItem::scene(){
    //cout << "SCENE2" << endl;
    return QGraphicsItem::scene();
}

void SelectItem::setLabel(Label *label){
    myLabel = label;
    if(label != nullptr){
        myPen = QPen(label->getColor());
        myPen.setWidth(2);
    }
}

void SelectItem::setVertexSize(qreal size){
    SelectItem::vertexSize = size;
    SelectItem::xoffset = QPointF(SelectItem::vertexSize, 0.);
    SelectItem::yoffset = QPointF(0., SelectItem::vertexSize);
}

void SelectItem::sortCorners(QRectF &rect, QPointF &newPoint){
    if(activeVertex & TOP){
        if(activeVertex & LEFT){
            rect.setTopLeft(newPoint);
        }
        else{
            rect.setTopRight(newPoint);
        }
    }
    else if(activeVertex & BOTTOM){
        if(activeVertex & RIGHT){
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

/*---------------- Protected members ---------------------------*/
void SelectItem::checkBoundaries(QPointF &shift, QRectF &rect){
    QPointF tlc = rect.topLeft() + shift;
    QPointF brc = rect.bottomRight() + shift;

    if(tlc.x() < 0 || tlc.y() < 0){
        tlc.setY(std::max(tlc.y(), 0.));
        tlc.setX(std::max(tlc.x(), 0.));
        rect.moveTopLeft(tlc);
    }
    else if(brc.x() >= SelectItem::myBounds.width() ||
            brc.y() >= SelectItem::myBounds.height()){
        brc.setY(std::min(int(brc.y()), SelectItem::myBounds.height() - 1));
        brc.setX(std::min(int(brc.x()), SelectItem::myBounds.width() - 1));
        rect.moveBottomRight(brc);
    }
    else{
        rect.moveTopLeft(tlc);
    }
}

bool SelectItem::isInsideRect(QRectF &rect, QPointF &point){
    if(point.y() >= rect.top() && point.y() <= rect.bottom()){
        if(point.x() <= rect.right() && point.x() >= rect.left()){
            return true;
        }
    }
    return false;
}
