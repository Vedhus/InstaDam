#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include "instadam.h"
#include "filters.h"
#include "maskobjects.h"
#include "filtercontrols.h"
#include "paintBrush.h"
class maskObjects;
class filterControls;
namespace Ui {
class PhotoViewer;
}

enum viewerTypes{PHOTO_VIEWER_TYPE, MASK_VIEWER_TYPE};


class PhotoViewer : public QGraphicsView
{
    Q_OBJECT

public:
    bool hasPhoto;
    bool paintMode;
    paintBrush *B;


    int zoom;

    cv::Mat cvImage;
    cv::Mat cvThumb;
    maskObjects *maskObject;
    filterControls *filterControl;
    int selectedLabelClass;
    maskTypes selectedMask;
    QPen pen;
    QPainterPath path;
    int **colors;
    QGraphicsPathItem *brush;
    QGraphicsPixmapItem *photo;
    QPixmap imMask;

    QGraphicsScene *scene;
    QList<QGraphicsPixmapItem*> labelItems;
    QList<QGraphicsItem*> labelGroup;
    QGraphicsItemGroup * labels;
    QGraphicsPixmapItem * labelsTemp;
    QGraphicsPixmapItem *filterIm;

    QPoint lastPos;
    QPixmap currentMap;
    QPixmap pixmapFilt;



    viewerTypes viewerType;
    PhotoViewer(QWidget *parent = 0);
    void setPhotoFromFile(QString filename, QVector<QString> );
    void setPhotoFromPixmap(QPixmap, QPixmap );

    void setPhoto(QPixmap , QPixmap );

    void testPixmap();
    void fitInView();
    virtual void wheelEvent(QWheelEvent* ) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    void setFilterControls(filterControls *);
    void setPanMode();

    void setBrushMode(Qt::PenCapStyle);
    void setEraserMode();


    void setMaskPixmap();

    cv::Mat QImage2Mat(QImage const& src);

    cv::Mat QPixmap2Mat(QPixmap src);
    void resetBrushDrag();





signals:
    void photoClicked(QPoint);
    void zoomed(int, float, QPointF);
    void changedMask(maskTypes);
    void loadedPhoto();
    void brushResetting();

public slots:
    void zoomedInADifferentView(int , float, QPointF );
    void setImMask(maskTypes, threshold_or_filter thof = FILTER);
    void labelChanged(int, QColor);
    void opacityChanged(int, int);
    void brushSizeChanged(int);
    void resetBrush();






private:
    Ui::PhotoViewer *ui;

};

#endif // PHOTOVIEWER_H

