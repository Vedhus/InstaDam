#include "testSelect.h"

#include <QGraphicsSceneMouseEvent>

void TestSelect::testPhotoRemoveItem() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setText("Label");
    myLabel->setColor(Qt::blue);
    myScene->addLabel(myLabel->getText().toStdString());
    ritem = new RectangleSelect(p1, myLabel);
    myScene->addItem(ritem);
    myScene->inactiveAll();
    QCOMPARE(myScene->currentItems.size(), 1);
    myScene->removeItem(ritem);
    QCOMPARE(myScene->currentItems.size(), 0);
    delete ritem;
    delete myScene;
}

void TestSelect::testPhotoClearItems() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    myScene->addLabel(myLabel->getText().toStdString());
    ritem = new RectangleSelect(p1, myLabel);
    myScene->addItem(ritem);
    myScene->clearItems();
    QCOMPARE(myScene->currentItems.size(), 0);
    QCOMPARE(myScene->labelmap.size(), 0);
    delete ritem;

    delete myScene;
}

void TestSelect::testPhotoKeyPress() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    myScene->keyPressEvent(event);
    delete event;
    delete myScene;
}

void TestSelect::testPhotoMousePress() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    QPointF qp = QPointF(15., 15.);
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    event.setScenePos(qp);
    event.setButtonDownScenePos(Qt::LeftButton, qp);
    myScene->mousePressEvent(&event);

    ritem = new RectangleSelect(p1, myLabel);
    myScene->addItem(ritem);
    ritem->addPoint(outsidePoint);

    myScene->mousePressEvent(&event);
    delete ritem;
    delete myScene;
}

void TestSelect::testPhotoMouseMove() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    QPointF qp = QPointF(15., 15.);
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    event.setScenePos(qp);
    event.setButtonDownScenePos(Qt::LeftButton, qp);
    myScene->mouseMoveEvent(&event);
    myScene->mousePressed = true;
    myScene->mouseMoveEvent(&event);
    delete myScene;

}

void TestSelect::testPhotoMouseRelease() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    QPointF qp = QPointF(15., 15.);
    QGraphicsSceneMouseEvent event;
    event.setButton(Qt::LeftButton);
    event.setScenePos(qp);
    event.setButtonDownScenePos(Qt::LeftButton, qp);
    myScene->mouseReleaseEvent(&event);
    delete myScene;

}

void TestSelect::testPhotoItemAt() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);
    myLabel->setText("Label");
    myScene->addLabel("Label");
    myScene->setCurrentLabel(myLabel->getText().toStdString());
    QPointF qp = QPointF(15., 15.);
    QCOMPARE(myScene->itemAt(qp, myLabel->getText().toStdString()), nullptr);
    ritem = new RectangleSelect(p1, myLabel);
    myScene->addLabelItem(ritem, "Label");
    ritem->addPoint(outsidePoint);
    QCOMPARE(myScene->itemAt(qp, myLabel->getText().toStdString()), ritem);

    delete ritem;
    delete myScene;
}

void TestSelect::testPhotoAddLabel() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);

    delete myScene;

}

void TestSelect::testPhotoAddLabelItems() {
    QRectF sceneRect = QRectF(0., 0., 50., 50.);
    myScene = new PhotoScene(PhotoScene::PHOTO_VIEWER_TYPE);
    myScene->setSceneRect(sceneRect);
    myLabel = QSharedPointer<Label>::create();
    myLabel->setColor(Qt::blue);

    delete myScene;

}
