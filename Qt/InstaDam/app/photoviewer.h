#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H
#include <QElapsedTimer>
#include <QFrame>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QtCore>
#include <QBitmap>
#include <QPainter>

#include "maskobjects.h"
#include "filtercontrols.h"
#include "photoScene.h"

namespace Ui {
class PhotoViewer;
}

enum brushTypes {PAINTBRUSH, ERASER, PAN};
enum cursorStates {ARROW, ROUNDBRUSH, SQUAREBRUSH};
class PhotoViewer : public QGraphicsView {
    Q_OBJECT

 public:
    static const int thumbSize = 150;
    explicit PhotoViewer(QWidget *parent = nullptr);
    cursorStates cursorState;
    bool hasPhoto;
    bool paintMode;
    int zoom;
    Qt::PenCapStyle capStyle;
    int brushSize;
    cv::Mat cvImage;
    cv::Mat cvThumb;
    maskObjects *maskObject;
    filterControls *filterControl;
    float currentZoomFactor = 1;

    brushTypes brushType;
    EnumConstants::maskTypes selectedMask;
    QPen pen;
    QPainterPath path;
    int **colors;
    QGraphicsPathItem *brush;
    QGraphicsPixmapItem *photo;
    QPixmap imMask;

    PhotoScene *scene;
    QGraphicsPixmapItem *filterIm;
    QPixmap pixmapImage;
    QPoint lastPos;
    QPixmap currentMap;
    QPixmap pixmapFilt;

    PhotoScene::viewerTypes viewerType;
    QSize setPhotoFromFile(QString filename);
    QElapsedTimer *timer;
    QTimer *displayTimer;
    bool measure = false;
    float times = 0;

    cv::Mat timesMat;
    cv::Mat filterTimesMat;

#ifdef WASM_BUILD
    QSize setPhotoFromByteArray(QByteArray &array);
#endif
    void setPhotoFromPixmap(QPixmap px);

    void setPhoto(QPixmap pixmap);

    void testPixmap();
    void fitInView();
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void setFilterControls(filterControls *event);
    void setPanMode(bool mode);
    void resetBrush(int size = 10, Qt::PenCapStyle capStyle_input = Qt::RoundCap);
    void setBrushMode(Qt::PenCapStyle cap);
    cv::Scalar colorAtPoint(QPointF pos);
    float limit(int , qreal , int );

    void resetCursor();

    //void setMaskPixmap();

    cv::Mat QImage2Mat(const QImage &src) const;

    cv::Mat QPixmap2Mat(QPixmap px) const;
    void setArrowCursor();
    void updateCursorShape(cursorStates);
    void setRoundBrushCursor(int size);
    void setSquareBrushCursor(int size);
    QCursor brushCursor;
    QCursor previousCursor;

 signals:
    // void photoClicked(QPoint);
    void zoomed(int i, float a, QPointF point);
    void changedMask(EnumConstants::maskTypes type);
    void loadedPhoto();
    void newTime(float);


 public slots:
    void zoomedInADifferentView(int zoom_input, float factor, QPointF point);
    void setImMask(EnumConstants::maskTypes filterName,
                   EnumConstants::threshold_or_filter thof = EnumConstants::FILTER);
    void updateZoomFactor(int zoom_input, float factor, QPointF point);
    void checkTime();

 private:
    Ui::PhotoViewer *ui;
 protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif  // PHOTOVIEWER_H
