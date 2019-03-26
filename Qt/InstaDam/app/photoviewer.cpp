#include "photoviewer.h"
#include "pixmapops.h"
#include "Selector/photoScene.h"
#include <math.h>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/core/mat.hpp"
#include "ui_instadam.h"
#include <iostream>
using namespace std;
PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent)
{
    viewerType = PhotoScene::PHOTO_VIEWER_TYPE;
    scene = new PhotoScene(viewerType, this);
    photo = new QGraphicsPixmapItem();
    labelsTemp = new QGraphicsPixmapItem();
    labels = new QGraphicsPixmapItem();
    filterIm = new QGraphicsPixmapItem();

    hasPhoto = false;

    labelsTemp->setOpacity(0);
    labels->setOpacity(0.5);

    scene->addItem(labelsTemp);
    scene->addItem(photo);
    scene->addItem(labels);

    setScene(scene);
    zoom = 1;
    brush = new QGraphicsPathItem();
    scene->addItem(brush);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setDragMode(QGraphicsView::NoDrag);
    resetBrush(10, Qt::RoundCap);
    maskObject = new maskObjects();
    selectedMask = CANNY;

}

void PhotoViewer::testPixmap()
{
    QPixmap pixmap2 =  QPixmap("G:/My Drive/SSTL/DCNN/Labellers/Python Labeller/InstaDamPublic/Qt/InstaDam/im2.JPG");
    this->photo->setPixmap(pixmap2);
}

void PhotoViewer::setFilterControls(filterControls *fc)
{
    filterControl = fc;
}

#ifdef WASM_BUILD
QSize PhotoViewer::setPhotoFromByteArray(QByteArray &array, QString labelname){
    //scene->update();

    QPixmap pixmap;
    QColor whiteColor = QColor(0,0,0,0);
    if(!pixmap.loadFromData(array)){
        return QSize(0,0);
    }
    cvImage = QPixmap2Mat(pixmap);

    QPixmap white_temp = QPixmap(pixmap.size());
    white_temp.fill(whiteColor);
    QPixmap labelMap;
    if (labelname.isNull())
    {
        labelMap = QPixmap(pixmap.size());
        labelMap.fill(whiteColor);

    }
    else
        labelMap = QPixmap(labelname);
    setPhoto(pixmap, labelMap, white_temp);
    return pixmap.size();
}
#endif
QSize PhotoViewer::setPhotoFromFile(QString filename, QString labelname)
{

    QColor whiteColor = QColor(0,0,0,0);
    QPixmap pixmap = QPixmap(filename);

    cvImage = cv::imread(filename.toLocal8Bit().constData(), CV_LOAD_IMAGE_COLOR);


    QPixmap labelMap;
    if (labelname.isNull())
    {
        labelMap = QPixmap(pixmap.size());
        labelMap.fill(whiteColor);

    }
    else
        labelMap = QPixmap(labelname);

    QPixmap white_temp = QPixmap(pixmap.size());
    white_temp.fill(whiteColor);
    setPhoto(pixmap, labelMap, white_temp);

    return pixmap.size();
}






void PhotoViewer::setPhotoFromPixmap(QPixmap px, QPixmap lpx)
{
    cvImage = QPixmap2Mat(px);

    QPixmap white_temp = QPixmap(px.size());
    QColor whiteColor = QColor(0,0,0,0);
    white_temp.fill(whiteColor);
    setPhoto(px, lpx, white_temp);

}

void PhotoViewer::setPhoto(QPixmap pixmap, QPixmap labelsPixmap, QPixmap labelsTempPixmap)
{

    cv::resize(cvImage, cvThumb,cv::Size(200,200) , CV_INTER_LINEAR);



    if (pixmap.isNull() == 0)
    {
        if (this->viewerType == PhotoScene::PHOTO_VIEWER_TYPE)
        {
                this->photo->setPixmap(pixmap);
                this->labels->setPixmap(labelsPixmap);
                this->labelsTemp->setPixmap(labelsTempPixmap);
                this->hasPhoto = true;

        }

    QRect viewrect = viewport()->rect();
    setImMask(selectedMask);
    qInfo("imMask set!");
    this->fitInView();
    qInfo("fit In View!");
    emit zoomed( zoom, 1,  mapToScene(viewrect.width()/2.0, round((viewrect.height()+1)/2.0)));
    emit loadedPhoto();

    }
}

cv::Mat PhotoViewer::QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}

cv::Mat PhotoViewer::QPixmap2Mat(QPixmap px)
{
     QImage src = px.toImage();
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}

void PhotoViewer::setImMask(maskTypes filterName, threshold_or_filter thof)
{
    selectedMask = filterName;
    qInfo("SettingMask");
    if (hasPhoto == true)
    {
        switch(thof)
        {case FILTER:
           filterControl->filtAndGeneratePixmaps(cvImage, selectedMask);
            break;
        case THRESH:
            qInfo("Case Threshold");
            filterControl->im2pixmap(selectedMask);
            break;
        }
        pixmapFilt = filterControl->qImg;
        imMask = filterControl->qAlpha;
        filterIm->setPixmap(pixmapFilt);
        emit changedMask(selectedMask);
    }

}




