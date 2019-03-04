#include "ellipseSelect.h"
#include <iostream>
#include <cmath>
#include <QPainter>
#include <QGraphicsScene>
//#include "QtWidgets/private/qgraphicsitem_p.h"

QString EllipseSelect::baseInstruction = QString("");

EllipseSelect::EllipseSelect(QPointF point, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, item)
{

    setRect(myRect);
    mytype = Ellipse;
    //myRect = rect;
    myPen = BoxBasedSelector::pen;
    setPen(myPen);
    invertColorForPen();
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

    sortCorners(myRect, point);
    calcCorners();

    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
    //printScene();
}

void EllipseSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(activeVertex != 0){
        addPoint(newPos);
        resized = true;
    }
    else{
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
        moved = true;
    }
    calcCorners();
    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
}

void EllipseSelect::updatePen(QPen pen){
    setPen(pen);
}


bool EllipseSelect::isInside(QPointF &point){
    QPointF center = rect().center();
    if((std::pow(point.x() - center.x(), 2)/std::pow(rect().width()/2., 2)) +
            (std::pow(point.y() - center.y(), 2)/std::pow(rect().height()/2., 2)) <= 1.){
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
