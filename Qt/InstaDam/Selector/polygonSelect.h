#ifndef POLYGONSELECT_H
#define POLYGONSELECT_H

#include <vector>
#include "selectItem.h"


class PolygonSelect : public SelectItem, public QGraphicsPolygonItem
{
    public:
        PolygonSelect(QPointF point, QGraphicsItem *item = nullptr);

        void addPoint(QPointF &point, int vertex = UNSELECTED) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &shift) override;
        void clickPoint(QPointF &point) override;
        void resetActiveVertex() override;
        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();
        void movePoint(QPointF &point);
        void checkPoint(QPointF &point);
        void removeVertex(int vertex=UNSELECTED) override;
        qreal magnitude(QPointF point){return std::sqrt(std::pow(point.x(), 2.) + std::pow(point.y(), 2.));}
    private:
        QRectF makeVertex(QPointF &point);
        void refresh();
        bool selected = true;
        //bool lastPointAdded = false;
        QPolygonF polygon;
        QVector<QPointF> myPoints;
        QVector<QRectF> myVertices;
        QVector<qreal> constant, multiple;
};
#endif // POLYGONSELECT_H
