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
#include <QJsonObject>
//#include "label.h"
#define UNUSED(x) (void)(x)
class Label;
const int UNSELECTED = INT_MAX;

enum SelectType:int {RectangleObj=51, EllipseObj=52, PolygonObj=53, FreedrawObj=54, FreeeraseObj=55};

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
    int myID;
    QPen highlightPen, myPen;

    SelectItem(qreal vertSize = 10., Label *label=nullptr, QGraphicsItem *item = nullptr);
    SelectItem(Label *label=nullptr, QGraphicsItem *item = nullptr);
    virtual ~SelectItem() override {}

    /*---------------- Virtual functions ---------------------------*/
    // manipulating the data
    virtual void addPoint(QPointF &point, int vertex = UNSELECTED) = 0;
    virtual void clickPoint(QPointF &point) = 0;
    virtual void insertVertex(int vertex, QPointF &point) = 0;
    virtual void moveItem(QPointF &oldPos, QPointF &newPos) = 0;
    virtual void removeVertex(int vertex = UNSELECTED) = 0;
    virtual void resetActiveVertex() = 0;
    virtual void resizeItem(int vertex, QPointF &shift) = 0;
    virtual void rotate(QPointF &from, QPointF &to) = 0;
    virtual void updatePen(QPen pen) = 0;

    // I/O
    virtual void read(const QJsonObject &json) = 0;
    virtual void write(QJsonObject &json) const = 0;
    // get info about the objects
    virtual QString baseInstructions() = 0;
    virtual bool isInside(QPointF &point) = 0;
    virtual int numberOfVertices() = 0;

    // working with the mirror (photo/maks pair)
    virtual SelectItem* getMirror() = 0;
    virtual void mirrorHide() = 0;
    virtual void mirrorShow() = 0;
    virtual void rotateMirror() = 0;
    virtual void setMirror(SelectItem *item) = 0;
    virtual void setMirrorActive() = 0;
    virtual void setMirrorAdded() = 0;
    virtual void setMirrorMoved() = 0;
    virtual void setMirrorResized() = 0;
    virtual void setMirrorVertex(int vertex) = 0;
    virtual void updateMirrorScene() = 0;
    /*-------------- End virtual functions-------------------------*/

    QGraphicsItem* getParentItem();
    void invertColorForPen();
    QGraphicsScene* scene();
    void setLabel(Label *label);
    static void setVertexSize(qreal size);
    void sortCorners(QRectF &rect, QPointF &newPoint);
    int type() const override;

    /*-------------- Inlines ---------------------------------------*/
    void flipH(){
        activeVertex ^= (TOP | BOTTOM);
    }

    void flipV(){
        activeVertex ^= (LEFT | RIGHT);
    }

    QPointF getActivePoint(){return activePoint;}
    int getActiveVertex(){return activeVertex;}
    bool isItemActive(){return active == true;}
    bool isItemAdded(){return hasBeenAdded;}
    void itemWasAdded(){setMirrorAdded(); hasBeenAdded = true;}

    void resetState(){
        moved = false;
        resized = false;
        setMirrorMoved();
        setMirrorResized();
        pointAdded = false;
        rotated = false;
        setActiveVertex(UNSELECTED);
    }

    void setActiveVertex(int h, int v = 0){
        activeVertex = 0;
        activeVertex = (h | v);
    }

    void setInactive(){active = false;}
    void setItemActive(){setMirrorActive(); active = true;}
    bool wasMoved(){return moved;}
    bool wasPointAdded(){return pointAdded;}
    bool wasResized(){return resized;}
    bool wasRotated(){return rotated;}

protected:
    SelectType selectType;

    QPointF selectedPoint;
    QPointF activePoint;

    QRectF myRect;

    QPen pen;

    Label *myLabel;

    bool active = false;
    bool hasBeenAdded = false;
    bool moved = false;
    bool pointAdded = false;
    bool resized = false;
    bool rotated = false;

    int mytype;
    int activeV = RIGHT;
    int activeH = BOTTOM;
    int activeVertex = (activeV | activeH);

    void checkBoundaries(QPointF &shift, QRectF &rect);
    bool isInsideRect(QRectF &rect, QPointF &point);
};




#endif

