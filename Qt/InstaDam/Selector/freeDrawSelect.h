#ifndef FREEDRAWSELECT_H
#define FREEDRAWSELECT_H

#include "selectItem.h"
#include <QHash>

//typedef QVector<QVector <bool> > Block;

class FreeDrawSelect : public QAbstractGraphicsShapeItem, public SelectItem
{

    public:
    static QString baseInstruction;
        FreeDrawSelect(QPointF point, QGraphicsItem *item = nullptr);

        void addPoint(QPointF &point, int vertex = UNSELECTED) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &shift) override;
        void clickPoint(QPointF &point) override;
        void resetActiveVertex() override{}
        void removeVertex(int vertex=UNSELECTED) override {}
        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();
        void movePoint(QPointF &point);
        void checkPoint(QPointF &point);
        void init(QPointF &point) override;
        void insertVertex(int vertex, QPointF &point) override {}
        QString baseInstructions() override {return FreeDrawSelect::baseInstruction;}
        int numberOfVertices() override {return 0;}
    private:
        const int pixel = 5;
        QPolygon polygon;
        //QVector<QVector <Block*> > myMap;
        QHash<int, int> *myMap;
        //Block inUse;
        QPoint gridSize;
        QPointF offset;
        //QPen myPen;
        const int baseSize = 100;
        //QVector<bool> baseVector = QVector<bool>(baseSize, false);
        //QPoint getBox(QPoint &point);
        //Block* makeBlock();
        QVector<QPoint> rasterizeLine(QPoint &start, QPoint &end);
        void checkPoint(QPoint &point);
        //void updateMap(QPoint &point);
};
#endif // POLYGONSELECT_H
