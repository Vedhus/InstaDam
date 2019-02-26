#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testEllipseClickPoint(){
    eitem = new EllipseSelect(point, 3.);
    eitem->addPoint(brc);
    eitem->clickPoint(outsidePoint);
    QCOMPARE(eitem->getActiveVertex(), 0);
    eitem->clickPoint(insidePoint);
    QCOMPARE(eitem->getActiveVertex(), 0);
    eitem->clickPoint(insideTLC);
    QCOMPARE(eitem->getActiveVertex(), TOP | LEFT);
    eitem->clickPoint(insideTRC);
    QCOMPARE(eitem->getActiveVertex(), TOP | RIGHT);
    eitem->clickPoint(insideBLC);
    QCOMPARE(eitem->getActiveVertex(), BOTTOM | LEFT);
    eitem->clickPoint(insideBRC);
    QCOMPARE(eitem->getActiveVertex(), BOTTOM | RIGHT);
    delete eitem;
}

void TestSelect::testEllipseBoundingRect(){
    eitem = new EllipseSelect(point, 3.);
    eitem->addPoint(brc);
    QRectF bb = eitem->boundingRect();
    QVERIFY(eitem->isInsideRect(bb, point));
    delete eitem;
}

void TestSelect::testEllipseResizeItem(){
    QRectF myRect = QRectF(point, outsidePoint);
    eitem = new EllipseSelect(point);
    eitem->addPoint(brc);
    eitem->resizeItem(BOTTOM | RIGHT, outsidePoint);
    QCOMPARE(eitem->myRect, myRect);
    delete eitem;
}

void TestSelect::testEllipseMoveItem(){
    eitem = new EllipseSelect(point);
    eitem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    QGraphicsScene *myScene = new QGraphicsScene(sceneRect);
    myScene->addItem((SelectItem *)eitem);
    QRectF myRect = QRectF(point, insidePoint);
    eitem->moveItem(brc, insidePoint);
    QCOMPARE(eitem->myRect, myRect);
    eitem->setActiveVertex(0,0);
    QPointF shift = outsidePoint + point;
    eitem->moveItem(outsidePoint, shift);
    myRect.setTopLeft(myRect.topLeft() + point);
    myRect.setBottomRight(myRect.bottomRight() + point);
    QCOMPARE(eitem->myRect, myRect);
    delete eitem;
    delete myScene;
}

void TestSelect::testEllipseIsInside(){
    eitem = new EllipseSelect(point, 2.);
    eitem->addPoint(brc);
    QVERIFY(eitem->isInside(insidePoint));
    QVERIFY(eitem->isInside(insideTLC));
    QVERIFY(eitem->isInside(outsidePoint) == false);
    QVERIFY(eitem->isInside(insideBRC));
    eitem->setInactive();
    QVERIFY(eitem->isInside(insideBRC) == false);
    delete eitem;
}

void TestSelect::testEllipseAddPoint(){
    eitem = new EllipseSelect(point);
    QRectF myRect = QRectF(point, brc);
    eitem->addPoint(brc);
    QVERIFY(eitem->myRect == myRect);
    delete eitem;
    eitem = new EllipseSelect(point);
    myRect = QRectF(tlc, point);
    eitem->addPoint(tlc);
    QVERIFY(eitem->myRect == myRect);
    delete eitem;
}

