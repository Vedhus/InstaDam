#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H


#include "instadam.h"
#include "filters.h"
#include "maskobjects.h"
#include "filtercontrols.h"
#include "paintbrush.h"
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

class maskObjects;
class filterControls;

namespace Ui {
class PhotoViewer;
}


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

    PhotoScene *scene;
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
#ifdef WASM_BUILD
    QSize setPhotoFromByteArray(QByteArray &array, QString labelname);
#endif
    QSize setPhotoFromFile(QString filename, QVector<QString> );
    void setPhotoFromPixmap(QPixmap, QPixmap );
    void setBrush(int brushSize, int brushMode, QSharedPointer<Label> label = nullptr);
    void setEraser(int brushSize, int brushMode, QSharedPointer<Label> label = nullptr);
    void setPhoto(QPixmap , QPixmap );

    void testPixmap();
    void fitInView();
    virtual void wheelEvent(QWheelEvent* ) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    void setFilterControls(filterControls *);
    void setPanMode(bool);

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












//enum brushTypes {PAINTBRUSH, ERASER, PAN};

//class PhotoViewer : public QGraphicsView
//{
//    Q_OBJECT

//public:
//    bool hasPhoto;
//    bool paintMode;
//    int zoom;
//    Qt::PenCapStyle capStyle;
//    int brushSize;
//    cv::Mat cvImage;
//    cv::Mat cvThumb;
//    maskObjects *maskObject;
//    filterControls *filterControl;

//    brushTypes brushType;
//    maskTypes selectedMask;
//    QPen pen;
//    QPainterPath path;
//    int **colors;
//    QGraphicsPathItem *brush;
//    QGraphicsPixmapItem *photo;
//    QPixmap imMask;

//    PhotoScene *scene;
//    QGraphicsPixmapItem *labels;
//    QGraphicsPixmapItem *labelsTemp;
//    QGraphicsPixmapItem *filterIm;

//    QPoint lastPos;
//    QPixmap currentMap;
//    QPixmap pixmapFilt;



//    viewerTypes viewerType;
//    PhotoViewer(QWidget *parent = nullptr);
//    QSize setPhotoFromFile(QString filename, QString labelname);
//#ifdef WASM_BUILD
//    QSize setPhotoFromByteArray(QByteArray &array, QString labelname);
//#endif
//    void setPhotoFromPixmap(QPixmap, QPixmap );

//    void setPhoto(QPixmap , QPixmap , QPixmap );

//    void testPixmap();
//    void fitInView();
//    virtual void wheelEvent(QWheelEvent* ) override;
//    virtual void mousePressEvent(QMouseEvent*) override;
//    virtual void mouseMoveEvent(QMouseEvent*) override;
//    virtual void mouseReleaseEvent(QMouseEvent*) override;
//    virtual void resizeEvent(QResizeEvent *event) override;
//    void setFilterControls(filterControls *);
//    void setPanMode(bool mode);
//    void resetBrush(int, Qt::PenCapStyle);
//    void setBrushMode(Qt::PenCapStyle);

//    void setMaskPixmap();

//    cv::Mat QImage2Mat(QImage const& src);

//    cv::Mat QPixmap2Mat(QPixmap src);





//signals:
//    void photoClicked(QPoint);
//    void zoomed(int, float, QPointF);
//    void changedMask(maskTypes);
//    void loadedPhoto();

//public slots:
//    void zoomedInADifferentView(int , float, QPointF );
//    void setImMask(maskTypes, threshold_or_filter thof = FILTER);






//private:
//    Ui::PhotoViewer *ui;

//};

//#endif // PHOTOVIEWER_H
