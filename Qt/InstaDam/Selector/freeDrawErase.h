#ifndef FREEDRAWERASE_H
#define FREEDRAWERASE_H

#include "freeDrawSelect.h"
#include <QSet>
#ifdef TEST
class TestSelect;

#endif

typedef QMap<FreeDrawSelect*, FreeMap* > EraseMap;
typedef QMapIterator<FreeDrawSelect*, FreeMap* > EraseMapIterator;
class FreeDrawErase : public FreeDrawSelect
{

    public:
    static QString baseInstruction;
        FreeDrawErase(QPointF point, int brushSize, int brushMode, Label *label = nullptr, QGraphicsItem *item = nullptr);
        ~FreeDrawErase() override;
        void moveItem(QPointF &oldPos, QPointF &newPos) override;
        bool isInside(QPointF &point) override {UNUSED(point); return false;}
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override {UNUSED(painter); UNUSED(option); UNUSED(widget);}
        EraseMap* getMap(){return undoMap;}
        void drawWithSquare(QPointF &oldPos, QPointF &newPos);
        void drawWithCircle(QPointF &oldPos, QPointF &newPos);

protected:
        void rasterizeLine(QPoint &start, QPoint &end);
    private:
#ifdef TEST

        friend TestSelect;
#endif
        void voidPoint(QPoint &point);
        EraseMap *undoMap;
        QVector<int> deleteList;
//        QHash<int, SelectItem*> freeDrawItems;
};
#endif // FREEDRAWERASE_H
