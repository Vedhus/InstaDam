#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QFrame>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>


namespace Ui {
class PhotoViewer;
}

class PhotoViewer : public QGraphicsView
{
    Q_OBJECT

public:
    bool hasPhoto;
    QGraphicsPixmapItem *photo;
    incrementI();
    QGraphicsScene *scene;
    QGraphicsPixmapItem *labels;
    QGraphicsPixmapItem *labelsTemp;
    QGraphicsPixmapItem *filterIm;
    PhotoViewer(QWidget *parent = 0);
    void setPhoto(QPixmap pixmap);
    void testPixmap();
    void fitInView(bool);


//    void resetBrush(int size = 10, int capStyle = 0);
//    void fitInView(bool scale = true);
//    void setImMask(int maskType = 0, QRect *rect = nullptr);
//    QPixmap *getThumbPixmap(int maskType = 0);
//    void setPicButtonTumbnails();
//    void getMaskFromMaskView();
//    void modifyMaskThreshold(int val = 0);
//    void zoomedInADifferentView(int zoom, int factor, QPoint point);
//    void wheelEvent(QWheelEvent *event) override;
//    void setPanMode();
//    void setBrushMode(int brushType);
//    void setBrushShape(int brushShape, QRect *rect);
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    Ui::PhotoViewer *ui;

};

#endif // PHOTOVIEWER_H
