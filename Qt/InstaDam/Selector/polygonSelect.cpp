#include "polygonSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;

PolygonSelect::PolygonSelect(QPointF point, Label *label, QGraphicsItem *item)
    : SelectItem(label, item), QGraphicsPolygonItem(item){
    //cout << "POLY INIT" << endl;
    myPoints.push_back(point);
    setActiveVertex(0);
    activePoint = point;
    //activeVertex = 0;
    polygon << point << point;

    myVertices.push_back(makeVertex(point));
    setPolygon(polygon);
    myRect = QGraphicsPolygonItem::boundingRect();
    mytype = PolygonObj;
    active = true;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    //for(int i = 0; i < myPoints.size(); i++){
    //    std::cout << "POINT " << i << "  " << myPoints[i].x() << "," << myPoints[i].y() << std::endl;
    //}
    //for(int i = 0; i < polygon.size(); i++){
    //    std::cout << "  POINT2 " << i << "  " << polygon[i].x() << "," << polygon[i].y() << std::endl;
    //}

    invertColorForPen();
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsPolygonItem::setFlag(QGraphicsItem::ItemIsMovable);

}

PolygonSelect::~PolygonSelect(){

}

/*---------------------------- Overrides ------------------------------*/
void PolygonSelect::addPoint(QPointF &point, int vertex){
    if(vertex != UNSELECTED){
        //std::cout << "ADD NEW" << std::endl;
        myPoints.insert(vertex, point);
        myVertices.insert(vertex, makeVertex(point));
        setActiveVertex(vertex);
        refresh();
        setMirrorVertex(vertex);
    }
    else if(getActiveVertex() != UNSELECTED){
        //std::cout << "MOVE POINT" << std::endl;
        movePoint(point);
        activePoint = point;
        setMirrorActivePoint(point);
    }
    else{
        active = true;

        pointAdded = false;

        activeVertex = myPoints.size();
        activePoint = point;
        myPoints.push_back(point);

        polygon << polygon[0];
        polygon[activeVertex] = point;

        myVertices.push_back(makeVertex(point));
    }
    QGraphicsPolygonItem::prepareGeometryChange();
    setPolygon(polygon);
    setMirrorPolygon(activeVertex);
    myRect = QGraphicsPolygonItem::boundingRect();

}

QRectF PolygonSelect::boundingRect() const{
    return QGraphicsPolygonItem::boundingRect();
}

void PolygonSelect::clickPoint(QPointF &point){
    //std::cout << "CLICKPOINT" << std::endl;
    active = true;
    selected = true;
    activeVertex = UNSELECTED;

    for(int i = 0; i < myVertices.size(); i++){
        if(isInsideRect(myVertices[i], point)){
            activeVertex = i;
            setMirrorVertex(i);
            activePoint = myPoints[i];
            setMirrorActivePoint(myPoints[i]);
            pointAdded = true;
            break;
        }
    }
}

void PolygonSelect::insertVertex(int vertex, QPointF &point){
    myPoints.insert(vertex + 1, point);
    myVertices.insert(vertex + 1, makeVertex(point));
    refresh();
    setActiveVertex(vertex + 1);
    setMirrorPolygon(vertex + 1);
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

void PolygonSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    //std::cout << "MOVED" << std::endl;
    if(activeVertex == UNSELECTED){
        moved = true;
        setMirrorMoved();
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
        setMirrorPolygon(UNSELECTED);
    }
    else{
        resized = pointAdded;
        setMirrorResized();
        checkPoint(newPos);
        movePoint(newPos);
    }
}

int PolygonSelect::numberOfVertices(){
    return myPoints.size();
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
    setMirrorPolygon(UNSELECTED);
}

void PolygonSelect::resetActiveVertex(){
    setActiveVertex(UNSELECTED);
}

void PolygonSelect::resizeItem(int vertex, QPointF &point){
    //std::cout << "RESIZEITEM" << std::endl;
    setActiveVertex(vertex);
    movePoint(point);
}

void PolygonSelect::updatePen(QPen pen){
    setPen(pen);
}

/*------------------------ Mirror -------------------------------*/
void PolygonSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

void PolygonSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
}

void PolygonSelect::setMirror(SelectItem *item){
    mirror = dynamic_cast<PolygonSelect*>(item);
}

void PolygonSelect::setMirrorActive(){
    if(mirror != nullptr)
        mirror->setItemActive();
}

void PolygonSelect::setMirrorActivePoint(QPointF point){
    if(mirror != nullptr){
        mirror->activePoint = point;
    }
}

void PolygonSelect::setMirrorMoved(){
    if(mirror != nullptr)
        mirror->moved = moved;
}

void PolygonSelect::setMirrorResized(){
    if(mirror != nullptr)
        mirror->resized = resized;
}

void PolygonSelect::setMirrorVertex(int vertex){
    if(mirror != nullptr)
        mirror->setActiveVertex(vertex);
}

void PolygonSelect::updateMirrorScene(){
    if(mirror != nullptr)
        mirror->scene()->update();
}

/*----------------------- End Mirror ----------------------------*/

void PolygonSelect::checkPoint(QPointF &point){
    //std::cout << "CHECKPOINT" << std::endl;
    point.setX(std::min(std::max(0., point.x()), scene()->width()));
    point.setY(std::min(std::max(0., point.y()), scene()->height()));
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
    setMirrorPolygon(activeVertex);
}
QGraphicsScene* PolygonSelect::scene(){
    return SelectItem::scene();
}

/*--------------------------- Private -------------------------------*/
QRectF PolygonSelect::makeVertex(QPointF &point){
    return QRectF(point - (SelectItem::xoffset/2.) - (SelectItem::yoffset/2.),
                  point + (SelectItem::yoffset/2.) + (SelectItem::xoffset/2.));
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

void PolygonSelect::setMirrorPolygon(int actVert){
    if(mirror != nullptr){
        mirror->setPolygon(polygon);
        mirror->setActiveVertex(actVert);
        mirror->myPoints = myPoints;
        mirror->myVertices = myVertices;
        mirror->QGraphicsPolygonItem::prepareGeometryChange();
    }
}

void PolygonSelect::setMirrorRect(QRectF rect){
    if(mirror != nullptr)
        mirror->myRect = rect;
}

