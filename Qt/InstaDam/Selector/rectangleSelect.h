#ifndef RECTANGLE_SELECT_H
#define RECTANGLE_SELECT_H

#include "selectItem.h"

class RectangleSelect : public SelectItem, public QGraphicsRectItem
{
    public:
        RectangleSelect(QPointF point, QGraphicsItem *item = nullptr);

        void addPoint(QPointF &point) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &shift) override;
        void clickPoint(QPointF &point) override;

        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();

    private:
        void calcCorners();
        QRectF tl, bl, tr, br;
};

#endif // RECTANGLE_SELECT_H
