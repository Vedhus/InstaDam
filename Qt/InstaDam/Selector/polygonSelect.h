#ifndef POLYGONSELECT_H
#define POLYGONSELECT_H

#include <vector>
#include <cmath>
#include "selectItem.h"

const QString polygonBaseInstruction = QString("Click points to construct a polygon. Individual points can be moved and deleted. Click 'Finish Polygon' when done.");
class PolygonSelect : public SelectItem, public QGraphicsPolygonItem
{

public:
    const QString baseInstruction = polygonBaseInstruction;
        PolygonSelect(QPointF point, Label *label=nullptr, QGraphicsItem *item = nullptr);
        ~PolygonSelect() override;
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
        void insertVertex(int vertex, QPointF &point) override;
        QString baseInstructions() override {return PolygonSelect::baseInstruction;}
        int numberOfVertices() override;
        void updatePen(QPen pen) override;
        void setMirror(SelectItem *item) override;
        void setMirrorVertex(int vertex) override;
        void setMirrorActivePoint(QPointF point);
        void updateMirrorScene() override;
        void mirrorHide() override;
        void mirrorShow() override;
        void setMirrorActive() override;
        void setMirrorMoved() override;
        void setMirrorResized() override;
        PolygonSelect* getMirror() override {return mirror;}
        void setMirrorAdded() override {mirror->hasBeenAdded = true;}

    private:
        QRectF makeVertex(QPointF &point);
        void refresh();
        bool selected = true;
        //bool lastPointAdded = false;
        QPolygonF polygon;
        QVector<QPointF> myPoints;
        QVector<QRectF> myVertices;
        PolygonSelect *mirror;
        void setMirrorPolygon(int actVert);
        void setMirrorRect(QRectF rect);
};
#endif // POLYGONSELECT_H
