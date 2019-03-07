#include "ellipseSelect.h"
#include "label.h"
#include <iostream>
#include <cmath>
#include <QPainter>
#include <QGraphicsScene>
//#include "QtWidgets/private/qgraphicsitem_p.h"

QString EllipseSelect::baseInstruction = QString("");

EllipseSelect::EllipseSelect(QPointF point, Label *label, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, label, item)
{
    setRect(myRect);
    mytype = Ellipse;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    invertColorForPen();
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

EllipseSelect::EllipseSelect(QPointF point, qreal vertexSize, Label *label, QGraphicsItem *item)
    : QGraphicsEllipseItem(item), BoxBasedSelector(point, vertexSize, label, item)
{
    setRect(myRect);
    mytype = Ellipse;
    updatePen(myPen);
    if(label)
        label->addItem(this);

    setPen(BoxBasedSelector::pen);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsEllipseItem::setFlag(QGraphicsItem::ItemIsMovable);
}

EllipseSelect::~EllipseSelect(){

}

void EllipseSelect::rotateMirror(){
    if(mirror != nullptr){
        mirror->myRotation = myRotation;
        mirror->BoxBasedSelector::setTransformOriginPoint(myRect.center());
        mirror->BoxBasedSelector::setRotation(myRotation);
    }
}

void EllipseSelect::setRectUnchecked(QRectF rect){
    QGraphicsEllipseItem::prepareGeometryChange();
    myRect = rect;
    setRect(myRect);
}

void EllipseSelect::setMirrorActive(){
    if(mirror != nullptr)
        mirror->setItemActive();
}

void EllipseSelect::setMirrorMoved(){
    if(mirror != nullptr)
        mirror->moved = moved;
}

void EllipseSelect::setMirrorResized(){
    if(mirror != nullptr)
        mirror->resized = resized;
}

QGraphicsScene* EllipseSelect::scene(){
    return SelectItem::scene();
}

void EllipseSelect::addPoint(QPointF &point, int vertex){
    UNUSED(vertex);
    sortCorners(myRect, point);
    calcCorners();

    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
    //printScene();
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

void EllipseSelect::setMirror(SelectItem *item){
    mirror = dynamic_cast<EllipseSelect*>(item);
}

void EllipseSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    if(activeVertex != 0){
        addPoint(newPos);
        resized = true;
        setMirrorResized();
    }
    else{
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
        moved = true;
        setMirrorMoved();
    }
    calcCorners();
    QGraphicsEllipseItem::prepareGeometryChange();
    setRect(myRect);
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

void EllipseSelect::updatePen(QPen pen){
    setPen(pen);
}

void EllipseSelect::updateMirrorScene(){
    if(mirror != nullptr)
        mirror->scene()->update();
}

void EllipseSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

void EllipseSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
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
void EllipseSelect::setMirrorVertex(int vertex){
    if(mirror != nullptr)
        mirror->setActiveVertex(vertex);
}

void EllipseSelect::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc){
    if(mirror != nullptr){
        mirror->tl = tlc;
        mirror->bl = blc;
        mirror->tr = trc;
        mirror->br = brc;
    }
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
