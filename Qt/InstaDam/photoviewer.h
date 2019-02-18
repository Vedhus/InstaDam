#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QFrame>
#include <QGraphicsView>
#include <QMainWindow>
//#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QtCore>
#include <QBitmap>
#include <QPainter>

#include "Selector/photoScene.h"

namespace Ui {
class PhotoViewer;
}

enum viewerTypes{PHOTO_VIEWER_TYPE, MASK_VIEWER_TYPE};
enum maskTypes{CANNY, THRESHOLD, BLUR, OTHER};
enum brushTypes {PAINTBRUSH, ERASER, PAN};

class PhotoViewer : public QGraphicsView
{
    Q_OBJECT

public:
    bool hasPhoto;
    bool paintMode;
    int zoom;
    Qt::PenCapStyle capStyle;
    int brushSize;

    brushTypes brushType;
    maskTypes selectedMask;
    QPen pen;
    QPainterPath path;
    int **colors;
    QGraphicsPathItem *brush;
    QGraphicsPixmapItem *photo;
    QPixmap imMask;

    PhotoScene *scene;
    QGraphicsPixmapItem *labels;
    QGraphicsPixmapItem *labelsTemp;
    QGraphicsPixmapItem *filterIm;
    QPoint lastPos;
    QPixmap currentMap;


    viewerTypes viewerType;
    PhotoViewer(QWidget *parent = 0);
    void setPhoto(QPixmap pixmap);
    void testPixmap();
    void fitInView(bool);
    virtual void wheelEvent(QWheelEvent* ) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    void setPanMode();
    void resetBrush(int, Qt::PenCapStyle);
    void setBrushMode(Qt::PenCapStyle);
    void setImMask(maskTypes, QRect *);


signals:
    void photoClicked(QPoint);
    void zoomed(int, float, QPointF);
    void loadedPhoto(bool);







//    QPixmap *getThumbPixmap(int maskType = 0);
//    void setPicButtonTumbnails();
//    void getMaskFromMaskView();
//    void modifyMaskThreshold(int val = 0);
//    void zoomedInADifferentView(int zoom, int factor, QPoint point);




private:
    Ui::PhotoViewer *ui;

};

#endif // PHOTOVIEWER_H
