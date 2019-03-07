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
#include <QPainter>
//#include "label.h"
#define UNUSED(x) (void)(x)
class Label;
const int UNSELECTED = INT_MAX;

enum SelectType:int {Rect=51, Ellipse=52,Polygon=53, Freedraw=54, Freeerase=55};

const int TOP = 0x1;
const int BOTTOM = 0x2;
const int LEFT = 0x4;
const int RIGHT = 0x8;
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
        static int ID;
        static QSize myBounds;
        static qreal vertexSize;
        static QPointF xoffset;
        static QPointF yoffset;

        SelectItem(qreal vertSize = 10., Label *label=nullptr, QGraphicsItem *item = nullptr);
        SelectItem(Label *label=nullptr, QGraphicsItem *item = nullptr);
        virtual ~SelectItem() override {}
        virtual void addPoint(QPointF &point, int vertex = UNSELECTED) = 0;
        virtual void moveItem(QPointF &oldPos, QPointF &newPos) = 0;
        virtual void resizeItem(int vertex, QPointF &shift) = 0;
        virtual void clickPoint(QPointF &point) = 0;
        virtual bool isInside(QPointF &point) = 0;
        virtual void removeVertex(int vertex = UNSELECTED) = 0;
        virtual void resetActiveVertex() = 0;
        virtual void insertVertex(int vertex, QPointF &point) = 0;
        virtual QString baseInstructions() = 0;
        virtual int numberOfVertices() = 0;
        virtual void updatePen(QPen pen) = 0;
        virtual void setMirror(SelectItem *item) = 0;
        virtual void setMirrorVertex(int vertex) = 0;
        virtual void updateMirrorScene() = 0;
        virtual void mirrorHide() = 0;
        virtual void mirrorShow() = 0;
        virtual void setMirrorActive() = 0;

        void sortCorners(QRectF &rect, QPointF &newPoint);
        int type() const override;
        static void setVertexSize(qreal size);
        QGraphicsScene* scene();
        QGraphicsItem* getParentItem();
        void setLabel(Label *label);
        void invertColorForPen();

        void resetState(){
            moved = false;
            resized = false;
            pointAdded = false;
            setActiveVertex(UNSELECTED);
        }
        void setActiveVertex(int h, int v = 0){
            activeVertex = 0;
            activeVertex = (h | v);
        }
        void flipH(){
            activeVertex ^= (TOP | BOTTOM);
        }
        void flipV(){
            activeVertex ^= (LEFT | RIGHT);
        }

        bool wasResized(){return resized;}
        bool wasMoved(){return moved;}
        bool wasPointAdded(){return pointAdded;}
        int getActiveVertex(){return activeVertex;}
        QPointF getActivePoint(){return activePoint;}
        void itemWasAdded(){hasBeenAdded = true;}
        bool isItemAdded(){return hasBeenAdded;}
        bool isItemActive(){return active == true;}
        void setItemActive(){setMirrorActive(); active = true;}
        void setInactive(){active = false;}

        int myID;
    protected:
        QPen highlightPen, myPen;
        SelectType selectType;
        QPointF selectedPoint;
        QRectF myRect;
        QPointF activePoint;
        QPen pen;
        Label *myLabel;

        bool active = false;
        bool pointAdded = false;
        bool resized = false;
        bool moved = false;
        bool hasBeenAdded = false;
        int mytype;
        int activeV = RIGHT;
        int activeH = BOTTOM;
        int activeVertex = (activeV | activeH);

        bool isInsideRect(QRectF &rect, QPointF &point);
        void checkBoundaries(QPointF &shift, QRectF &rect);

};




#endif

