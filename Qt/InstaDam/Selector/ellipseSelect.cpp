#include "ellipseSelect.h"
#include <iostream>

#include <QPainter>
#include <QGraphicsScene>
//#include "QtWidgets/private/qgraphicsitem_p.h"

/*void EllipseSelect::calcCorners(){
    tl = QRectF(myRect.topLeft(), myRect.topLeft() + SelectItem::xoffset + SelectItem::yoffset);
    bl = QRectF(myRect.bottomLeft() - SelectItem::yoffset, myRect.bottomLeft() + SelectItem::xoffset);
    tr = QRectF(myRect.topRight() - SelectItem::xoffset, myRect.topRight() + SelectItem::yoffset);
    br = QRectF(myRect.bottomRight() - SelectItem::xoffset - SelectItem::yoffset, myRect.bottomRight());

}

void EllipseSelect::clickPoint(QPointF &point){
    active = true;
    if(isInsideRect(tl, point)){
        setActiveVertex(TOP, LEFT);
        activePoint = myRect.topLeft();
    }
    else if(isInsideRect(tr, point)){
        setActiveVertex(TOP, RIGHT);
        activePoint = myRect.bottomRight();
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
    //cout << "  CLIK POINT" << endl;
}



void EllipseSelect::resizeItem(int vertex, QPointF &newPos){
    //std::cout << "RRSZ" << std::endl;
    setActiveVertex(vertex);
    addPoint(newPos);
}
*/

EllipseSelect::EllipseSelect(QPointF point, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, item)
{

    setRect(myRect);
    mytype = Ellipse;
    //myRect = rect;
    setPen(BoxBasedSelector::pen);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

EllipseSelect::EllipseSelect(QPointF point, qreal vertexSize, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, vertexSize, item)
{
    setRect(myRect);
    mytype = Ellipse;
    //myRect = rect;

    setPen(BoxBasedSelector::pen);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

void EllipseSelect::addPoint(QPointF &point, int vertex){
    //myRect.setBottomRight(point);
    //std::cout << "ADD POINT " << point.x() << "," << point.y() << std::endl;

    sortCorners(myRect, point);
    //std::cout << "  " << myRect.topLeft().x() << "," << myRect.topLeft().y() << "  " << myRect.bottomRight().x() << "," << myRect.bottomRight().y() << std::endl;
    calcCorners();

    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
    //printScene();
}

void EllipseSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    //std::cout << "MI " << activeCorner << std::endl;
    if(activeVertex != 0){
        //std::cout << "RESIZE" << std::endl;
        addPoint(newPos);
        resized = true;
    }
    else{
        //std::cout << "MOVE" << std::endl;
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
        moved = true;
    }
    calcCorners();
    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
}


bool EllipseSelect::isInside(QPointF &point){
    //QPointF center = rect().center();
    //if((std::pow(point.x() - center.x(), 2)/std::pow(rect().width()/2., 2)) +
    //        (std::pow(point.y() - center.y(), 2)/std::pow(rect().height()/2., 2)) <= 1.){
    if(QGraphicsEllipseItem::contains(point)){
        return true;
    }
    else if(active && (isInsideRect(tl, point) || isInsideRect(tr, point) || isInsideRect(bl, point) || isInsideRect(br, point))){
        return true;
    }
    return false;
}


QRectF EllipseSelect::boundingRect() const{
    return QGraphicsEllipseItem::boundingRect();
}

void EllipseSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsEllipseItem::paint(painter, option, widget);
    if(active){
        painter->setBrush(brush());
        painter->drawRect(tl);
        painter->drawRect(bl);
        painter->drawRect(tr);
        painter->drawRect(br);
    }

}
