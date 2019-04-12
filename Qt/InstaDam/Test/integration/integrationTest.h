#ifndef INTEGRATIONTEST_H
#define INTEGRATIONTEST_H
#include <QtTest/QtTest>
#include <QVector>
#include "../appTest/instadam.h"
#include "../appTest/ui_instadam.h"

class IntegrationTest: public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();
    void init();
    void testWriteAndReadLocal();
    void testWriteAndReadAnnotation();
    void testAnnotateModify();

    void testSaveAndNext();
    void testExport();

private:
    InstaDam *idm;
    int count = 0;
    QPointF rect1 = QPointF(20., 20.);
    QPointF rect2 = QPointF(30., 30.);
    QPointF rectRotate1 = QPointF(25., 25.);
    QPointF rectRotate2 = QPointF(22., 28.);

    QPointF ellipse1 = QPointF(110., 100.);
    QPointF ellipse2 = QPointF(145., 180.);
    QPointF ellipseCorner = QPointF(144., 179.);
    QPointF ellipseResize = QPointF(160., 150.);
    QPointF eShift = ellipseResize - ellipseCorner;

    QPointF ellipse3 = QPointF(100., 200.);
    QPointF ellipse4 = QPointF(90., 10.);
    QPointF inEllipse = QPointF(95., 50.);
    QPointF ellipseMove = QPointF(200., 150.);
    QPointF ellipseShift = ellipseMove - inEllipse;

    QPointF poly1 = QPointF(300., 10.);
    QPointF poly2 = QPointF(320., 15.);
    QPointF poly3 = QPointF(400., 190.);
    QPointF poly4 = QPointF(195., 156.);
    QPointF poly5 = QPointF(280., 158.);
    QPointF inPoly = QPointF(330., 100.);
    QPointF movePoly = QPointF(325., 200.);
    QPointF moveShift = movePoly - inPoly;
    QPointF polyVertex = QPointF(401., 191.);
    QPointF moveVertex = QPointF(365., 180.);
    QPointF vertexShift = moveVertex - polyVertex;

    QPointF free1 = QPointF(10., 10.);
    QPointF free2 = QPointF(10., 10.);
    QPointF free3 = QPointF(10., 10.);
    QPointF free4 = QPointF(10., 10.);
    QPointF free5 = QPointF(10., 10.);
    QPointF free6 = QPointF(10., 10.);
    QVector<QPointF> freePoints;

    QPointF erase1 = QPointF(10., 10.);
    QPointF erase2 = QPointF(10., 10.);
    QPointF erase3 = QPointF(10., 10.);
    QVector<QPointF> erasePoints;

    void makeMouseDownEvent(QPointF point, Qt::MouseButton button = Qt::LeftButton,
                            PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE);
    void makeMouseMoveEvent(QPointF from, QPointF to, Qt::MouseButton button = Qt::LeftButton,
                            PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE);
    void makeMouseReleaseEvent(QPointF from, QPointF to, Qt::MouseButton button = Qt::LeftButton,
                               PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE);
    void makeKeyPressEvent(Qt::Key key);

    int findLabelIndex(const QString name) const;

    void clickLabel(int index) {idm->labelButtons[index]->click();}
    void clickLabel(QString name) {idm->labelButtons[findLabelIndex(name)]->click();}
    void clickRectangleSelect() {idm->ui->rectangleSelectButton->click();}
    void clickEllipseSelect() {idm->ui->ellipseSelectButton->click();}
    void clickPolygonSelect() {idm->ui->polygonSelectButton->click();}
    void clickFreeDrawSelect() {idm->ui->freeSelectButton->click();}
    void clickFinishPolygon() {idm->polygonSelectForm->finishPolygonButton->click();}
    void clickInsertPoint() {idm->polygonSelectForm->insertPointButton->click();}
    void clickRoundBrush() {idm->freeSelectForm->roundBrushButton->click();}
    void clickSquareBrush() {idm->freeSelectForm->squareBrushButton->click();}
    void clickDrawButton() {idm->freeSelectForm->drawButton->click();}
    void clickEraseButton() {idm->freeSelectForm->eraseButton->click();}
    void setBrushSize(int val) {idm->freeSelectForm->brushSizeSpinner->setValue(val);}
    void clickMaskSelections() {idm->ui->showMaskSelections->click();}
    void clickAddSelection() {idm->ui->addSelectionButton->click();}
    void clickCancelSelection() {idm->ui->cancelSelectionButton->click();}
    void draw(QPointF from, QPointF to, Qt::MouseButton button = Qt::LeftButton,
              PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE) {
        makeMouseDownEvent(from, button, type);
        move(from, to, button, type);
        makeMouseReleaseEvent(from, to, button, type);
    }

    void click(QPointF point, Qt::MouseButton button = Qt::LeftButton,
               PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE) {
        makeMouseDownEvent(point, button, type);
        makeMouseReleaseEvent(point, point, button, type);
    }

    void move(QPointF from, QPointF to, Qt::MouseButton button = Qt::LeftButton,
              PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE) {
        makeMouseMoveEvent(from, to, button, type);
    }
    void freeDraw(QVector<QPointF> points, Qt::MouseButton button = Qt::LeftButton,
              PhotoScene::viewerTypes type = PhotoScene::PHOTO_VIEWER_TYPE) {
        makeMouseDownEvent(points[0], button, type);
        for (int i = 0; i < points.size() - 1; i++) {
            move(points[i], points[i + 1], button, type);
        }
        makeMouseReleaseEvent(points[0], points[points.size() - 1], button, type);
    }

    SelectItem* getItem(int type, ushort num = 1);
    void setup();
    void drawObjects();
    void clear();
};

#endif // INTEGRATIONTEST_H
