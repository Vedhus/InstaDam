#ifndef FREEDRAWSELECT_H
#define FREEDRAWSELECT_H

#include "selectItem.h"
#include <QHash>
#ifdef TEST
class TestSelect;

#endif

//typedef QVector<QVector <bool> > Block;
typedef QHash<int, QPoint> FreeMap;
typedef QHashIterator<int, QPoint> FreeMapIterator;
class FreeDrawSelect : public QAbstractGraphicsShapeItem, public SelectItem
{

    public:
    static QString baseInstruction;
        FreeDrawSelect(QPointF point, int brushSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
        ~FreeDrawSelect() override;
        void addPoint(QPointF &point, int vertex = UNSELECTED) override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        void resizeItem(int vertex, QPointF &shift) override;
        void clickPoint(QPointF &point) override;
        void resetActiveVertex() override{}
        void removeVertex(int vertex=UNSELECTED) override {UNUSED(vertex);}
        QRectF boundingRect() const override;
        bool isInside(QPointF &point) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
        QGraphicsScene* scene();
        void movePoint(QPointF &point);
        void updatePen(QPen pen) override;
        void checkPoint(QPointF &point);
        void insertVertex(int vertex, QPointF &point) override {UNUSED(vertex);UNUSED(point);}
        QString baseInstructions() override {return FreeDrawSelect::baseInstruction;}
        int numberOfVertices() override {return 0;}
        void deletePoints(QVector<int> &points, FreeMap *delHash);
        void deletePoint(int points, FreeMap *delHash);
        void addPoints(FreeMap *points);
        void setMirror(SelectItem *item) override;
        void setMirrorVertex(int vertex) override {UNUSED(vertex);}
        void updateMirrorScene() override;
        void mirrorHide() override;
        void mirrorShow() override;
        void setMirrorActive() override {}


protected:
        FreeMap *myMap;
        int pointToInt(QPoint point){return coordsToInt(point.x(), point.y());}
        int coordsToInt(int x, int y){return ((y * SelectItem::myBounds.width()) + x);}
        void rasterizeLine(QPoint &start, QPoint &end);
        int halfWidth;
        int fullWidth;
        QPoint lastPoint;

private:
#ifdef TEST

        friend TestSelect;
#endif
        FreeDrawSelect *mirror = nullptr;
        void checkPoint(QPoint &point);
        void setMirrorMap();
};
#endif // POLYGONSELECT_H
