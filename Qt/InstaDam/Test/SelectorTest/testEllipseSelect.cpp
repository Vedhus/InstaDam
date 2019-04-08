#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testEllipseClickPoint(){
    eitem = new EllipseSelect(point, 3., myLabel);
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
    eitem = new EllipseSelect(point, 3., myLabel);
    eitem->addPoint(brc);
    QRectF bb = eitem->boundingRect();
    QVERIFY(eitem->isInsideRect(bb, point));
    delete eitem;
}

void TestSelect::testEllipseResizeItem(){
    QRectF myRect = QRectF(point, outsidePoint);
    eitem = new EllipseSelect(point, myLabel);
    eitem->addPoint(brc);
    eitem->resizeItem(BOTTOM | RIGHT, outsidePoint);
    QCOMPARE(eitem->myRect, myRect);
    delete eitem;
}

void TestSelect::testEllipseMoveItem(){
    eitem = new EllipseSelect(point, myLabel);
    eitem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(eitem);
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
    eitem = new EllipseSelect(point, 2., myLabel);
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
    eitem = new EllipseSelect(point, myLabel);
    QRectF myRect = QRectF(point, brc);
    eitem->addPoint(brc);
    QVERIFY(eitem->myRect == myRect);
    delete eitem;
    eitem = new EllipseSelect(point, myLabel);
    myRect = QRectF(tlc, point);
    eitem->addPoint(tlc);
    QVERIFY(eitem->myRect == myRect);
    delete eitem;
}

void TestSelect::testEMirror(){
    eitem = new EllipseSelect(point, myLabel);
    EllipseSelect *mitem = new EllipseSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    eitem->setMirror(mitem);
    mitem->setMirror(eitem);
    eitem->addPoint(brc);
    eitem->setMirrorActive();
    QCOMPARE(eitem->myRect, mitem->myRect);
    mitem->moveItem(point, brc);
    QCOMPARE(eitem->myRect, mitem->myRect);
    eitem->mirrorHide();
    QCOMPARE(mitem->isVisible(), false);
    eitem->mirrorShow();
    QCOMPARE(mitem->isVisible(), true);
    eitem->setActiveVertex(SelectItem::NONE);
    eitem->moveItem(point, tlc);
    //QRectF sceneRect = QRectF(0., 0., 50., 50.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    myScene->addItem(mitem);
    mitem->itemWasAdded();
    eitem->updateMirrorScene();
    //QCOMPARE(ritem->scene(), myScene);
    eitem->resetActiveVertex();
    QCOMPARE(eitem->getActiveVertex(), 0);
    delete eitem;
    delete mitem;
}

void TestSelect::testEToPixmap(){
    QPixmap pix(QSize(100,100));
    pix.fill(Qt::transparent);
    eitem = new EllipseSelect(point, myLabel);
    QPainter *paint = new QPainter(&pix);
    eitem->toPixmap(paint);
    delete eitem;
    delete paint;
}

void TestSelect::testERotate(){
    eitem = new EllipseSelect(point, myLabel);
    EllipseSelect *mitem = new EllipseSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    eitem->setMirror(mitem);
    mitem->setMirror(eitem);
    eitem->addPoint(brc);

    eitem->rotate(brc, tlc);
    QCOMPARE(eitem->myRect, mitem->myRect);
    delete eitem;
    delete mitem;

}

void TestSelect::testEReadWrite(){
    eitem = new EllipseSelect(point, myLabel);
    eitem->addPoint(brc);
    eitem->rotate(brc, tlc);
    QJsonObject json;
    eitem->write(json);
    EllipseSelect *item = new EllipseSelect(json, myLabel);
    QCOMPARE(eitem->myRect, item->myRect);
    QCOMPARE(eitem->myRotation, item->myRotation);
    delete eitem;
    delete item;
}

void TestSelect::testEOpacity(){
    eitem = new EllipseSelect();
    eitem->setOpacity(.5);
    QCOMPARE(eitem->SelectItem::opacity(), 0.5);
    delete eitem;
}

