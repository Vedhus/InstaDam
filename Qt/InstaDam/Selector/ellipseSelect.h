#ifndef ELLIPSE_SELECT_H
#define ELLIPSE_SELECT_H

#include "boxbasedselector.h"
#ifdef TEST
class TestSelect;

#endif
const QString ellipseBaseString = QString("Click to define a corner, then drag the mouse (while holding the button down) to define the ellipse. Use the right mouse button to rotate.");

class EllipseSelect : public QGraphicsEllipseItem, public BoxBasedSelector {
 public:
    const QString baseInstruction = ellipseBaseString;
    EllipseSelect();
    EllipseSelect(const QJsonObject &json,
                  QSharedPointer<Label> label = nullptr,
                  QGraphicsItem *item = nullptr);
    EllipseSelect(QPointF point, QSharedPointer<Label> label = nullptr,
                  QGraphicsItem *item = nullptr);
    EllipseSelect(QPointF point, qreal vertexSize,
                  QSharedPointer<Label>label = nullptr,
                  QGraphicsItem *item = nullptr);
    ~EllipseSelect() override;

    /*-------------- Implemented functions from SelectItem ---------*/
    void addPoint(QPointF &point, const int vertex = UNSELECTED) override;
    QString baseInstructions() const override {
        return EllipseSelect::baseInstruction;
    }
    QRectF boundingRect() const override;
    bool isInside(const QPointF &point) const override;
    void moveItem(const QPointF &oldPos, QPointF &newPos) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void setRectUnchecked(QRectF rect) override;
    void updatePen(QPen pen) override;
    void toPixmap(QPainter *painter) override;
    void setOpacity(qreal val);

    // Mirror functions
    EllipseSelect* getMirror() const override {return mirror;}
    void mirrorHide() const override;
    void mirrorShow() const override;
    void rotateMirror() const override;
    void setMirror(SelectItem* item) override;
    void setMirrorActive() const override;
    void setMirrorAdded() const override {mirror->hasBeenAdded = true;}
    void setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc,
                          QRectF brc) const override;
    void setMirrorMoved() const override;
    void setMirrorResized() const override;
    void setMirrorVertex(int vertex) const override;
    void updateMirrorScene() const override;
    /*------------- End implemented functions*/


    QGraphicsScene* scene();
    bool isVisible() const {return SelectItem::isVisible();}
    int type() const override {return SelectItem::type();}

 private:
    EllipseSelect *mirror = nullptr;
#ifdef TEST
    friend TestSelect;
#endif
};
#endif  // ELLIPSE_SELECT_H
