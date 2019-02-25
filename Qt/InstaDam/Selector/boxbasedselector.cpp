#include "boxbasedselector.h"
#include <QPainter>

BoxBasedSelector::BoxBasedSelector(QPointF point, QGraphicsItem *item) : SelectItem(item){
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners();
    active = true;
    pen = QPen(Qt::blue);
    pen.setWidth(5);
}

BoxBasedSelector::BoxBasedSelector(QPointF point, qreal vertSize, QGraphicsItem *item ) :
    SelectItem(vertSize, item){
    myRect.setTopLeft(point);
    myRect.setBottomRight(point);
    calcCorners();
    active = true;
    pen = QPen(Qt::blue);
    pen.setWidth(5);
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
}
void BoxBasedSelector::resizeItem(int vertex, QPointF &newPos){
    setActiveVertex(vertex);
    addPoint(newPos);
}
void BoxBasedSelector::calcCorners(){
    tl = QRectF(myRect.topLeft(), myRect.topLeft() + SelectItem::xoffset + SelectItem::yoffset);
    bl = QRectF(myRect.bottomLeft() - SelectItem::yoffset, myRect.bottomLeft() + SelectItem::xoffset);
    tr = QRectF(myRect.topRight() - SelectItem::xoffset, myRect.topRight() + SelectItem::yoffset);
    br = QRectF(myRect.bottomRight() - SelectItem::xoffset - SelectItem::yoffset, myRect.bottomRight());
}