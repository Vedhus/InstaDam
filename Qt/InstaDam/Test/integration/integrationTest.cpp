#include "integrationTest.h"

#include <QGraphicsSceneMouseEvent>

void IntegrationTest::initTestCase() {
    idm = new InstaDam();
    idm->runningLocally = true;
    idm->addLabelHash("Label1", Qt::red);
    idm->addLabelHash("Label2", Qt::blue);
    idm->addLabelHash("Label3", Qt::green);
    idm->on_actionNew_triggered();
    QCOMPARE(idm->currentProject->labels.size(), 3);
    idm->on_actionOpen_File_triggered();

}

void IntegrationTest::cleanupTestCase() {
    delete idm;
}

void IntegrationTest::makeMouseDownEvent(QPointF point, Qt::MouseButton button,
                                         PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(point);
    event.setModifiers(Qt::NoModifier);
    event.setWidget(dynamic_cast<QWidget*>(scene));
    event.setButtonDownPos(button, point);
    event.setButtonDownScenePos(button, point);
    scene->mousePressEvent(&event);
}

void IntegrationTest::makeMouseMoveEvent(QPointF from, QPointF to,
                                         Qt::MouseButton button,
                                         PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseMove);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(to);
    event.setLastScenePos(from);
    event.setModifiers(Qt::NoModifier);
    event.setWidget(dynamic_cast<QWidget*>(scene));
    event.setButtonDownPos(button, from);
    event.setButtonDownScenePos(button, from);
    scene->mouseMoveEvent(&event);
}

void IntegrationTest::makeMouseReleaseEvent(QPointF from, QPointF to,
                                            Qt::MouseButton button,
                                            PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseRelease);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(to);
    event.setModifiers(Qt::NoModifier);
    event.setWidget(dynamic_cast<QWidget*>(scene));
    event.setButtonDownPos(button, from);
    event.setButtonDownScenePos(button, from);
    scene->mouseReleaseEvent(&event);
}

void IntegrationTest::makeKeyPressEvent(Qt::Key key) {
    QKeyEvent event = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
    idm->scene->keyPressEvent(&event);
}

int IntegrationTest::findLabelIndex(const QString name) const {
    for (int i = 0; i < idm->labelButtons.size(); i++) {
        if (idm->labelButtons[i]->text() == name) {
            return i;
        }
    }
    return -1;
}

SelectItem* IntegrationTest::getItem(int type, ushort num) {
    //size_t start = 0;//lastIdx + 1;
    ushort count = 0;
    for (std::list<SelectItem*>::reverse_iterator it = idm->scene->currentItems.rbegin();
         it != idm->scene->currentItems.rend(); ++it) {
        if ((*it)->type() == type){
            count++;
            if (count == num)
                return (*it);
        }
    }
    return nullptr;
}

void IntegrationTest::testWriteAndReadLocal() {
    idm->setImgInName("testImg1.jpg");
    idm->on_actionOpen_File_triggered();

    // draw a rectangle
    clickLabel("Label2");
    clickRectangleSelect();
    draw(rect1, rect2);
    QCOMPARE(getItem(SelectItem::Rectangle)->myRect, QRectF(rect1, rect2));

    // draw an ellipse
    clickLabel("Label1");
    clickEllipseSelect();
    draw(ellipse1, ellipse2);
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2));

    // resize the ellipse
    draw(ellipseCorner, ellipseResize);
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2 + eShift));

    // undo the resize
    idm->undoGroup->undo();
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2));

    // redo the resize
    idm->undoGroup->redo();
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2 + eShift));

    // draw another ellipse
    draw(ellipse3, ellipse4);
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4, ellipse3));

    // move the ellipse
    draw(inEllipse, ellipseMove);
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4 + ellipseShift, ellipse3 + ellipseShift));

}

void IntegrationTest::testAnnotateMove() {

}

void IntegrationTest::testOpenProjectAndAnnotate() {

}

void IntegrationTest::testSaveAndNext() {

}

void IntegrationTest::testExport() {

}

void IntegrationTest::testAsserts() {

}

QTEST_MAIN(IntegrationTest)
#include "moc_integrationTest.cpp"