void PhotoViewer::zoomedInADifferentView(int zoom_input, float factor, QPointF point)
{
    zoom = zoom_input;

    if (zoom > 0)
    {
        scale(factor, factor);
    }
    else if(zoom ==0)
    {
        fitInView();
    }
    else {
        zoom = 0;
    }
    centerOn(point);
}




void PhotoViewer::fitInView()
{
    int MAX = 100;
    QRectF rect = QRectF(photo->pixmap().rect());
    if (!rect.isNull())
    {
        setSceneRect(rect);
        if (this->hasPhoto)
        {
            QRectF unity = transform().mapRect(QRectF(0,0,1,1));
            if (1.0/unity.width() > MAX || 1.0/unity.height() >MAX)
                    QGraphicsView::fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
            else
                this->scale(1.0/unity.width(), 1.0/unity.height());
            QRect viewrect = this->viewport()->rect();
            QRectF scenerect = transform().mapRect(rect);

            float factor = fmin(viewrect.width() / scenerect.width(), viewrect.height() / scenerect.height());
            if (factor>MAX)
                     QGraphicsView::fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
            else
                this->scale(factor, factor);

        }
        this->zoom = 0;
    }

}


void PhotoViewer::setPanMode(bool mode)
{
    if(mode){
        this->brushType = PAN;
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else{
        this->brushType = PAINTBRUSH;
        setDragMode(QGraphicsView::NoDrag);
    }
}

void PhotoViewer::resetBrush(int size = 10, Qt::PenCapStyle capStyle_input = Qt::RoundCap)
{

    brushSize = size;

    //scene->removeItem(brush);
    delete brush;
    pen = QPen();

    pen.setColor(QColor(255,0,255,255));
    pen.setWidth(size);

    capStyle = capStyle_input;
    pen.setCapStyle(capStyle);
    path = QPainterPath();
    brush = new QGraphicsPathItem();
    brush->setPen(pen);
    scene->addItem(brush);
    paintMode = false;
}

void PhotoViewer::setBrushMode(Qt::PenCapStyle cap)
{
    if (this->hasPhoto)
    {
        setDragMode(QGraphicsView::NoDrag);
        this->capStyle = cap;
        this->brushType = PAINTBRUSH;
        resetBrush(this->brushSize, this->capStyle);
        this->update();
    }
}




void PhotoViewer::wheelEvent(QWheelEvent* event)

{
    float factor = 1;
    QRect viewrect = this->viewport()->rect();
    if (this->hasPhoto)
    {
        if (event->angleDelta().y() > 0)
        {
            factor = 1.25;
            zoom += 1;
        }
        else
        {
            factor = 0.8f;
            zoom -= 1;
        }

        if (zoom > 0)
        {
            this->scale(factor, factor);
            this->centerOn(this->mapToScene(viewrect.width()/2, round((viewrect.height()+1)/2.0)));
        }

        else if (zoom == 0)
        {
            this->fitInView();
        }
        else
        {
            zoom = 0;
        }

        QPointF point = this->mapToScene(round(viewrect.width()/2.0), round((viewrect.height())/2.0));
        emit zoomed(zoom, factor, point);

    }

}

void PhotoViewer::resizeEvent(QResizeEvent *event)
{

    fitInView();
    QGraphicsView::resizeEvent(event);
}



void PhotoViewer::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);

    //if (hasPhoto)
    //{
    //    if (photo->isUnderMouse())
    //    {
    //        emit photoClicked(QPoint(event->pos()));
    //    }
    if (dragMode() == QGraphicsView::NoDrag)
    {
            //resetBrush(brushSize, capStyle);
            //lastPos = event->pos();
            //currentMap = labelsTemp->pixmap();
            //if (viewerType == PhotoScene::MASK_VIEWER_TYPE)
            //    currentMap.setMask(imMask.mask());
            paintMode = true;
            update();
        //}

    }
}


void PhotoViewer::mouseMoveEvent(QMouseEvent* event)
{

    if(hasPhoto)
    {
        if (!paintMode)
        {
            QRect viewrect = viewport()->rect();
            QPointF point = this->mapToScene(round(viewrect.width()/2.0), round(viewrect.height()/2.0));
            emit zoomed(zoom, 1, point);
        }

    }
    QGraphicsView::mouseMoveEvent(event);


}

void PhotoViewer::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
    //if(hasPhoto)
    //{
    //    if (paintMode)
    //    {
            //if (viewerType == PhotoScene::PHOTO_VIEWER_TYPE)
            //{
            //    this->labels->setPixmap(directPixmaps(labels->pixmap(),currentMap, brushType));
            //}
            //else if (viewerType == PhotoScene::MASK_VIEWER_TYPE)
            //    this->labels->setPixmap(maskPixmaps(labels->pixmap(),currentMap, imMask, brushType));
            //else
            //    qInfo("Invalid viewer type");
            paintMode = false;
            //resetBrush(brushSize, capStyle);
            update();
    //    }
    //}

}
