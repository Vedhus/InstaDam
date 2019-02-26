#ifndef BOXBASEDSELECTOR_H
#define BOXBASEDSELECTOR_H
#include "selectItem.h"

class BoxBasedSelector : public SelectItem
{
public:
    BoxBasedSelector(QPointF point, QGraphicsItem *item = nullptr);
    BoxBasedSelector(QPointF point, qreal vertSize, QGraphicsItem *item = nullptr);
    void clickPoint(QPointF &point) override;
    void resizeItem(int vertex, QPointF &newPos) override;
    void resetActiveVertex() override;
    void removeVertex(int vertex = UNSELECTED) override;
    void init(QPointF &point) override {}
protected:
    void calcCorners();
    QRectF tl, bl, tr, br;
};

#endif // BOXBASEDSELECTOR_H
