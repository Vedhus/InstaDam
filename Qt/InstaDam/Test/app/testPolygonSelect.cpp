#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testPOpacity() {
    pitem = new PolygonSelect();
    pitem->setOpacity(0.5);
    QCOMPARE(pitem->SelectItem::opacity(), 0.5);
    delete pitem;
}
void TestSelect::testPAddPoint() {
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    QCOMPARE(pitem->numberOfVertices(), 3);
    delete pitem;
}
void TestSelect::testPMove() {
    pitem = new PolygonSelect(p1, myLabel);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(pitem);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->moveItem(p1, pmove);
    QCOMPARE(pitem->myPoints[0], pmove);
    QCOMPARE(pitem->myPoints[1], p2 + pshift);
    pitem->setActiveVertex(0);
    pitem->moveItem(pnew, pnew);
    QCOMPARE(pitem->myPoints[0], p1 + pshift);
    QCOMPARE(pitem->myPoints[1], p2 + pshift);
    pitem->addPoint(p1, 0);
    QCOMPARE(pitem->myPoints[0], p1);
    QCOMPARE(pitem->myPoints[2], p2 + pshift);
    pitem->addPoint(p5);
    QCOMPARE(pitem->myPoints[0], p5);
    QCOMPARE(pitem->myPoints[2], p2 + pshift);
    delete myScene;
    //delete pitem;
}

void TestSelect::testPisInside(){
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    QCOMPARE(pitem->isInside(pout), false);
    QCOMPARE(pitem->isInside(pin), true);
    QCOMPARE(pitem->isInside(pjin), true);

    delete pitem;
}

void TestSelect::testPClickPoint() {
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    pitem->clickPoint(p5);
    QCOMPARE(pitem->getActiveVertex(), SelectItem::UNSELECTED);
    pitem->clickPoint(p1);
    QCOMPARE(pitem->getActiveVertex(), 0);
    delete pitem;
}
void TestSelect::testPInsertVertex() {
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    pitem->insertVertex(1, p4);
    QCOMPARE(pitem->myPoints[2], p4);
    delete pitem;
}

void TestSelect::testPReadWrite() {
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    QJsonObject json;
    pitem->write(json);
    PolygonSelect *item = new PolygonSelect(json, myLabel);
    QCOMPARE(pitem->myPoints[0], item->myPoints[0]);
    delete pitem;
    delete item;

}
void TestSelect::testPRemoveVertex() {
    pitem = new PolygonSelect(p1, myLabel);
    pitem->resetActiveVertex();
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    QCOMPARE(pitem->numberOfVertices(), 3);
    pitem->removeVertex(1);
    QCOMPARE(pitem->numberOfVertices(), 2);
    QCOMPARE(pitem->myPoints[1], p3);
    pitem->removeVertex(SelectItem::UNSELECTED);
    QCOMPARE(pitem->numberOfVertices(), 1);
    delete pitem;
}
void TestSelect::testPResize() {

}
void TestSelect::testPMirror() {
    pitem = new PolygonSelect(point, myLabel);
    PolygonSelect *mitem = new PolygonSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    pitem->setMirror(mitem);
    mitem->setMirror(pitem);
    pitem->addPoint(brc);
    pitem->setMirrorActive();
    pitem->resetActiveVertex();
    pitem->addPoint(p2);
    QCOMPARE(pitem->numberOfVertices(), mitem->numberOfVertices());
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->moveItem(p1, pmove);
    QCOMPARE(pitem->numberOfVertices(), mitem->numberOfVertices());
    QCOMPARE(mitem->myPoints[0], pitem->myPoints[0]);
    QCOMPARE(mitem->myPoints[1], pitem->myPoints[1]);
    pitem->setActiveVertex(0);
    pitem->moveItem(pnew, pnew);
    QCOMPARE(mitem->myPoints[0], pitem->myPoints[0]);
    QCOMPARE(mitem->myPoints[1], pitem->myPoints[1]);
    pitem->addPoint(p1, 0);
    QCOMPARE(mitem->myPoints[0], pitem->myPoints[0]);
    QCOMPARE(mitem->myPoints[2], pitem->myPoints[2]);
    pitem->addPoint(p5);
    QCOMPARE(mitem->myPoints[0], pitem->myPoints[0]);
    QCOMPARE(mitem->myPoints[2], pitem->myPoints[2]);


    delete pitem;
    delete mitem;
}

void TestSelect::testPShowHide() {
    pitem = new PolygonSelect(point, myLabel);
    PolygonSelect *mitem = new PolygonSelect(point);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    pitem->setMirror(mitem);
    mitem->setMirror(pitem);
    pitem->addPoint(brc);
    pitem->setMirrorActive();

    maskScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    maskScene->addItem(mitem);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->addItem(pitem);
    pitem->updateMirrorScene();
    mitem->setOnMaskScene(true);
    pitem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    pitem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    mitem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->setFromMaskScene(true);
    pitem->hideMask();
    QCOMPARE(mitem->isVisible(), true);
    pitem->showMask();
    QCOMPARE(mitem->isVisible(), true);

    //delete pitem;
    //delete mitem;
    delete myScene;
    delete maskScene;
}

void TestSelect::testPScene() {
    pitem = new PolygonSelect(point, myLabel);
    QCOMPARE(pitem->scene(), nullptr);
    pitem->addPoint(brc);
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myScene->addItem(pitem);
    QCOMPARE(pitem->scene(), myScene);
    //delete pitem;
    delete myScene;

}

void TestSelect::testPToPixmap(){
    QPixmap pix(QSize(100,100));
    pix.fill(Qt::transparent);
    pitem = new PolygonSelect(point, myLabel);
    QPainter *paint = new QPainter(&pix);
    pitem->toPixmap(paint);
    delete pitem;
    delete paint;
}
