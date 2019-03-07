#ifndef RECTANGLE_SELECT_H
#define RECTANGLE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif

class RectangleSelect : public BoxBasedSelector, public QGraphicsRectItem
{

public:
    static QString baseInstruction;
    RectangleSelect(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    RectangleSelect(QPointF point, qreal vertSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~RectangleSelect() override;
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    //void resizeItem(int vertex, QPointF &newPos) override;
    //void clickPoint(QPointF &point) override;
    void updatePen(QPen pen) override;
    //int type(){return SelectItem::type();}
    QRectF boundingRect() const override;
    bool isInside(QPointF &point) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QGraphicsScene* scene();
    QString baseInstructions() override {return RectangleSelect::baseInstruction;}
    void setRectUnchecked(QRectF rect) override;
    void setMirror(SelectItem* item) override;
    void setMirrorVertex(int vertex) override;
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) override;
    void updateMirrorScene() override;
    void mirrorHide() override;
    void mirrorShow() override;
    void setMirrorActive() override;

private:
    RectangleSelect *mirror = nullptr;
#ifdef TEST
        friend TestSelect;
#endif
};

#endif // RECTANGLE_SELECT_H
