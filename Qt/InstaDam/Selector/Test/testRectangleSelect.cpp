

#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;


void TestSelect::testRectClickPoint(){
    ritem = new RectangleSelect(point, 3.);
    ritem->addPoint(brc);
    ritem->clickPoint(outsidePoint);
    QCOMPARE(ritem->getActiveVertex(), 0);
    ritem->clickPoint(insidePoint);
    QCOMPARE(ritem->getActiveVertex(), 0);
    ritem->clickPoint(insideTLC);
    QCOMPARE(ritem->getActiveVertex(), TOP | LEFT);
    ritem->clickPoint(insideTRC);
    QCOMPARE(ritem->getActiveVertex(), TOP | RIGHT);
    ritem->clickPoint(insideBLC);
    QCOMPARE(ritem->getActiveVertex(), BOTTOM | LEFT);
    ritem->clickPoint(insideBRC);
    QCOMPARE(ritem->getActiveVertex(), BOTTOM | RIGHT);
    delete ritem;
}

void TestSelect::testRectBoundingRect(){
    ritem = new RectangleSelect(point, 3.);
    ritem->addPoint(brc);
    QRectF bb = ritem->boundingRect();
    QVERIFY(ritem->isInsideRect(bb, point));
    delete ritem;
}

void TestSelect::testRectResizeItem(){
    QRectF myRect = QRectF(point, outsidePoint);
    ritem = new RectangleSelect(point);
    ritem->addPoint(brc);
    ritem->resizeItem(BOTTOM | RIGHT, outsidePoint);
    QCOMPARE(ritem->myRect, myRect);
    delete ritem;
}

void TestSelect::testRectMoveItem(){
    ritem = new RectangleSelect(point);
    ritem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    QGraphicsScene *myScene = new QGraphicsScene(sceneRect);
    myScene->addItem((SelectItem *)ritem);
    QRectF myRect = QRectF(point, insidePoint);
    ritem->moveItem(brc, insidePoint);
    QCOMPARE(ritem->myRect, myRect);
    ritem->setActiveVertex(0,0);
    QPointF shift = outsidePoint + point;
    ritem->moveItem(outsidePoint, shift);
    myRect.setTopLeft(myRect.topLeft() + point);
    myRect.setBottomRight(myRect.bottomRight() + point);
    QCOMPARE(ritem->myRect, myRect);
    delete ritem;
    delete myScene;
}

void TestSelect::testCheckBoundaries(){
    ritem = new RectangleSelect(point);
    ritem->addPoint(brc);
    QRectF origRect = ritem->myRect;
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    QGraphicsScene *myScene = new QGraphicsScene(sceneRect);
    myScene->addItem((SelectItem *)ritem);
    QPointF shift = QPointF(5., 10.);
    ritem->checkBoundaries(shift, ritem->myRect);
    origRect.setTopLeft(origRect.topLeft() + shift);
    origRect.setBottomRight(origRect.bottomRight() + shift);
    QCOMPARE(origRect, ritem->myRect);
    shift = QPointF(-40., -40.);
    ritem->checkBoundaries(shift, ritem->myRect);
    origRect.setTopLeft(QPointF(0.,0.));
    origRect.setBottomRight(QPointF(15., 10.));
    QCOMPARE(origRect, ritem->myRect);
    shift = QPointF(60., 60.);
    ritem->checkBoundaries(shift, ritem->myRect);
    origRect.setTopLeft(QPointF(34., 39.));
    origRect.setBottomRight(QPointF(49., 49.));
    QCOMPARE(origRect, ritem->myRect);
    delete ritem;
    delete myScene;
}

void TestSelect::testSortCorners(){
    ritem = new RectangleSelect(point);
    ritem->setActiveVertex(TOP, LEFT);
    ritem->addPoint(tlc);
    QRectF myRect = QRectF(tlc, point);
    QCOMPARE(ritem->myRect, myRect);
    delete ritem;
}
void TestSelect::testRectIsInside(){
    ritem = new RectangleSelect(point);
    ritem->addPoint(brc);
    QVERIFY(ritem->isInside(insidePoint));
    QVERIFY(ritem->isInside(insideTLC));
    QVERIFY(ritem->isInside(outsidePoint) == false);
    delete ritem;
}

void TestSelect::testInit()
{
    ritem = new RectangleSelect(point);
    QCOMPARE(ritem->type(), Rect);
    delete ritem;
    ritem = new RectangleSelect(point, 5.);
    QCOMPARE(ritem->type(), Rect);
    delete ritem;
}

void TestSelect::testActive(){
    ritem = new RectangleSelect(point);
    QCOMPARE(ritem->isItemActive(), true);
    ritem->setInactive();
    QCOMPARE(ritem->isItemActive(), false);
    ritem->setItemActive();
    QCOMPARE(ritem->isItemActive(), true);
    delete ritem;
}

void TestSelect::testActiveVertex(){
    ritem = new RectangleSelect(point);
    ritem->setActiveVertex(TOP, LEFT);
    QCOMPARE((ritem->getActiveVertex() & TOP), TOP);
    ritem->flipH();
    QCOMPARE((ritem->getActiveVertex() & BOTTOM), BOTTOM);
    QCOMPARE((ritem->getActiveVertex() & LEFT), LEFT);
    QVERIFY((ritem->getActiveVertex() & TOP) != TOP);
    ritem->flipV();
    QCOMPARE((ritem->getActiveVertex() & RIGHT), RIGHT);
    delete ritem;
}

void TestSelect::testRectAddPoint(){
    ritem = new RectangleSelect(point);
    QRectF myRect = QRectF(point, brc);
    ritem->addPoint(brc);
    QVERIFY(ritem->myRect == myRect);
    delete ritem;
    ritem = new RectangleSelect(point);
    myRect = QRectF(tlc, point);
    ritem->addPoint(tlc);
    QVERIFY(ritem->myRect == myRect);
    delete ritem;
}

void TestSelect::testGetType(){
    ritem = new RectangleSelect(point);
    QCOMPARE(ritem->type(), Rect);
    delete ritem;
}

void TestSelect::testScene(){
    ritem = new RectangleSelect(point);
    QCOMPARE(ritem->scene(), nullptr);
    ritem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    QGraphicsScene *myScene = new QGraphicsScene(sceneRect);
    myScene->addItem((SelectItem *)ritem);
    QCOMPARE(ritem->scene(), myScene);
    delete ritem;
    delete myScene;
}

void TestSelect::testGetParentItem(){
    ritem = new RectangleSelect(point);
    QCOMPARE(ritem->getParentItem(), nullptr);
    delete ritem;
}

QTEST_MAIN(TestSelect)
#include "moc_testSelect.cpp"
