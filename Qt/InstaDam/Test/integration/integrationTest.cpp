#include "integrationTest.h"

#include <QGraphicsSceneMouseEvent>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

void IntegrationTest::init() {
    idm = new InstaDam();
    idm->runningLocally = true;
}

void IntegrationTest::clear() {
    struct stat info;
    if (stat( "annotations", &info) == 0) {
        char oldname[] = "annotations";
        std::string newname = "annot_" + std::to_string(count);
        count++;
        rename( oldname , newname.c_str() );
    }
}

void IntegrationTest::setup() {
    clear();
    idm->addLabelHash("Label1", Qt::red);
    idm->addLabelHash("Label2", Qt::blue);
    idm->addLabelHash("Label3", Qt::green);
    idm->on_actionNew_triggered();
    QCOMPARE(idm->currentProject->labels.size(), 3);
}

void IntegrationTest::initTestCase() {
    freePoints.append(free1);
    freePoints.append(free2);
    freePoints.append(free3);
    freePoints.append(free4);
    freePoints.append(free5);
    freePoints.append(free6);
    erasePoints.append(erase1);
    erasePoints.append(erase2);
    erasePoints.append(erase3);
}

void IntegrationTest::cleanup() {
    delete idm;
}

void IntegrationTest::makeMouseDownEvent(QPointF point,
                                         Qt::KeyboardModifier modifier,
                                         Qt::MouseButton button,
                                         PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(point);
    event.setModifiers(modifier);
    event.setWidget(dynamic_cast<QWidget*>(scene));
    event.setButtonDownPos(button, point);
    event.setButtonDownScenePos(button, point);
    scene->mousePressEvent(&event);
}

void IntegrationTest::makeMouseMoveEvent(QPointF from, QPointF to,
                                         Qt::KeyboardModifier modifier,
                                         Qt::MouseButton button,
                                         PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseMove);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(to);
    event.setLastScenePos(from);
    event.setModifiers(modifier);
    event.setWidget(dynamic_cast<QWidget*>(scene));
    event.setButtonDownPos(button, from);
    event.setButtonDownScenePos(button, from);
    scene->mouseMoveEvent(&event);
}

void IntegrationTest::makeMouseReleaseEvent(QPointF from, QPointF to,
                                            Qt::KeyboardModifier modifier,
                                            Qt::MouseButton button,
                                            PhotoScene::viewerTypes type) {
    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseRelease);
    PhotoScene* scene = (type == PhotoScene::PHOTO_VIEWER_TYPE ? idm->scene :
                                                                 idm->maskScene);
    event.setButton(button);
    event.setButtons(button);
    event.setScenePos(to);
    event.setModifiers(modifier);
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

void IntegrationTest::drawObjects() {
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

    // draw another ellipse
    draw(ellipse3, ellipse4);
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4, ellipse3));

    // polygon
    clickLabel("Label3");
    clickPolygonSelect();
    click(poly1);
    click(poly2);
    click(poly3);
    click(poly5);
    clickFinishPolygon();
    QCOMPARE(getItem(SelectItem::Polygon)->numberOfVertices(), 4);

    // FreeDraw
    clickFreeDrawSelect();
    clickDrawButton();
    clickRoundBrush();
    setBrushSize(20);
    freeDraw(freePoints);
    QCOMPARE(getItem(SelectItem::Freedraw)->isVisible(), true);

    QImage orig = dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw))->getPixmap().toImage();

    // FreeErase
    clickEraseButton();
    clickSquareBrush();
    freeDraw(erasePoints);
    QImage erased = dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw))->getPixmap().toImage();
    QVERIFY(orig != erased);

}

void IntegrationTest::testWriteAndReadLocal() {
    setup();
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/testImg1.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    drawObjects();

    int numLab = idm->currentProject->labels.size();

    // save project
    std::string projName = "/myTestProject.idpro";
    idm->setPrjOutName((sPath + projName).c_str());
    idm->on_actionSave_triggered();

    delete idm;
    // load project
    init();

    idm->setPrjInName((sPath + projName).c_str());
    idm->on_actionOpen_triggered();

    QCOMPARE(idm->currentProject->labels.size(), numLab);
}

