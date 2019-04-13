#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::generateData() {
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    myLabel->setText("Label");
    ritem = new RectangleSelect(p1, myLabel);
    ritem->addPoint(p2);
    eitem = new EllipseSelect(p3, myLabel);
    eitem->addPoint(p4);
    pitem = new PolygonSelect(p1, myLabel);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    fitem = new FreeDrawSelect(p1, 4, Qt::RoundCap, myLabel);
    fitem->moveItem(p3, p5);
}

void TestSelect::testLabelSetId() {
    myLabel = QSharedPointer<Label>::create();
    myLabel->setId(5);
    QCOMPARE(myLabel->labelId, 5);
}

void TestSelect::testLabelReadWrite() {
    generateData();
    QJsonObject json;
    myLabel->writeIdantn(json);
    //add opacity test
}

void TestSelect::testLabelText() {
    myLabel = QSharedPointer<Label>::create();
    QString text = "hello";
    myLabel->setText(text);
    QCOMPARE(myLabel->getText(), text);
}

void TestSelect::testLabelAddRemoveItem() {
    generateData();
    QCOMPARE(myLabel->rectangleObjects.size(), 1);
    QCOMPARE(myLabel->ellipseObjects.size(), 1);
    QCOMPARE(myLabel->polygonObjects.size(), 1);
    QCOMPARE(myLabel->freeDrawObjects.size(), 1);
    myLabel->removeItem(ritem->myID);
    QCOMPARE(myLabel->rectangleObjects.size(), 0);
    myLabel->removeItem(eitem->myID);
    QCOMPARE(myLabel->ellipseObjects.size(), 0);
    myLabel->removeItem(pitem->myID);
    QCOMPARE(myLabel->polygonObjects.size(), 0);
    myLabel->removeItem(fitem->myID);
    QCOMPARE(myLabel->freeDrawObjects.size(), 0);
    delete ritem;
    delete eitem;
    delete pitem;
    delete fitem;
}

void TestSelect::testLabelSetOpacity() {
    generateData();
    myLabel->setOpacity(2);
    QCOMPARE(ritem->SelectItem::opacity(), .02);
    QCOMPARE(eitem->SelectItem::opacity(), .02);
    QCOMPARE(pitem->SelectItem::opacity(), .02);
    QCOMPARE(fitem->SelectItem::opacity(), .02);
    delete ritem;
    delete eitem;
    delete pitem;
    delete fitem;
}

void TestSelect::testLabelExportLabel() {
    generateData();
    myLabel->exportLabel(QSize(500,500));
    delete ritem;
    delete eitem;
    delete pitem;
    delete fitem;
}

void TestSelect::testLabelSetMaskState() {
    generateData();
    pitem->setOnMaskScene(true);
    myLabel->setMaskState(Qt::Unchecked);
    QCOMPARE(pitem->isVisible(), false);
    myLabel->setMaskState(Qt::Checked);

    delete ritem;
    delete eitem;
    delete pitem;
    delete fitem;
}
