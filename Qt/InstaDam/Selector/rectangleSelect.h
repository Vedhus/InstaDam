#ifndef RECTANGLE_SELECT_H
#define RECTANGLE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif
const QString rectBaseString = QString("Click to define a corner, then drag the mouse (while holding the button down) to define the rectangle. Use the right mouse button to rotate.");
class RectangleSelect : public BoxBasedSelector, public QGraphicsRectItem
{

public:
    const QString baseInstruction = rectBaseString;

    RectangleSelect(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    RectangleSelect(QPointF point, qreal vertSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~RectangleSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    QString baseInstructions() override {return RectangleSelect::baseInstruction;}
    QRectF boundingRect() const override;
    bool isInside(QPointF &point) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void updatePen(QPen pen) override;
    void setRectUnchecked(QRectF rect) override;

    // Mirror
    RectangleSelect* getMirror() override {return mirror;}
    void mirrorHide() override;
    void mirrorShow() override;
    void rotateMirror() override;
    void setMirror(SelectItem* item) override;
    void setMirrorActive() override;
    void setMirrorAdded() override {mirror->hasBeenAdded = true;}
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) override;
    void setMirrorMoved() override;
    void setMirrorResized() override;
    void setMirrorVertex(int vertex) override;
    void updateMirrorScene() override;
    /*------------- End implemented functions*/

    int type(){return SelectItem::type();}

    QGraphicsScene* scene();

private:
    RectangleSelect *mirror = nullptr;
#ifdef TEST
        friend TestSelect;
#endif
};

#endif // RECTANGLE_SELECT_H
