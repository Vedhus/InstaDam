#ifndef TESTRECTANGLESELECT_H
#define TESTRECTANGLESELECT_H
#include <QtTest/QtTest>
#include "../rectangleSelect.h"
#include "../ellipseSelect.h"

class TestSelect: public QObject
{
    Q_OBJECT

private slots:
    void testInit();
    void testActive();
    void testActiveVertex();
    void testRectAddPoint();
    void testRectMoveItem();
    void testRectResizeItem();
    void testRectClickPoint();
    void testRectIsInside();
    void testSortCorners();
    void testGetType();
    void testRectBoundingRect();
    void testCheckBoundaries();
    void testScene();
    void testGetParentItem();
    void testEllipseAddPoint();
    void testEllipseMoveItem();
    void testEllipseResizeItem();
    void testEllipseClickPoint();
    void testEllipseBoundingRect();
    void testEllipseIsInside();

private:
    QPointF point = QPointF(5., 10.);
    QPointF brc = QPointF(20.,20.);
    QPointF tlc = QPointF(1.,1.);
    QPointF insidePoint = QPointF(15.,15.);
    QPointF outsidePoint = QPointF(30.,30.);
    QPointF insideTLC = QPointF(6.,11.);
    QPointF insideTRC = QPointF(19.,11.);
    QPointF insideBLC = QPointF(6.,19.);
    QPointF insideBRC = QPointF(19.,19.);
    RectangleSelect *ritem;
    EllipseSelect *eitem;
};

#endif // TESTRECTANGLESELECT_H
