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
    PolygonSelect();
    PolygonSelect(const QJsonObject &json, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    PolygonSelect(QPointF point, QSharedPointer<Label> label=nullptr, QGraphicsItem *item = nullptr);
    ~PolygonSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    QString baseInstructions() override {return PolygonSelect::baseInstruction;}
    QRectF boundingRect() const override;
    void clickPoint(QPointF &point) override;
    void insertVertex(int vertex, QPointF &point) override;
    bool isInside(QPointF &point) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    int numberOfVertices() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void removeVertex(int vertex=UNSELECTED) override;
    void resetActiveVertex() override;
    void resizeItem(int vertex, QPointF &shift) override;
    void rotate(QPointF &from, QPointF &to) override {UNUSED(from); UNUSED(to);}
    void updatePen(QPen pen) override;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) const override;
    void toPixmap(QPainter *painter) override;

    // Mirror
    PolygonSelect* getMirror() override {return mirror;}
    void mirrorHide() override;
    void mirrorShow() override;
    void rotateMirror() override {}
    void setMirror(SelectItem *item) override;
    void setMirrorActive() override;
    void setMirrorActivePoint(QPointF point);
    void setMirrorAdded() override {mirror->hasBeenAdded = true;}
    void setMirrorMoved() override;
    void setMirrorResized() override;
    void setMirrorVertex(int vertex) override;
    void updateMirrorScene() override;

    void setMirrorPolygon(int actVert);
    /*------------- End implemented functions*/

    void checkPoint(QPointF &point);
    bool isVisible(){return SelectItem::isVisible();}
    qreal magnitude(QPointF point){return std::sqrt(std::pow(point.x(), 2.) + std::pow(point.y(), 2.));}
    void movePoint(QPointF &point);
    QPolygonF getPolygon(){return polygon;}

    QGraphicsScene* scene();
    void setOpacity(qreal);

private:
    bool selected = true;

    QPolygonF polygon;

    QVector<QPointF> myPoints;
    QVector<QRectF> myVertices;

    PolygonSelect *mirror;

    QRectF makeVertex(QPointF &point);

    void refresh();

    void setMirrorRect(QRectF rect);
};
#endif // POLYGONSELECT_H
