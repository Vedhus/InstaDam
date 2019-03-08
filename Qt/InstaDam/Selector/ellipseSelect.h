#ifndef ELLIPSE_SELECT_H
#define ELLIPSE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif
const QString ellipseBaseString = QString("Click to define a corner, then drag the mouse (while holding the button down) to define the ellipse. Use the right mouse button to rotate.");

class EllipseSelect : public QGraphicsEllipseItem, public BoxBasedSelector
{

public:
    const QString baseInstruction = ellipseBaseString;
    EllipseSelect(QPointF point, Label *label = nullptr, QGraphicsItem *item = nullptr);
    EllipseSelect(QPointF point, qreal vertexSize, Label *label = nullptr, QGraphicsItem *item = nullptr);
    ~EllipseSelect() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    void addPoint(QPointF &point, int vertex = UNSELECTED) override;
    QString baseInstructions() override {return EllipseSelect::baseInstruction;}
    QRectF boundingRect() const override;
    bool isInside(QPointF &point) override;
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setRectUnchecked(QRectF rect) override;
    void updatePen(QPen pen) override;

    // Mirror functions
    EllipseSelect* getMirror() override {return mirror;}
    void mirrorHide() override;
    void mirrorShow() override;
    void rotateMirror() override;
    void setMirror(SelectItem* item) override;
    void setMirrorActive() override;
    void setMirrorAdded() override {mirror->hasBeenAdded = true;}
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc) override;
    void setMirrorMoved() override;
    void setMirrorResized() override;
    void setMirrorVertex(int vertex) override;
    void updateMirrorScene() override;
    /*------------- End implemented functions*/


    QGraphicsScene* scene();

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
