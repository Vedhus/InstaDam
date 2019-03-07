#ifndef BOXBASEDSELECTOR_H
#define BOXBASEDSELECTOR_H
#include "selectItem.h"

class BoxBasedSelector : public SelectItem
{
public:
    BoxBasedSelector(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    BoxBasedSelector(QPointF point, qreal vertSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~BoxBasedSelector() override;
    void clickPoint(QPointF &point) override;
    void resizeItem(int vertex, QPointF &newPos) override;
    void resetActiveVertex() override;
    void removeVertex(int vertex = UNSELECTED) override;
    void insertVertex(int vertex, QPointF &point) override {UNUSED(point);UNUSED(vertex);}
    int numberOfVertices() override {return 2;}
    virtual void setRectUnchecked(QRectF rect) = 0;
    virtual void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) = 0;
protected:
    void calcCorners(bool mir = true);
    QRectF tl, bl, tr, br;
};

#endif // BOXBASEDSELECTOR_H