void IntegrationTest::testWriteAndReadAnnotation() {
    setup();
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/testImg1.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    drawObjects();
    QRectF rectRect = QRectF(rect1, rect2);
    QRectF ell1Rect = QRectF(ellipse1, ellipse2);
    QRectF ell2Rect = QRectF(ellipse4, ellipse3);
    QImage orig = dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw))->getPixmap().toImage();

    int numLab = idm->currentProject->labels.size();

    // save annotations
    idm->on_actionSave_Annotation_triggered();
    delete idm;

    // load project
    init();

    QCOMPARE(idm->currentProject->labels.size(), 0);

    // load annotations
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();
    QCOMPARE(idm->currentProject->labels.size(), numLab);
    QCOMPARE(getItem(SelectItem::Rectangle)->myRect, rectRect);
    QVERIFY(getItem(SelectItem::Ellipse)->myRect == ell1Rect ||
            getItem(SelectItem::Ellipse, 2)->myRect == ell1Rect);
    QVERIFY(getItem(SelectItem::Ellipse, 2)->myRect == ell2Rect ||
            getItem(SelectItem::Ellipse)->myRect == ell2Rect);
    QCOMPARE(getItem(SelectItem::Polygon)->numberOfVertices(), 4);
    QCOMPARE(dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw))->getPixmap().toImage(), orig);

}
void IntegrationTest::testAnnotateModify() {
    setup();
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/testImg1.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    drawObjects();
    // resize ellipse1
    clickEllipseSelect();
    //draw(ellipseCorner, ellipseCorner);
    draw((ellipse1 + ellipse2)/2., (ellipse1 + ellipse2)/2., Qt::ShiftModifier);
    draw(ellipseCorner, ellipseResize, Qt::ShiftModifier);
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2 + eShift));

    // undo the resize
    idm->undoGroup->undo();
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2));

    // redo the resize
    idm->undoGroup->redo();
    QCOMPARE(getItem(SelectItem::Ellipse)->myRect, QRectF(ellipse1, ellipse2 + eShift));

    // move ellipse2
    draw(inEllipse, ellipseMove, Qt::ShiftModifier);
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4 + ellipseShift, ellipse3 + ellipseShift));

    // undo move
    idm->undoGroup->undo();
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4, ellipse3));

    // redo move
    idm->undoGroup->redo();
    QCOMPARE(getItem(SelectItem::Ellipse, 2)->myRect, QRectF(ellipse4 + ellipseShift, ellipse3 + ellipseShift));

    // rotate the rectangle
    draw(rectRotate1, rectRotate2, Qt::ShiftModifier, Qt::RightButton);
    qreal angle = getItem(SelectItem::Rectangle)->rotation();
    QVERIFY(angle != 0.);

    // undo rotate
    idm->undoGroup->undo();
    QCOMPARE(getItem(SelectItem::Rectangle)->rotation(), 0.);

    // redo rotate
    idm->undoGroup->redo();
    QCOMPARE(getItem(SelectItem::Rectangle)->rotation(), angle);

    // move polygon
    draw(inPoly, movePoly, Qt::ShiftModifier);
    //clickFinishPolygon();
    QVector<QPointF> points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points[0], poly1 + moveShift);

    // undo move
    idm->undoGroup->undo();
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[1], poly2);
    QCOMPARE(points[2], poly3);
    QCOMPARE(points[3], poly5);

    move(free1, free2, Qt::ShiftModifier, Qt::NoButton);
    // add vertex
    click(inPoly, Qt::ShiftModifier);
    clickInsertPoint();
    click(poly3, Qt::ShiftModifier);
    click(poly5, Qt::ShiftModifier);
    click(poly4);
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 5);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[1], poly2);
    QCOMPARE(points[2], poly3);
    QCOMPARE(points[3], poly4);
    QCOMPARE(points[4], poly5);

    // undo add
    idm->undoGroup->undo();
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 4);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[1], poly2);
    QCOMPARE(points[2], poly3);
    QCOMPARE(points[3], poly5);

    // redo add
    idm->undoGroup->redo();
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 5);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[3], poly4);

    // move vertex
    draw(polyVertex, moveVertex, Qt::ShiftModifier);
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 5);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[2], poly3 + vertexShift);

    // undo move
    idm->undoGroup->undo();
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 5);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[2], poly3);

    // redo move
    idm->undoGroup->redo();
    points = dynamic_cast<PolygonSelect*>(getItem(SelectItem::Polygon))->myPoints;
    QCOMPARE(points.size(), 5);
    QCOMPARE(points[0], poly1);
    QCOMPARE(points[2], poly3 + vertexShift);

    // FreeDraw
    clickFreeDrawSelect();
    clickDrawButton();
    clickRoundBrush();
    setBrushSize(25);
    freeDraw(freePoints);
    QCOMPARE(getItem(SelectItem::Freedraw, 2)->myPen.width(), 25);
    QCOMPARE(getItem(SelectItem::Freedraw, 2)->isVisible(), true);

    // undo
    idm->undoGroup->undo();
    QCOMPARE(getItem(SelectItem::Freedraw, 2)->isVisible(), false);

    // redo
    idm->undoGroup->redo();
    QCOMPARE(getItem(SelectItem::Freedraw, 2)->isVisible(), true);

    QImage orig = dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw,
                                                        2))->getPixmap().toImage();

    // FreeErase
    clickEraseButton();
    clickSquareBrush();
    freeDraw(erasePoints);
    QImage erased = dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw,
                                                          2))->getPixmap().toImage();
    QVERIFY(orig != erased);

    // undo
    idm->undoGroup->undo();
    QCOMPARE(orig, dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw,
                                                         2))->getPixmap().toImage());

    // redo
    idm->undoGroup->redo();
    QCOMPARE(erased, dynamic_cast<FreeDrawSelect*>(getItem(SelectItem::Freedraw,
                                                           2))->getPixmap().toImage());
}

