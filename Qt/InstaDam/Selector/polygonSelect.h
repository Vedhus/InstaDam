#ifndef POLYGONSELECT_H
#define POLYGONSELECT_H

#include <vector>
#include <cmath>
#include "selectItem.h"

const QString polygonBaseInstruction = QString("Click points to construct a polygon. Hold the sift button down to modify. Individual points can be moved and deleted using backspace. Click 'Finish Polygon' when done.");
class PolygonSelect : public SelectItem, public QGraphicsPolygonItem {
 public:
    const QString baseInstruction = polygonBaseInstruction;
    PolygonSelect();
    PolygonSelect(const QJsonObject &json,
                  QSharedPointer<Label> label = nullptr,
                  QGraphicsItem *item = nullptr);
    PolygonSelect(QPointF point, QSharedPointer<Label> label = nullptr,
                  QGraphicsItem *item = nullptr);
    ~PolygonSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, const int vertex = UNSELECTED) override;
    QString baseInstructions() const override {
        return PolygonSelect::baseInstruction;
    }
    QRectF boundingRect() const override;
    void clickPoint(const QPointF &point) override;
    void insertVertex(const int vertex, const QPointF &point) override;
    bool isInside(const QPointF &point) const override;
    void moveItem(const QPointF &oldPos, QPointF &newPos) override;
    int numberOfVertices() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void removeVertex(int vertex = UNSELECTED) override;
    void resetActiveVertex() override;
    void resizeItem(const int vertex, QPointF &shift) override;
    void rotate(const QPointF &from, const QPointF &to) override {
        UNUSED(from);
        UNUSED(to);
    }
    void updatePen(QPen pen) override;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) const override;
    void toPixmap(QPainter *painter) override;
    void setOpacity(qreal);

    // Mirror
    PolygonSelect* getMirror() const override {return mirror;}
    void mirrorHide() const override;
    void mirrorShow() const override;
    void rotateMirror() const override {}
    void setMirror(SelectItem *item) override;
    void setMirrorActive() const override;
    void setMirrorActivePoint(QPointF point) const;
    void setMirrorAdded() const override {mirror->hasBeenAdded = true;}
    void setMirrorMoved() const override;
    void setMirrorResized() const override;
    void setMirrorVertex(int vertex) const override;
    void updateMirrorScene() const override;

    void setMirrorPolygon(int actVert);
    /*------------- End implemented functions*/

    void checkPoint(QPointF &point);
    bool isVisible() const {return SelectItem::isVisible();}
    qreal magnitude(QPointF point) const {
        return std::sqrt(std::pow(point.x(), 2.) + std::pow(point.y(), 2.));
    }
    void movePoint(const QPointF &point);
    QPolygonF getPolygon() const {return polygon;}

    QGraphicsScene* scene() const;

#ifdef TEST
    friend class TestSelect;
#endif
 protected:
    QVector<QPointF> myPoints;
    QVector<QRectF> myVertices;

 private:
    bool selected = true;

    QPolygonF polygon;

    PolygonSelect *mirror = nullptr;

    QRectF makeVertex(const QPointF &point) const;

    void refresh();
};
#endif  // POLYGONSELECT_H
