#ifndef ELLIPSE_SELECT_H
#define ELLIPSE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif

class EllipseSelect : public QGraphicsEllipseItem, public BoxBasedSelector
{

    public:
    static QString baseInstruction;
        EllipseSelect(QPointF point, QGraphicsItem *item = nullptr);
        EllipseSelect(QPointF point, qreal vertexSize, QGraphicsItem *item = nullptr);
        void addPoint(QPointF &point, int vertex = UNSELECTED) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        //void resizeItem(int vertex, QPointF &shift) override;
        //void clickPoint(QPointF &point) override;
        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();
        QString baseInstructions() override {return EllipseSelect::baseInstruction;}

#ifdef TEST
private:
        friend TestSelect;
#endif
        //void calcCorners();
        //QRectF tl, bl, tr, br;
};
#endif // ELLIPSE_SELECT_H
