#ifndef RECTANGLE_SELECT_H
#define RECTANGLE_SELECT_H

#include <iostream>

#include "boxbasedselector.h"

#ifdef TEST
class TestSelect;

#endif
const QString rectBaseString = QString("Click to define a corner, then drag the mouse (while holding the button down) to define the rectangle. Use the right mouse button to rotate.");
class RectangleSelect : public BoxBasedSelector, public QGraphicsRectItem {
 public:
    const QString baseInstruction = rectBaseString;
    RectangleSelect();
    RectangleSelect(const QJsonObject &json,
                    QSharedPointer<Label> label = nullptr,
                    QGraphicsItem *item = nullptr);
    RectangleSelect(QPointF point, QSharedPointer<Label> label = nullptr,
                    QGraphicsItem *item = nullptr);
    RectangleSelect(QPointF point, qreal vertSize,
                    QSharedPointer<Label> label = nullptr,
                    QGraphicsItem *item = nullptr);
    ~RectangleSelect() override;

    /*-------------- Implemented funtions from SelectItem ---------*/
    void addPoint(QPointF &point, const int vertex = UNSELECTED) override;
    QString baseInstructions() const override {
        return RectangleSelect::baseInstruction;
    }
    QRectF boundingRect() const override;
    bool isInside(const QPointF &point) const override;
    void moveItem(const QPointF &oldPos, QPointF &newPos) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void updatePen(QPen pen) override;
    void setRectUnchecked(QRectF rect) override;
    void toPixmap(QPainter *painter) override;
    void setOpacity(qreal);

    // Mirror
    RectangleSelect* getMirror() const override {return mirror;}
    void mirrorHide() const override;
    void mirrorShow() const override;
    void rotateMirror() const override;
    void setMirror(SelectItem* item) override;
    void setMirrorActive() const override;
    void setMirrorAdded() const override {mirror->hasBeenAdded = true;}
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc,
                          QRectF brc) const override;
    void setMirrorMoved() const override;
    void setMirrorResized() const override;
    void setMirrorVertex(int vertex) const override;
    void updateMirrorScene() const override;
    /*------------- End implemented functions*/

    bool isVisible() {return SelectItem::isVisible();}
    int type() const override {return SelectItem::type();}

    QGraphicsScene* scene();
#ifdef TEST
    friend TestSelect;
#endif

 private:
    RectangleSelect *mirror = nullptr;
};

#endif  // RECTANGLE_SELECT_H
