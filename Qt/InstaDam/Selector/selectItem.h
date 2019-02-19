/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   diagramitem.h
 * Author: friedel
 *
 * Created on February 6, 2019, 9:00 PM
 */

#ifndef SELECTITEM_H
#define SELECTITEM_H

#include <QGraphicsItem>
#include <QWidget>

enum SelectType:int {Generic=50, Rect=51, Ellipse=52,Polygon=53, Free=54};
//enum Side:int {NONE=0, TOP=1, BOTTOM=2, LEFT=4, RIGHT=8};
const unsigned char TOP = 0x1;
const unsigned char BOTTOM = 0x2;
const unsigned char LEFT = 0x4;
const unsigned char RIGHT = 0x8;
QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
class QGraphicsItemPrivate;
QT_END_NAMESPACE

class SelectItem : public QGraphicsItem
{
    public:
        static qreal cornerSize;
        static QPointF xoffset;
        static QPointF yoffset;

        SelectItem(qreal corner = 10., QGraphicsItem *item = nullptr);
        SelectItem(QGraphicsItem *item = nullptr);
        virtual void addPoint(QPointF &point) = 0;
        virtual void moveItem(QPointF &oldPos, QPointF &newPos) = 0;
        virtual void resizeItem(unsigned char corner, QPointF &shift) = 0;
        virtual void clickPoint(QPointF &point) = 0;
        virtual bool isInside(QPointF &point) = 0;
        //virtual void setScene() = 0;
        void sortCorners(QRectF &rect, QPointF &newPoint);
        SelectType getType();
        int type() const override;
        void setActive(){active = true;}
        void setInactive(){active = false;}
        static void setCornerSize(qreal size);
        QGraphicsScene* scene();
        QGraphicsItem* getParentItem();
        bool wasResized(){return resized;}
        bool wasMoved(){return moved;}
        unsigned char getActiveCorner(){return activeCorner;}
        void resetState(){
            moved = false;
            resized = false;
        }
        void setActiveCorner(unsigned char h, unsigned char v = 0){
            //activeH = h;
            //activeV = v;
            activeCorner = 0;
            activeCorner = (h | v);
        }
        //void setActiveH(unsigned char h){setActiveCorner(h, activeV);}
        //void setActiveV(unsigned char v){setActiveCorner(activeH, v);}
        void flipH(){
            activeCorner ^= (TOP | BOTTOM);
            //if(activeH == TOP){
            //    setActiveH(BOTTOM);
            //}
            //else{
            //    setActiveH(TOP);
            //}
        }
        void flipV(){
            activeCorner ^= (LEFT | RIGHT);
            //if(activeV == LEFT){
            //    setActiveV(RIGHT);
            //}
            //else{
            //    setActiveV(LEFT);
            //}
        }

    protected:
        SelectType selectType;
        QPointF selectedPoint;
        int mytype = Generic;
        bool active = false;
        unsigned char activeV = RIGHT;
        unsigned char activeH = BOTTOM;
        unsigned char activeCorner = (activeV | activeH);
        bool isInsideRect(QRectF &rect, QPointF &point);
        void checkBoundaries(QPointF &shift, QRectF &rect);
        bool resized = false;
        bool moved = false;
};




#endif /* DIAGRAMITEM_H */

