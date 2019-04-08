#ifndef TESTELLIPSESELECT_H
#define TESTELLIPSESELECT_H
#include <QtTest/QtTest>
#include "../ellipseSelect.h"

class TestEllipseSelect: public QObject
{
    Q_OBJECT

private slots:
    void testAddPoint();
    void testMoveItem();
    void testResizeItem();
    void testClickPoint();
    void testBoundingRect();
    void testIsInside();
private:
    QPointF point = QPointF(5., 10.);
    QPointF brc = QPointF(20.,20.);
    QPointF tlc = QPointF(1.,1.);
    QPointF insidePoint = QPointF(15.,15.);
    QPointF outsidePoint = QPointF(30.,30.);
    QPointF inOutside = QPointF(9., 11.);
    QPointF insideTLC = QPointF(6.,11.);
    QPointF insideTRC = QPointF(19.,11.);
    QPointF insideBLC = QPointF(6.,19.);
    QPointF insideBRC = QPointF(19.,19.);
    EllipseSelect *eitem;
};

#endif // TESTELLIPSESELECT_H
