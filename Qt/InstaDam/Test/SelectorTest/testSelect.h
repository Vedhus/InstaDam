#ifndef TESTRECTANGLESELECT_H
#define TESTRECTANGLESELECT_H
#include <QtTest/QtTest>
#include "rectangleSelect.h"
#include "ellipseSelect.h"
#include "polygonSelect.h"
#include "freeDrawErase.h"
#include "freeDrawSelect.h"
#include "photoScene.h"
#include "label.h"

//int SelectItem::ID = 0;
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
    void testRMirror();
    void testRToPixmap();
    void testRRotate();
    void testRReadWrite();
    void testROpacity();
    void testSelectInlines();
    void testMaskShowHide();

    void testEReadWrite();
    void testEOpacity();
    void testEMirror();
    void testEToPixmap();
    void testERotate();

    void testPOpacity();
    void testPAddPoint();
    void testPMove();
    void testPClickPoint();
    void testPInsertVertex();
    void testPReadWrite();
    void testPRemoveVertex();
    void testPResize();
    void testPMirror();
    void testPShowHide();
    void testPScene();
    void testPisInside();
    void testPToPixmap();

    void testFreeMove();
    void testFreeStubs();
    void testFreeReadWrite();

    void testEraseMove();
    void testFreeShowHide();
    void testFreeToPixmap();

    void testLabelSetId();
    void testLabelReadWrite();
    void testLabelText();
    void testLabelAddRemoveItem();
    void testLabelSetOpacity();
    void testLabelExportLabel();
    void testLabelSetMaskState();

private:
    QSharedPointer<Label> myLabel = QSharedPointer<Label>::create();
    QPointF point = QPointF(5., 10.);
    QPointF brc = QPointF(20.,20.);
    QPointF tlc = QPointF(1.,1.);
    QPointF insidePoint = QPointF(15.,15.);
    QPointF outsidePoint = QPointF(30.,30.);
    QPointF insideTLC = QPointF(6.,11.);
    QPointF insideTRC = QPointF(19.,11.);
    QPointF insideBLC = QPointF(6.,19.);
    QPointF insideBRC = QPointF(19.,19.);

    QPointF p1 = QPointF(10., 10.);
    QPointF p2 = QPointF(20., 10.);
    QPointF p3 = QPointF(25., 35.);
    QPointF p4 = QPointF(22., 40.);
    QPointF p5 = QPointF(5., 22.);
    QPointF pnew = QPointF(25., 25.);
    QPointF pmove = QPointF(15.,15.);
    QPointF pshift = pmove - p1;
    QPointF pout = QPointF(1., 12.);
    QPointF pin = QPointF(12., 12.);
    QPointF pjin = QPointF(9., 9.);

    RectangleSelect *ritem;
    EllipseSelect *eitem;
    PolygonSelect *pitem;
    FreeDrawSelect *fitem;
    FreeDrawErase *feitem;

    void generateData();

};

#endif // TESTRECTANGLESELECT_H
