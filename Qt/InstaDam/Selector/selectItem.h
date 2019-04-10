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

#define UNUSED(x) (void)(x)
class Label;
class FreeDrawSelect;

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

class SelectItem : public QGraphicsItem {
 public:
    enum SelectType {
        Rectangle = 51,
        Ellipse = 52,
        Polygon = 53,
        Freedraw = 54,
        Freeerase = 55};

    enum Vertex:int{
        NONE = 0x0,
        TOP = 0x1,
        BOTTOM = 0x2,
        LEFT = 0x4,
        RIGHT = 0x8,
        UNSELECTED = INT_MAX
    };

    static int ID;
    static QSize myBounds;
    static qreal vertexSize;
    static QPointF xoffset;
    static QPointF yoffset;
    int myID;
    QPen highlightPen, myPen;

    SelectItem(qreal vertSize = 10.,
               QSharedPointer<Label> label = nullptr,
               QGraphicsItem *item = nullptr);
    SelectItem(QSharedPointer<Label> label = nullptr,
               QGraphicsItem *item = nullptr);
    ~SelectItem() override {}

    /*---------------- Virtual functions ---------------------------*/
    // manipulating the data
    virtual void addPoint(QPointF &point,
                          const int vertex = UNSELECTED) = 0;
    virtual void clickPoint(const QPointF &point) = 0;
    virtual void insertVertex(const int vertex, const QPointF &point) = 0;
    virtual void moveItem(const QPointF &oldPos, QPointF &newPos) = 0;
    virtual void removeVertex(const int vertex = UNSELECTED) = 0;
    virtual void resetActiveVertex() = 0;
    virtual void resizeItem(const int vertex, QPointF &shift) = 0;
    virtual void rotate(const QPointF &from, const QPointF &to) = 0;
    virtual void updatePen(QPen pen) = 0;

    // I/O
    virtual void read(const QJsonObject &json) = 0;
    virtual void write(QJsonObject &json) const = 0;
    virtual void toPixmap(QPainter* painter) = 0;
    // get info about the objects
    virtual QString baseInstructions() const = 0;
    virtual bool isInside(const QPointF &point) const = 0;
    virtual int numberOfVertices() const = 0;

    // working with the mirror (photo/maks pair)
    virtual SelectItem* getMirror() const = 0;
    virtual void mirrorHide() const = 0;
    virtual void mirrorShow() const = 0;
    virtual void rotateMirror() const = 0;
    virtual void setMirror(SelectItem *item) = 0;
    virtual void setMirrorActive() const = 0;
    virtual void setMirrorAdded() const = 0;
    virtual void setMirrorMoved() const = 0;
    virtual void setMirrorResized() const = 0;
    virtual void setMirrorVertex(int vertex) const = 0;
    virtual void updateMirrorScene() const = 0;
    /*-------------- End virtual functions-------------------------*/

    QGraphicsItem* getParentItem() const;
    void invertColorForPen();
    QGraphicsScene* scene() const;
    void setLabel(QSharedPointer<Label> label, bool init = false);
    QSharedPointer<Label> getLabel() const {return myLabel;}
    static void setVertexSize(qreal size);
    void sortCorners(QRectF &rect, QPointF &newPoint);
    int type() const override;

    /*-------------- Inlines ---------------------------------------*/
    void flipH() {
        activeVertex ^= (TOP | BOTTOM);
    }

    void flipV() {
        activeVertex ^= (LEFT | RIGHT);
    }

    QPointF getActivePoint() const {return activePoint;}
    int getActiveVertex() const {return activeVertex;}
    bool isItemActive()const {return active;}
    bool isItemAdded()const {return hasBeenAdded;}
    void itemWasAdded() {
        setMirrorAdded();
        hasBeenAdded = true;
    }

    void resetState() {
        moved = false;
        resized = false;
        setMirrorMoved();
        setMirrorResized();
        pointAdded = false;
        rotated = false;
        setActiveVertex(SelectItem::UNSELECTED);
    }

    void setActiveVertex(int h, int v = NONE) {
        activeVertex = 0;
        activeVertex = (h | v);
    }

    void setInactive() {active = false;}
    void setItemActive() {
        setMirrorActive();
        active = true;
    }
    bool wasMoved() const {return moved;}
    bool wasPointAdded() const {return pointAdded;}
    bool wasResized() const {return resized;}
    bool wasRotated() const {return rotated;}
    void setFromMaskScene(bool value) {fromMaskScene = value;}
    void setOnMaskScene(bool value) {onMaskScene = value;}
    bool getOnMaskScene() const {return onMaskScene;}
    bool getFromMaskScene() const {return fromMaskScene;}
    void hideMask() {
        if (onMaskScene) {
            if (!fromMaskScene)
                hide();
        } else if (getMirror() != nullptr && !getMirror()->fromMaskScene) {
            mirrorHide();
        }
    }
    void showMask() {
        if (onMaskScene) {
            if (!fromMaskScene && getMirror() && getMirror()->isVisible()) {
                show();
            }
        } else if (getMirror() != nullptr && !getMirror()->fromMaskScene &&
                isVisible()) {
            mirrorShow();
        }
    }
#ifdef TEST
    friend class IntegrationTest;
#endif
 protected:
    SelectType selectType;

    QPointF selectedPoint;
    QPointF activePoint;

    QRectF myRect;

    QPen pen;

    QSharedPointer<Label> myLabel;

    bool active = false;
    bool hasBeenAdded = false;
    bool moved = false;
    bool pointAdded = false;
    bool resized = false;
    bool rotated = false;
    bool fromMaskScene = false;
    bool onMaskScene = false;

    int mytype;
    int activeV = RIGHT;
    int activeH = BOTTOM;
    int activeVertex = (activeV | activeH);

    FreeDrawSelect* pixmap;
    void checkBoundaries(const QPointF &shift, QRectF &rect);
    bool isInsideRect(const QRectF &rect, const QPointF &point) const;
};

#endif

