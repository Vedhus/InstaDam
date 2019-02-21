#ifndef RECTANGLE_SELECT_H
#define RECTANGLE_SELECT_H

#include "selectItem.h"
#ifdef TEST
class TestSelect;

#endif

class RectangleSelect : public SelectItem, public QGraphicsRectItem
{
    public:
        RectangleSelect(QPointF point, QGraphicsItem *item = nullptr);
        RectangleSelect(QPointF point, qreal vertSize, QGraphicsItem *item = nullptr);
        ~RectangleSelect() override;
        void addPoint(QPointF &point) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &newPos) override;
        void clickPoint(QPointF &point) override;
        int type(){return SelectItem::type();}
        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();

    private:
#ifdef TEST
        friend TestSelect;
#endif
        void calcCorners();
        QRectF tl, bl, tr, br;
};

#endif // RECTANGLE_SELECT_H
