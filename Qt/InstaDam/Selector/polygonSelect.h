#ifndef POLYGONSELECT_H
#define POLYGONSELECT_H

#include <vector>
#include "selectItem.h"

const int UNSELECTED = INT_MAX;

class PolygonSelect : public SelectItem, public QGraphicsPolygonItem
{
    public:
        PolygonSelect(QPointF point, QGraphicsItem *item = nullptr);

        void addPoint(QPointF &point) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &shift) override;
        void clickPoint(QPointF &point) override;

        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();
        void movePoint(QPointF &point);
        void checkPoint(QPointF &point);
        qreal magnitude(QPointF point){return std::sqrt(std::pow(point.x(), 2.) + std::pow(point.y(), 2.));}

    private:
        QRectF makeVertex(QPointF &point);
        void preCalc();
        QPolygonF polygon;
        QVector<QPointF> myPoints;
        QVector<QRectF> myVertices;
        QVector<qreal> constant, multiple;
};
#endif // POLYGONSELECT_H