void IntegrationTest::testClickButtons() {
    clickEllipseSelect();
    clickRectangleSelect();
    clickEllipseSelect();
    clickFreeDrawSelect();
    clickEllipseSelect();
    clickFreeDrawSelect();
    clickEllipseSelect();
    clickPolygonSelect();
    clickEllipseSelect();
    clickPolygonSelect();
    clickRectangleSelect();
    clickPolygonSelect();
    clickFreeDrawSelect();
    clickPolygonSelect();
    clickFreeDrawSelect();
    clickRectangleSelect();
    clickFreeDrawSelect();
}

void IntegrationTest::testSaveAndNext() {
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/projectTest/horsehead.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    QCOMPARE(idm->currentProject->numLabels(), 4);

    clickLabel("Test2");
    clickRectangleSelect();
    draw(rect1, rect2);
    QCOMPARE(idm->currentLabel->rectangleObjects.size(), 2);
    idm->currentLabel->setOpacity(15);
    QCOMPARE(getItem(SelectItem::Rectangle)->opacity(), .15);
    QCOMPARE(getItem(SelectItem::Rectangle, 2)->opacity(), .15);

    clickSaveAndNext();
    QCOMPARE(idm->currentProject->numLabels(), 4);
    for (int i = 0; i < idm->currentProject->numLabels(); i++) {
        clickLabel(idm->currentProject->getLabel(i)->getText());
        QCOMPARE(idm->currentLabel->rectangleObjects.size(), 0);
        QCOMPARE(idm->currentLabel->ellipseObjects.size(), 0);
        QCOMPARE(idm->currentLabel->polygonObjects.size(), 0);
        QCOMPARE(idm->currentLabel->freeDrawObjects.size(), 0);
    }

    clickEllipseSelect();
    draw(ellipse1, ellipse2);
    QCOMPARE(idm->currentLabel->ellipseObjects.size(), 1);

    clickSaveAndBack();
    QCOMPARE(idm->currentProject->numLabels(), 4);
    clickLabel("Test2");
    QCOMPARE(idm->currentLabel->rectangleObjects.size(), 2);
    QCOMPARE(idm->currentLabel->rectangleObjects.size(), 2);
}

void IntegrationTest::testExport() {
    setup();
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/testImg1.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    drawObjects();
    idm->exportImages(false);
}

void IntegrationTest::testExportZip() {
    setup();
    char currentPath[FILENAME_MAX];
    getcwd( currentPath, FILENAME_MAX );
    std::string sPath(currentPath);
    std::string fn = "/testImg1.jpg";
    idm->setImgInName((sPath + fn).c_str());
    idm->on_actionOpen_File_triggered();

    drawObjects();
    idm->on_actionExport_zip_triggered();
}

QTEST_MAIN(IntegrationTest)
#include "moc_integrationTest.cpp"
