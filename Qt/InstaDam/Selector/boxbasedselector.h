#ifndef BOXBASEDSELECTOR_H
#define BOXBASEDSELECTOR_H
#include "selectItem.h"

class BoxBasedSelector : public SelectItem
{
public:
    BoxBasedSelector(const QJsonObject &json, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    BoxBasedSelector(QPointF point, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    BoxBasedSelector(QPointF point, qreal vertSize, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    ~BoxBasedSelector() override;

    /*---------------- Virtual functions ---------------------------*/
    virtual void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) = 0;
    virtual void setRectUnchecked(QRectF rect) = 0;
    /*-------------- End virtual functions-------------------------*/

    /*-------------- Implemented functions from SelectItem ---------*/
    void clickPoint(QPointF &point) override;
    void removeVertex(int vertex = UNSELECTED) override;
    void resetActiveVertex() override;
    void resizeItem(int vertex, QPointF &newPos) override;
    void rotate(QPointF &from, QPointF &to) override;
    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) const override;

    void insertVertex(int vertex, QPointF &point) override {UNUSED(point);UNUSED(vertex);}
    int numberOfVertices() override {return 2;}
    /*------------- End implemented functions*/

    void setRotationAngle(qreal angle) {myRotation = angle;}
    qreal getRotationAngle() {return myRotation;}
    QRectF getRect(){return myRect;}

protected:
    QRectF tl, bl, tr, br;

    qreal myRotation = 0;

    void calcCorners(bool mir = true);
};

#endif // BOXBASEDSELECTOR_H
