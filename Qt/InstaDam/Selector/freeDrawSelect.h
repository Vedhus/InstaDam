#ifndef FREEDRAWSELECT_H
#define FREEDRAWSELECT_H

#include "selectItem.h"
#include <QHash>
#include <QPixmap>
#ifdef TEST
class TestSelect;

#endif

//typedef QVector<QVector <bool> > Block;
class FreeDrawSelect : public QGraphicsPixmapItem, public SelectItem
{
public:
    static QString baseInstruction;
    FreeDrawSelect();
    FreeDrawSelect(QPixmap map, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    FreeDrawSelect(QPixmap map, QPen pen);
    FreeDrawSelect(const QJsonObject &json, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    FreeDrawSelect(const QList<FreeDrawSelect*> items);
    FreeDrawSelect(QPointF point, QPen pen, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    FreeDrawSelect(QPointF point, int brushSize, Qt::PenCapStyle brushMode, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
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
    void toPixmap(QPainter *painter) override;

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

    void addPoints(QSharedPointer<QPixmap> points);
    void deletePoints(QPen &pen, QSharedPointer<QPixmap> map);
    void deletePoints(QPointF &start, QPointF &end, QPen pen, QSharedPointer<QPixmap> outmap);
    void setPointsUnchecked(QPixmap map);

    QGraphicsScene* scene();
    bool isVisible(){return SelectItem::isVisible();}
    void setRecalc(){needToPixmap = true;}
    QPixmap getPixmap(){return myPixmap;}

protected:
    QPixmap myPixmap;


    QPoint lastPoint;

private:
#ifdef TEST

    friend TestSelect;
#endif
    FreeDrawSelect *mirror = nullptr;
    bool needToPixmap = false;
    QPainter myPainter;
};
#endif // POLYGONSELECT_H
