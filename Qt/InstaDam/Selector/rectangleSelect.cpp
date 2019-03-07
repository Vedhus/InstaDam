#include "rectangleSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;
QString RectangleSelect::baseInstruction = QString("");

RectangleSelect::~RectangleSelect(){

}

RectangleSelect::RectangleSelect(QPointF point, qreal vertSize, Label *label, QGraphicsItem *item)
    : BoxBasedSelector(point, vertSize,label, item), QGraphicsRectItem(item)
{
    //cout << "RR" << endl;
    setRect(myRect);
    mytype = Rect;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    invertColorForPen();
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

void RectangleSelect::updatePen(QPen pen){
    setPen(pen);
}

RectangleSelect::RectangleSelect(QPointF point, Label *label, QGraphicsItem *item)
    : BoxBasedSelector(point, label, item), QGraphicsRectItem(item)
{
    setRect(myRect);
    mytype = Rect;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

QGraphicsScene* RectangleSelect::scene(){
    return SelectItem::scene();
}

void RectangleSelect::setMirrorActive(){
    if(mirror != nullptr)
        mirror->setItemActive();
}

void RectangleSelect::setMirrorMoved(){
    if(mirror != nullptr)
        mirror->moved = moved;
}

void RectangleSelect::setMirrorResized(){
    if(mirror != nullptr)
        mirror->resized = resized;
}

void RectangleSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

void RectangleSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
}

void RectangleSelect::updateMirrorScene(){
    if(mirror != nullptr)
        mirror->scene()->update();
}

void RectangleSelect::setRectUnchecked(QRectF rect){
    QGraphicsRectItem::prepareGeometryChange();
    //cout << "S R U" << endl;
    myRect = rect;
    setRect(myRect);
}

void RectangleSelect::addPoint(QPointF &point, int vertex){
    UNUSED(vertex);
    //myRect.setBottomRight(point);
    //cout << "ADD POINT " << point.x() << "," << point.y() << endl;

    sortCorners(myRect, point);
    calcCorners();

    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

void RectangleSelect::setMirrorVertex(int vertex){
    if(mirror != nullptr){
        //cout << "SET A V M" << endl;
        mirror->setActiveVertex(vertex);
    }
}
void RectangleSelect::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc){
    if(mirror != nullptr){
        //cout << " SET M C" << endl;
        mirror->tl = tlc;
        mirror->bl = blc;
        mirror->tr = trc;
        mirror->br = brc;
    }
}
void RectangleSelect::setMirror(SelectItem *item){
    //cout << "MIRROR " << myID << endl;
    mirror = dynamic_cast<RectangleSelect*>(item);
}

void RectangleSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(activeVertex != 0){
        resized = true;
        setMirrorResized();
        //cout << "RESIZE" << endl;
        addPoint(newPos);
    }
    else{
        moved = true;
        setMirrorMoved();
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
    }
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
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

void RectangleSelect::rotateMirror(){
    if(mirror != nullptr){
        mirror->myRotation = myRotation;
        mirror->BoxBasedSelector::setTransformOriginPoint(myRect.center());
        mirror->BoxBasedSelector::setRotation(myRotation);
    }
}
