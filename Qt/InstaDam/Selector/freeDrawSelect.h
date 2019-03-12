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
    FreeDrawSelect();
    FreeDrawSelect(const QJsonObject &json, Label *label = nullptr, QGraphicsItem *item = nullptr);
    FreeDrawSelect(const QList<FreeDrawSelect*> items);
    FreeDrawSelect(QPointF point, int brushSize, int brushMode, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~FreeDrawSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    QString baseInstructions() override {return FreeDrawSelect::baseInstruction;}
    QRectF boundingRect() const override;
    void clickPoint(QPointF &point) override;
    void insertVertex(int vertex, QPointF &point) override {UNUSED(vertex);UNUSED(point);}
    bool isInside(QPointF &point) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    int numberOfVertices() override {return 0;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void removeVertex(int vertex=UNSELECTED) override {UNUSED(vertex);}
    void resetActiveVertex() override{}
    void resizeItem(int vertex, QPointF &shift) override;
    void rotate(QPointF &from, QPointF &to) override {UNUSED(from); UNUSED(to);}
    void updatePen(QPen pen) override;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) const override;

    // Mirror
    FreeDrawSelect* getMirror() override {return mirror;}
    void mirrorHide() override;
    void mirrorShow() override;
    void rotateMirror() override {}
    void setMirror(SelectItem *item) override;
    void setMirrorActive() override {}
    void setMirrorAdded() override {mirror->hasBeenAdded = true;}
    void setMirrorMoved() override {}
    void setMirrorResized() override {}
    void setMirrorVertex(int vertex) override {UNUSED(vertex);}
    void updateMirrorScene() override;

    void setMirrorMap();
    /*------------- End implemented functions*/

    void addPoints(FreeMap *points);
    void checkPoint(QPointF &point);
    void deletePoint(int points, FreeMap *delHash);
    void deletePoints(QVector<int> &points, FreeMap *delHash);
    void drawWithCircle(QPointF &oldPos, QPointF &newPos);
    void drawWithSquare(QPointF &oldPos, QPointF &newPos);
    QPolygon getPoints(){return QPolygon::fromList(myMap->values());}
    //void movePoint(QPointF &point);

    QGraphicsScene* scene();

protected:
    FreeMap *myMap = nullptr;

    int brushType;
    int fullWidth;
    int halfWidth;

    QPoint lastPoint;

    int coordsToInt(int x, int y){return ((y * SelectItem::myBounds.width()) + x);}
    int pointToInt(QPoint point){return coordsToInt(point.x(), point.y());}
    void rasterizeLine(QPoint &start, QPoint &end);

private:
#ifdef TEST

    friend TestSelect;
#endif
    FreeDrawSelect *mirror = nullptr;

    void calcRect();
    void checkPoint(QPoint &point);
};
#endif // POLYGONSELECT_H
