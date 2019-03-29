#ifndef FREEDRAWERASE_H
#define FREEDRAWERASE_H

#include "freeDrawSelect.h"
#include <QSet>
#ifdef TEST
class TestSelect;

#endif

typedef QMap<FreeDrawSelect*, QSharedPointer<QPixmap> > EraseMap;
typedef QMapIterator<FreeDrawSelect*, QSharedPointer<QPixmap> > EraseMapIterator;

class FreeDrawErase : public FreeDrawSelect
{
public:
    static QString baseInstruction;
    FreeDrawErase(QPointF point, int brushSize, Qt::PenCapStyle brushMode, QSharedPointer<Label> label = nullptr, QGraphicsItem *item = nullptr);
    ~FreeDrawErase() override;

    /*-------------- Implemented fvuntions from SelectItem ---------*/
    bool isInside(QPointF &point) override {UNUSED(point); return false;}
    void moveItem(QPointF &oldPos, QPointF &newPos) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override {UNUSED(painter); UNUSED(option); UNUSED(widget);}
    void toPixmap(QPainter *painter) override {UNUSED(painter);}

    /*------------- End implemented functions*/


    QSharedPointer<EraseMap> getMap(){return undoMap;}

private:
#ifdef TEST

    friend TestSelect;
#endif
    QVector<int> deleteList;

    QSharedPointer<EraseMap> undoMap = nullptr;

    //void voidPoint(QPoint &point);
};
#endif // FREEDRAWERASE_H
