#include "polygonSelect.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;

PolygonSelect::PolygonSelect(QPointF point, QGraphicsItem *item)
    : SelectItem(item), QGraphicsPolygonItem(item){
    cout << "POLY INIT" << endl;
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

    myPen = QPen(Qt::green);
    myPen.setWidth(5);
    setPen(myPen);
    /*QColor temp = myPen.color();
    qreal r = temp.redF();
    qreal g = temp.greenF();
    qreal b = temp.blueF();
    r = r > 0.5 ? 0 : 1;
    g = g > 0.5 ? 0 : 1;
    b = b > 0.5 ? 0 : 1;
    temp.setRgbF(r, g, b);
    QPen tpen(temp);
    tpen.setWidth(5);
    highlightPen = tpen;*/
    invertColorForPen();
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);

}

void PolygonSelect::updatePen(QPen pen){
    setPen(pen);
}

int PolygonSelect::numberOfVertices(){
    return myPoints.size();
}
void PolygonSelect::insertVertex(int vertex, QPointF &point){
    myPoints.insert(vertex + 1, point);
    myVertices.insert(vertex + 1, makeVertex(point));
    refresh();
    setActiveVertex(vertex + 1);
}

void PolygonSelect::removeVertex(int vertex){
    //cout << "REMOVING " << vertex << endl;
    if(vertex == UNSELECTED){
        vertex = myPoints.size() - 1;
        //cout << " CHOICE " << vertex << endl;
        if(vertex < 0){
            vertex = 0;
        }
    }
    myPoints.removeAt(vertex);
    myVertices.removeAt(vertex);
    refresh();
    setActiveVertex(UNSELECTED);
}

void PolygonSelect::refresh(){
    //cout << "REFRESH" << endl;
    polygon.clear();
    for(int i = 0; i < myPoints.size(); i++){
        //cout << "  REF " << myPoints[i].x() << "," << myPoints[i].y() << endl;
        polygon << myPoints[i];
    }
    if(myPoints.size() > 1)
        polygon << myPoints[0];
    setPolygon(polygon);
}

//void updateCorner(QPointF point);
void PolygonSelect::addPoint(QPointF &point, int vertex){
    if(vertex != UNSELECTED){
        std::cout << "ADD NEW" << std::endl;
        myPoints.insert(vertex, point);
        myVertices.insert(vertex, makeVertex(point));
        setActiveVertex(vertex);
        refresh();
    }
    else if(getActiveVertex() != UNSELECTED){
        std::cout << "MOVE POINT" << std::endl;
        movePoint(point);
        activePoint = point;
    }
    else{
        active = true;
        //std::cout << "NEW POINT " << point.x() << "," << point.y() << std::endl;
        pointAdded = false;
        //cout << "  LVA1 " << lastPointAdded << endl;
        //std::cout << "S1 " << myPoints.size() << std::endl;
        activeVertex = myPoints.size();
        activePoint = point;
        myPoints.push_back(point);
        //std::cout << "S2 " << myPoints.size() << std::endl;

        polygon << polygon[0];
        polygon[activeVertex] = point;
        std::cout << "SIZE " << myPoints.size() << std::endl;
        //for(int i = 0; i < myPoints.size(); i++){
        //    std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << "  " << myPoints.size() << std::endl;
        //}
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
    //std::cout << "CHECKPOINT" << std::endl;
    point.setX(std::min(std::max(0., point.x()), scene()->width()));
    point.setY(std::min(std::max(0., point.y()), scene()->height()));
}

void PolygonSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    //std::cout << "MOVED" << std::endl;
    if(activeVertex == UNSELECTED){
        moved = true;
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
        resized = pointAdded;
        checkPoint(newPos);
        movePoint(newPos);
    }
}

void PolygonSelect::resetActiveVertex(){
    setActiveVertex(UNSELECTED);
}
void PolygonSelect::movePoint(QPointF &point){
    //std::cout << "MOVE P " << std::endl;
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
    //std::cout << "RESIZEITEM" << std::endl;
    setActiveVertex(vertex);
    movePoint(point);
}

void PolygonSelect::clickPoint(QPointF &point){
    //std::cout << "CLICKPOINT" << std::endl;
    active = true;
    selected = true;
    activeVertex = UNSELECTED;

    for(int i = 0; i < myVertices.size(); i++){
        if(isInsideRect(myVertices[i], point)){
            activeVertex = i;
            activePoint = myPoints[i];
            pointAdded = true;
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
        for(int i = 0; i < myVertices.size(); i++){
        //for(QVector<QRectF>::iterator it = myVertices.begin() ; it != myVertices.end(); ++it){
            //painter->drawRect(*it);
            if(activeVertex == i || (activeVertex == UNSELECTED && i == myVertices.size()-1)){
                painter->setPen(highlightPen);
                painter->drawRect(myVertices[i]);
                painter->setPen(myPen);
            }
            else{
                painter->drawRect(myVertices[i]);
            }
        }
    }
}

QRectF PolygonSelect::makeVertex(QPointF &point){
    return QRectF(point - (SelectItem::xoffset/2.) - (SelectItem::yoffset/2.),
                  point + (SelectItem::yoffset/2.) + (SelectItem::xoffset/2.));
}
