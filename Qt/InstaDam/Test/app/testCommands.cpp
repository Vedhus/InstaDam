#include "testSelect.h"

#include <iostream>

using namespace std;
void TestSelect::setupUndo() {
    myUndo = new QUndoStack();
    myLabel->clear();
    myLabel->setColor(Qt::red);
    myLabel->setText("UndoLabel");
    QRectF sceneRect = QRectF(0., 0., 500., 500.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    maskScene = new PhotoScene(PhotoScene::MASK_VIEWER_TYPE);
    maskScene->setSceneRect(sceneRect);
}

void TestSelect::testCommandAdd() {
    setupUndo();
    ritem = new RectangleSelect(p1, myLabel);
    RectangleSelect *mitem = new RectangleSelect(p1);
    mitem->setLabel(myLabel);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    myScene->addItem(ritem);
    maskScene->addItem(mitem);
    ritem->addPoint(p3);
    QUndoCommand *command = new AddCommand(ritem, myScene);
    myUndo->push(command);
    QCOMPARE(myScene->currentItems.size(), 1);
    QCOMPARE(ritem->isVisible(), true);
    myUndo->undo();
    QCOMPARE(ritem->isVisible(), false);
    myUndo->redo();
    QCOMPARE(mitem->isVisible(), true);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandDelete() {
    setupUndo();
    ritem = new RectangleSelect(p1, myLabel);
    RectangleSelect *mitem = new RectangleSelect(p1);
    mitem->setLabel(myLabel);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    myScene->addItem(ritem);
    maskScene->addItem(mitem);
    ritem->addPoint(p3);
    QUndoCommand *command = new DeleteCommand(ritem, myScene);
    myUndo->push(command);
    QCOMPARE(myScene->currentItems.size(), 1);
    QCOMPARE(ritem->isVisible(), false);
    myUndo->undo();
    QCOMPARE(ritem->isVisible(), true);
    myUndo->redo();
    QCOMPARE(mitem->isVisible(), false);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandMove() {
    setupUndo();
    ritem = new RectangleSelect(p1, myLabel);
    RectangleSelect *mitem = new RectangleSelect(p1);
    mitem->setLabel(myLabel);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    myScene->addItem(ritem);
    maskScene->addItem(mitem);
    ritem->addPoint(p3);
    QRectF initRect = ritem->myRect;
    ritem->setActiveVertex(0);
    ritem->moveItem(p2, p3);
    QRectF finalRect = ritem->myRect;
    QUndoCommand *command = new MoveCommand(ritem, p2, p3);
    myUndo->push(command);
    QCOMPARE(myScene->currentItems.size(), 1);
    QCOMPARE(ritem->myRect, finalRect);
    myUndo->undo();
    QCOMPARE(ritem->myRect, initRect);
    myUndo->redo();
    QCOMPARE(mitem->myRect, finalRect);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandMoveVertex() {
    setupUndo();
    ritem = new RectangleSelect(p1, myLabel);
    RectangleSelect *mitem = new RectangleSelect(p1);
    mitem->setLabel(myLabel);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    myScene->addItem(ritem);
    maskScene->addItem(mitem);
    ritem->addPoint(p3);
    QRectF initRect = ritem->myRect;
    ritem->setActiveVertex(SelectItem::TOP | SelectItem::LEFT);
    ritem->moveItem(p1, p2);
    QRectF finalRect = ritem->myRect;
    QUndoCommand *command = new MoveVertexCommand(ritem, p1, p2, SelectItem::TOP | SelectItem::LEFT);
    myUndo->push(command);
    QCOMPARE(myScene->currentItems.size(), 1);
    QCOMPARE(ritem->myRect, finalRect);
    myUndo->undo();
    QCOMPARE(ritem->myRect, initRect);
    myUndo->redo();
    QCOMPARE(mitem->myRect, finalRect);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandAddVertex() {
    setupUndo();
    pitem = new PolygonSelect(p1, myLabel);
    PolygonSelect *mitem = new PolygonSelect(p1);
    mitem->setLabel(myLabel);
    pitem->setMirror(mitem);
    mitem->setMirror(pitem);
    myScene->addItem(pitem);
    maskScene->addItem(mitem);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    QUndoCommand *command = new AddVertexCommand(pitem, p3);
    myUndo->push(command);
    QCOMPARE(pitem->myPoints.size(), 3);
    myUndo->undo();
    QCOMPARE(pitem->myPoints.size(), 2);
    myUndo->redo();
    QCOMPARE(mitem->myPoints.size(), 3);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandDeleteVertex() {
    setupUndo();
    pitem = new PolygonSelect(p1, myLabel);
    PolygonSelect *mitem = new PolygonSelect(p1);
    mitem->setLabel(myLabel);
    pitem->setMirror(mitem);
    mitem->setMirror(pitem);
    myScene->addItem(pitem);
    maskScene->addItem(mitem);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p2);
    pitem->setActiveVertex(SelectItem::UNSELECTED);
    pitem->addPoint(p3);
    pitem->setActiveVertex(1);
    pitem->activePoint = pitem->myPoints[1];
    QUndoCommand *command = new DeleteVertexCommand(pitem);
    myUndo->push(command);
    QCOMPARE(pitem->myPoints.size(), 2);
    QCOMPARE(pitem->myPoints[0], p1);
    QCOMPARE(pitem->myPoints[1], p3);
    myUndo->undo();
    QCOMPARE(pitem->myPoints.size(), 3);
    QCOMPARE(pitem->myPoints[0], p1);
    QCOMPARE(pitem->myPoints[1], p2);
    QCOMPARE(pitem->myPoints[2], p3);
    myUndo->redo();
    QCOMPARE(mitem->myPoints.size(), 2);
    QCOMPARE(pitem->myPoints[0], p1);
    QCOMPARE(pitem->myPoints[1], p3);

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandErasePoints() {
    setupUndo();
    SelectItem::myBounds = QSize(500, 500);
    fitem = new FreeDrawSelect(p1, 3, Qt::RoundCap, myLabel);
    fitem->moveItem(p1, p2);
    fitem->moveItem(p2, p3);
    myLabel->addItem(fitem);
    fitem->moveItem(point, brc);
    myScene->addItem(fitem);

    feitem = new FreeDrawErase(p1, 3, Qt::RoundCap, myLabel);
    myLabel->addItem(feitem);
    feitem->moveItem(point, brc);
    QUndoCommand *command = new ErasePointsCommand(feitem, myScene, maskScene);
    myUndo->push(command);
    myUndo->undo();
    myUndo->redo();

    delete myUndo;
    delete myScene;
    delete maskScene;
}

void TestSelect::testCommandRotate() {
    setupUndo();
    ritem = new RectangleSelect(p1, myLabel);
    RectangleSelect *mitem = new RectangleSelect(p1);
    mitem->setLabel(myLabel);
    ritem->setMirror(mitem);
    mitem->setMirror(ritem);
    myScene->addItem(ritem);
    maskScene->addItem(mitem);
    ritem->addPoint(p3);
    qreal initAngle = ritem->myRotation;
    ritem->rotate(p1, p3);
    qreal finalAngle = ritem->myRotation;
    QUndoCommand *command = new RotateCommand(ritem, p1, p3);
    myUndo->push(command);
    QCOMPARE(myScene->currentItems.size(), 1);
    QCOMPARE(ritem->myRotation, finalAngle);
    myUndo->undo();
    QCOMPARE(ritem->myRotation, initAngle);
    myUndo->redo();
    QCOMPARE(mitem->myRotation, finalAngle);

    delete myUndo;
    delete myScene;
    delete maskScene;
}
