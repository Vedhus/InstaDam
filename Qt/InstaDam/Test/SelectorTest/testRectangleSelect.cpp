
#include "testSelect.h"
#include <iostream>
using namespace std;

QSize SelectItem::myBounds = QSize(50,50);
int SelectItem::ID = 0;
void TestSelect::testRectClickPoint(){
    ritem = new RectangleSelect(point, 3., myLabel);
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
    ritem = new RectangleSelect(point, 3., myLabel);
    ritem->addPoint(brc);
    QRectF bb = ritem->boundingRect();
    QVERIFY(ritem->isInsideRect(bb, point));
    delete ritem;
}

void TestSelect::testRectResizeItem(){
    QRectF myRect = QRectF(point, outsidePoint);
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    ritem->resizeItem(BOTTOM | RIGHT, outsidePoint);
    QCOMPARE(ritem->myRect, myRect);
    delete ritem;
}

void TestSelect::testRectMoveItem(){
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(ritem);
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
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    QRectF origRect = ritem->myRect;
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(ritem);
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
    ritem = new RectangleSelect(point, myLabel);
    ritem->setActiveVertex(TOP, LEFT);
    ritem->addPoint(tlc);
    QRectF myRect = QRectF(tlc, point);
    QCOMPARE(ritem->myRect, myRect);
    delete ritem;
}
void TestSelect::testRectIsInside(){
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    QVERIFY(ritem->isInside(insidePoint));
    QVERIFY(ritem->isInside(insideTLC));
    QVERIFY(ritem->isInside(outsidePoint) == false);
    delete ritem;
}

void TestSelect::testInit() {
    ritem = new RectangleSelect(point, myLabel);
    QCOMPARE(ritem->type(), SelectItem::Rectangle);
    delete ritem;
    ritem = new RectangleSelect(point, 5., myLabel);
    QCOMPARE(ritem->type(), SelectItem::Rectangle);
    delete ritem;
    ritem = new RectangleSelect();
    QCOMPARE(ritem->type(), SelectItem::Rectangle);
    ritem->baseInstructions();
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
    ritem = new RectangleSelect(point, myLabel);
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
    ritem = new RectangleSelect(point, myLabel);
    QRectF myRect = QRectF(point, brc);
    ritem->addPoint(brc);
    QVERIFY(ritem->myRect == myRect);
    delete ritem;
    ritem = new RectangleSelect(point, myLabel);
    myRect = QRectF(tlc, point);
    ritem->addPoint(tlc);
    QVERIFY(ritem->myRect == myRect);
    delete ritem;
}

void TestSelect::testGetType(){
    ritem = new RectangleSelect(point, myLabel);
    QCOMPARE(ritem->type(), SelectItem::Rectangle);
    delete ritem;
}

void TestSelect::testScene(){
    ritem = new RectangleSelect(point, myLabel);
    QCOMPARE(ritem->scene(), nullptr);
    ritem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(ritem);
    QCOMPARE(ritem->scene(), myScene);
    delete ritem;
    delete myScene;
}

void TestSelect::testGetParentItem(){
    ritem = new RectangleSelect(point, myLabel);
    QCOMPARE(ritem->getParentItem(), nullptr);
    delete ritem;
}

void TestSelect::testRMirror(){
    ritem = new RectangleSelect(point, myLabel);
    RectangleSelect *mitem = new RectangleSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    ritem->addPoint(brc);
    ritem->setMirrorActive();
    QCOMPARE(ritem->myRect, mitem->myRect);
    mitem->moveItem(point, brc);
    QCOMPARE(ritem->myRect, mitem->myRect);
    ritem->mirrorHide();
    QCOMPARE(mitem->isVisible(), false);
    ritem->mirrorShow();
    QCOMPARE(mitem->isVisible(), true);
    ritem->setActiveVertex(SelectItem::NONE);
    ritem->moveItem(point, tlc);
    //QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    myScene->addItem(mitem);
    mitem->itemWasAdded();
    ritem->updateMirrorScene();
    //QCOMPARE(ritem->scene(), myScene);
    ritem->resetActiveVertex();
    QCOMPARE(ritem->getActiveVertex(), 0);
    delete ritem;
    delete mitem;
}

void TestSelect::testRToPixmap(){
    QPixmap pix(QSize(100,100));
    pix.fill(Qt::transparent);
    ritem = new RectangleSelect(point, myLabel);
    QPainter *paint = new QPainter(&pix);
    ritem->toPixmap(paint);
    delete ritem;
    delete paint;
}

void TestSelect::testRRotate(){
    ritem = new RectangleSelect(point, myLabel);
    RectangleSelect *mitem = new RectangleSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    ritem->addPoint(brc);

    ritem->rotate(brc, tlc);
    QCOMPARE(ritem->myRect, mitem->myRect);
    delete ritem;
    delete mitem;

}

void TestSelect::testRReadWrite(){
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    ritem->rotate(brc, tlc);
    QJsonObject json;
    ritem->write(json);
    RectangleSelect *item = new RectangleSelect(json, myLabel);
    QCOMPARE(ritem->myRect, item->myRect);
    QCOMPARE(ritem->myRotation, item->myRotation);
    delete ritem;
    delete item;
}

void TestSelect::testROpacity(){
    ritem = new RectangleSelect(point, myLabel);
    ritem->setOpacity(.5);
    QCOMPARE(ritem->SelectItem::opacity(), 0.5);
    delete ritem;
}

void TestSelect::testSelectInlines(){
    ritem = new RectangleSelect(point, myLabel);
    ritem->addPoint(brc);
    ritem->setActiveVertex(SelectItem::TOP | SelectItem::LEFT);
    ritem->flipH();
    QCOMPARE(ritem->getActiveVertex(), SelectItem::BOTTOM | SelectItem::LEFT);
    ritem->flipV();
    QCOMPARE(ritem->getActiveVertex(), SelectItem::BOTTOM | SelectItem::RIGHT);
    QCOMPARE(ritem->isItemActive(), true);
    QCOMPARE(ritem->getActivePoint(), QPointF(0.,0.));
    delete ritem;
}

void TestSelect::testMaskShowHide(){
    ritem = new RectangleSelect(point, myLabel);
    RectangleSelect *mitem = new RectangleSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    ritem->addPoint(brc);
    ritem->setMirrorActive();

    PhotoScene *mScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    mScene->addItem(mitem);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->addItem(ritem);
    mitem->setOnMaskScene(true);
    ritem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    ritem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    mitem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->setFromMaskScene(true);
    ritem->hideMask();
    QCOMPARE(mitem->isVisible(), true);
    ritem->showMask();
    QCOMPARE(mitem->isVisible(), true);

    delete ritem;
    delete mitem;

}

QTEST_MAIN(TestSelect)
#include "moc_testSelect.cpp"
