#ifndef ELLIPSE_SELECT_H
#define ELLIPSE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif

class EllipseSelect : public QGraphicsEllipseItem, public BoxBasedSelector
{

public:
    static QString baseInstruction;
    EllipseSelect(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    EllipseSelect(QPointF point, qreal vertexSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~EllipseSelect() override;
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    //void resizeItem(int vertex, QPointF &shift) override;
    //void clickPoint(QPointF &point) override;
    void updatePen(QPen pen) override;
    QRectF boundingRect() const override;
    bool isInside(QPointF &point) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QGraphicsScene* scene();
    QString baseInstructions() override {return EllipseSelect::baseInstruction;}
    void setMirror(SelectItem* item) override;
    void setRectUnchecked(QRectF rect) override;
    void setMirrorVertex(int vertex) override;
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) override;
    void updateMirrorScene() override;
    void mirrorHide() override;
    void mirrorShow() override;
    void setMirrorActive() override;
    void setMirrorMoved() override;
    void setMirrorResized() override;
    EllipseSelect* getMirror() override {return mirror;}
    void setMirrorAdded() override {mirror->hasBeenAdded = true;}

private:
        EllipseSelect *mirror;
#ifdef TEST
private:
        friend TestSelect;
#endif
        //void calcCorners();
        //QRectF tl, bl, tr, br;
};
#endif // ELLIPSE_SELECT_H
