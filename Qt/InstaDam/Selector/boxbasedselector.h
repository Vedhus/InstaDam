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
    void init(QPointF &point) override {UNUSED(point);}
    void insertVertex(int vertex, QPointF &point) override {UNUSED(point);UNUSED(vertex);}
    int numberOfVertices() override {return 2;}
protected:
    void calcCorners();
    QRectF tl, bl, tr, br;
};

#endif // BOXBASEDSELECTOR_H
