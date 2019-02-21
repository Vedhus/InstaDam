#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include "instadam.h"
#include "filters.h"
#include "maskobjects.h"
#include "filtercontrols.h"
class maskObjects;
class filterControls;
namespace Ui {
class PhotoViewer;
}

enum viewerTypes{PHOTO_VIEWER_TYPE, MASK_VIEWER_TYPE};

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
    cv::Mat cvImage;
    cv::Mat cvThumb;
    maskObjects *maskObject;
    filterControls *filterControl;

    brushTypes brushType;
    maskTypes selectedMask;
    QPen pen;
    QPainterPath path;
    int **colors;
    QGraphicsPathItem *brush;
    QGraphicsPixmapItem *photo;
    QPixmap imMask;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *labels;
    QGraphicsPixmapItem *labelsTemp;
    QGraphicsPixmapItem *filterIm;

    QPoint lastPos;
    QPixmap currentMap;
    QPixmap pixmapFilt;



    viewerTypes viewerType;
    PhotoViewer(QWidget *parent = 0);
    void setPhoto(QString);
    void testPixmap();
    void fitInView();
    virtual void wheelEvent(QWheelEvent* ) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    void setFilterControls(filterControls *);
    void setPanMode();
    void resetBrush(int, Qt::PenCapStyle);
    void setBrushMode(Qt::PenCapStyle);

    void setMaskPixmap();




signals:
    void photoClicked(QPoint);
    void zoomed(int, float, QPointF);
    void changedMask(maskTypes);
    void loadedPhoto();

public slots:
    void zoomedInADifferentView(int , float, QPointF );
    void setImMask(maskTypes, threshold_or_filter thof = FILTER);






private:
    Ui::PhotoViewer *ui;

};

#endif // PHOTOVIEWER_H
