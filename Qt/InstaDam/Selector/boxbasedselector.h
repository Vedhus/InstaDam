#ifndef BOXBASEDSELECTOR_H
#define BOXBASEDSELECTOR_H
#include "selectItem.h"

class BoxBasedSelector : public SelectItem
{
public:
    BoxBasedSelector(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    BoxBasedSelector(QPointF point, qreal vertSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~BoxBasedSelector() override;

    /*---------------- Virtual functions ---------------------------*/
    virtual void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) = 0;
    virtual void setRectUnchecked(QRectF rect) = 0;
    /*-------------- End virtual functions-------------------------*/

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void clickPoint(QPointF &point) override;
    void removeVertex(int vertex = UNSELECTED) override;
    void resetActiveVertex() override;
    void resizeItem(int vertex, QPointF &newPos) override;
    void rotate(QPointF &from, QPointF &to) override;

    void insertVertex(int vertex, QPointF &point) override {UNUSED(point);UNUSED(vertex);}
    int numberOfVertices() override {return 2;}
    /*------------- End implemented functions*/

protected:
    QRectF tl, bl, tr, br;

    qreal myRotation = 0;

    void calcCorners(bool mir = true);
};

#endif // BOXBASEDSELECTOR_H
