#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testFreeMove(){
    QPointF p = QPointF(20,20);
    QPointF e = QPointF(20,100);
    fitem = new FreeDrawSelect(p, 20, Qt::RoundCap, myLabel);
    FreeDrawSelect *mir = new FreeDrawSelect(p, 20, Qt::RoundCap, nullptr);
    fitem->setMirror(mir);
    mir->setMirror(fitem);
    fitem->moveItem(p, e);
    FreeDrawSelect *nfitem = new FreeDrawSelect(fitem->getPixmap(), myLabel);
    //QCOMPARE(fitem->getPixmap(), nfitem->getPixmap());
    QPen pen(Qt::blue);
    FreeDrawSelect *ofitem = new FreeDrawSelect(fitem->getPixmap(), pen);
    //QCOMPARE(fitem->getPixmap(), ofitem->getPixmap());
    QList<FreeDrawSelect *> myList = QList<FreeDrawSelect *>();
    myList.append(fitem);
    myList.append(ofitem);
    myList.append(nfitem);
    FreeDrawSelect *fds = new FreeDrawSelect(myList);
    FreeDrawSelect *ffds = new FreeDrawSelect(p, pen);
    delete ofitem;
    delete fitem;
    delete nfitem;
    delete fds;
    delete ffds;
}

void TestSelect::testEraseMove(){
    myLabel->clear();
    QRectF sceneRect = QRectF(0., 0., 500., 500.);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    SelectItem::myBounds = QSize(500, 500);
    myScene->setSceneRect(sceneRect);
    myLabel->setColor(Qt::blue);
    fitem = new FreeDrawSelect(p1, 3, Qt::RoundCap, myLabel);
    fitem->moveItem(p1, p2);
    fitem->moveItem(p2, p3);
    myLabel->addItem(fitem);
    fitem->moveItem(point, brc);
    myScene->addItem(fitem);

    feitem = new FreeDrawErase(p1, 3, Qt::RoundCap, myLabel);
    myLabel->addItem(feitem);
    feitem->moveItem(point, brc);

    delete fitem;
    delete feitem;
}

void TestSelect::testFreeStubs(){
    fitem = new FreeDrawSelect();
    fitem->addPoint(p1, 0);
    fitem->clickPoint(p1);
    fitem->rotate(p1, p2);
    QCOMPARE(fitem->activeVertex, SelectItem::UNSELECTED);
    QCOMPARE(fitem->isInside(p1), false);
    fitem->resizeItem(0, p1);
    delete fitem;
}

void TestSelect::testFreeReadWrite(){
    fitem = new FreeDrawSelect(p1, 20, Qt::RoundCap, myLabel);
    fitem->moveItem(p1, p2);
    QJsonObject json;
    fitem->write(json);
    FreeDrawSelect *item = new FreeDrawSelect(json, myLabel);
    delete fitem;
    delete item;
}

void TestSelect::testFreeToPixmap(){
    QPixmap pix(QSize(100,100));
    pix.fill(Qt::transparent);
    fitem = new FreeDrawSelect();
    QPainter *paint = new QPainter(&pix);
    fitem->toPixmap(paint);
    delete fitem;
    delete paint;
}

void TestSelect::testFreeShowHide() {
    fitem = new FreeDrawSelect(p1, 20, Qt::RoundCap, myLabel);
    FreeDrawSelect *mitem = new FreeDrawSelect(p1, 20, Qt::RoundCap, myLabel);
    mitem->setLabel(myLabel);
    mitem->updatePen(mitem->myPen);
    fitem->setMirror(mitem);
    mitem->setMirror(fitem);
    fitem->addPoint(brc);
    fitem->setMirrorActive();

    PhotoScene *mScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    mScene->addItem(mitem);
    PhotoScene *myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->addItem(fitem);
    fitem->updateMirrorScene();
    mitem->setOnMaskScene(true);
    fitem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    fitem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->hideMask();
    QCOMPARE(mitem->isVisible(), false);
    mitem->showMask();
    QCOMPARE(mitem->isVisible(), true);
    mitem->setFromMaskScene(true);
    fitem->hideMask();
    QCOMPARE(mitem->isVisible(), true);
    fitem->showMask();
    QCOMPARE(mitem->isVisible(), true);

    delete fitem;
    delete mitem;

}
