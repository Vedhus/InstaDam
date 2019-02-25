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
    activePoint = point;
    //activeVertex = 0;
    polygon << point << point;
    constant.push_back(0.);
    multiple.push_back(0.);
    myVertices.push_back(makeVertex(point));
    setPolygon(polygon);
    myRect = QGraphicsPolygonItem::boundingRect();
    mytype = Polygon;
    active = true;
    //for(int i = 0; i < myPoints.size(); i++){
    //    std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << std::endl;
    //}
    //for(int i = 0; i < polygon.size(); i++){
    //    std::cout << "  POINT2 " << i << "  " << polygon[i].x() << "," << polygon[i].y() << std::endl;
    //}

    QPen pen(Qt::green);
    pen.setWidth(5);
    setPen(pen);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);

}


void PolygonSelect::removeVertex(int vertex){
    if(vertex == UNSELECTED){
        vertex = myPoints.size() - 2;
        if(vertex < 0){
            vertex = 0;
        }
    }
    myPoints.removeAt(vertex);
    myVertices.removeAt(vertex);
    setActiveVertex(UNSELECTED);
}
//void updateCorner(QPointF point);
void PolygonSelect::addPoint(QPointF &point, int vertex){
    if(vertex != UNSELECTED){
        myPoints.insert(vertex, point);
        myVertices.insert(vertex, makeVertex(point));
        setActiveVertex(vertex);
    }
    else if(getActiveVertex() != UNSELECTED){
        std::cout << "MOVE POINT" << std::endl;
        movePoint(point);
        activePoint = point;
    }
    else{
        active = true;
        std::cout << "NEW POINT " << point.x() << "," << point.y() << std::endl;
    //std::cout << "S1 " << myPoints.size() << std::endl;
        activeVertex = myPoints.size();
        activePoint = point;
        myPoints.push_back(point);
    //std::cout << "S2 " << myPoints.size() << std::endl;

        polygon << polygon[0];
        polygon[activeVertex] = point;
    //std::cout << myPoints.size() << std::endl;
    for(int i = 0; i < myPoints.size(); i++){
        std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << "  " << myPoints.size() << std::endl;
    }
    //for(int i = 0; i < polygon.size(); i++){
    //    std::cout << "  POINT2 " << i << "  " << polygon[i].x() << "," << polygon[i].y() << std::endl;
    //}
    //std::cout << std::endl;
        myVertices.push_back(makeVertex(point));
        constant.push_back(0.);
        multiple.push_back(0.);
    }
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
    activePoint = point;
    myVertices[activeVertex] = makeVertex(point);
    polygon.clear();
    for(int i = 0; i < myPoints.size(); i++){
        polygon << myPoints[i];
    }
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
    selected = true;
    activeVertex = UNSELECTED;

    for(int i = 0; i < myVertices.size(); i++){
        if(isInsideRect(myVertices[i], point)){
            activeVertex = i;
            std::cout << "AV " << i << std::endl;
            break;
        }
    }
}
//void setScene() override;
QRectF PolygonSelect::boundingRect() const{
    return QGraphicsPolygonItem::boundingRect();
}

bool PolygonSelect::isInside(QPointF &point){
    if(QGraphicsPolygonItem::contains(point)){
        return true;
    }
    else{
        for(int i = 0; i < myVertices.size(); i++){
            if(isInsideRect(myVertices[i], point)){
                return true;
            }
        }
    }
    return false;
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
    return QRectF(point - SelectItem::xoffset - SelectItem::yoffset,
                  point + SelectItem::yoffset + SelectItem::xoffset);
}
