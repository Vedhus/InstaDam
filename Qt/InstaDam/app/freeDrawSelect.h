#ifndef FREEDRAWSELECT_H
#define FREEDRAWSELECT_H

#include <QHash>
#include <QPixmap>
#include "selectItem.h"
#include "photoScene.h"
#ifdef TEST
class TestSelect;

#endif

class FreeDrawSelect : public QGraphicsPixmapItem, public SelectItem {
 public:
    static QString baseInstruction;
    FreeDrawSelect();
    FreeDrawSelect(const QPixmap map, QSharedPointer<Label> label = nullptr,
                   QGraphicsItem *item = nullptr, bool importBool = false);
    FreeDrawSelect(const QPixmap map, QPen pen);
    FreeDrawSelect(const QJsonObject &json,
                   QSharedPointer<Label> label = nullptr,
                   QGraphicsItem *item = nullptr);
    explicit FreeDrawSelect(const QList<FreeDrawSelect*> &items, QSharedPointer<Label> label = nullptr);
    FreeDrawSelect(QPointF point, QPen pen,
                   QSharedPointer<Label> label = nullptr,
                   QGraphicsItem *item = nullptr);
    FreeDrawSelect(QPointF point, int brushSize, Qt::PenCapStyle brushMode,
                   QSharedPointer<Label> label = nullptr,
                   PhotoScene::viewerTypes sceneType = PhotoScene::PHOTO_VIEWER_TYPE,
                   QGraphicsItem *item = nullptr);
    ~FreeDrawSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, const int vertex = UNSELECTED) override;
    QString baseInstructions() const override {
        return FreeDrawSelect::baseInstruction;
    }
    QRectF boundingRect() const override;
    void setOpacity(qreal val);
    void setZValue(int);
    void clickPoint(const QPointF &point) override;
    void insertVertex(const int vertex, const QPointF &point) override {
        UNUSED(vertex);
        UNUSED(point);
    }
    bool isInside(const QPointF &point) const override;
    void moveItem(const QPointF &oldPos, QPointF &newPos) override;
    int numberOfVertices() const override {return 0;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void removeVertex(int vertex = UNSELECTED) override {UNUSED(vertex);}
    void resetActiveVertex() override {}
    void resizeItem(const int vertex, QPointF &oldP, QPointF &newP) override;
    void rotate(const QPointF &from, const QPointF &to) override {
        UNUSED(from);
        UNUSED(to);
    }
    void updatePen(QPen pen) override;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) const override;
    void toPixmap(QPainter *painter) override;

    // Mirror
    FreeDrawSelect* getMirror() const override {return mirror;}
    void mirrorHide() const override;
    void mirrorShow() const override;
    void rotateMirror() const override {}
    void setMirror(SelectItem *item) override;
    void setMirrorActive() const override {}
    void setMirrorAdded() const override {mirror->hasBeenAdded = true;}
    void setMirrorMoved() const override {}
    void setMirrorResized() const override {}
    void setMirrorVertex(int vertex) const override {UNUSED(vertex);}
    void updateMirrorScene() const override;

    void setMirrorMap();
    /*------------- End implemented functions*/
    bool foundPixels = false;
    void addPoints(QSharedPointer<QPixmap> points);
    void deletePoints(QPen &pen, QSharedPointer<QPixmap> map);
    void deletePoints(const QPointF &start, const QPointF &end, QPen pen,
                      QSharedPointer<QPixmap> outmap);
    void setPointsUnchecked(QPixmap map);

    QGraphicsScene* scene() const;
    bool isVisible() const {return SelectItem::isVisible();}
    QPixmap getPixmap() const {return myPixmap;}
    void importPixmap(const QPixmap);

 protected:
    QPixmap myPixmap;
    QPoint lastPoint;

 private:
#ifdef TEST
    friend TestSelect;
#endif
    FreeDrawSelect *mirror = nullptr;
    QPainter myPainter;
    void loadFromPixmap(const QPixmap map, bool importBool = false);
    void setup();
    void init(QSharedPointer<Label> label = nullptr, PhotoScene::viewerTypes sceneType = PhotoScene::PHOTO_VIEWER_TYPE);
};
#endif  // POLYGONSELECT_H
