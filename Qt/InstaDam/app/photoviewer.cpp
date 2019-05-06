#include "photoviewer.h"

#include <math.h>

#include "pixmapops.h"
#include "Selector/photoScene.h"
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/core/mat.hpp"
#include "ui_instadam.h"

/*!
  \class PhotoViewer
  \ingroup app
  \inmodule InstaDam
  \inherits QGraphicsView
  \brief Viewer for displaying an image and drawing SelectItems on it.
*/

/*!
  Craetes an instance with parent QWidget \a parent.
*/
PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent) {
    viewerType = PhotoScene::PHOTO_VIEWER_TYPE;
    scene = new PhotoScene(viewerType, this);
    photo = new QGraphicsPixmapItem();
    filterIm = new QGraphicsPixmapItem();

    hasPhoto = false;

    scene->addItem(photo);

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
    selectedMask = EnumConstants::CANNY;
    connect(this, SIGNAL(zoomed(int, float , QPointF)), this, SLOT(updateZoomFactor(int, float , QPointF)));
}

/*!
  For testing
*/
void PhotoViewer::testPixmap() {
    QPixmap pixmap2 =  QPixmap("G:/My Drive/SSTL/DCNN/Labellers/Python Labeller/InstaDamPublic/Qt/InstaDam/im2.JPG");
    this->photo->setPixmap(pixmap2);
}

/*!
  Sets the internal filter controls to \a fc.
*/
void PhotoViewer::setFilterControls(filterControls *fc) {
    filterControl = fc;
}

#ifdef WASM_BUILD
QSize PhotoViewer::setPhotoFromByteArray(QByteArray &array) {
    QPixmap pixmap;
    QColor whiteColor = QColor(1, 1, 1, 0);
    if (!pixmap.loadFromData(array)) {
        return QSize(0, 0);
    }
    cvImage = QPixmap2Mat(pixmap);

    setPhoto(pixmap);
    return pixmap.size();
}
#endif

/*!
  Sets the internal image based on data from \a filename.
*/
QSize PhotoViewer::setPhotoFromFile(QString filename) {
    QPixmap pixmap = QPixmap(filename);

    cvImage = cv::imread(filename.toLocal8Bit().constData(),
                         CV_LOAD_IMAGE_COLOR);
    cv::cvtColor(cvImage,cvImage, cv::COLOR_BGR2RGB);

    setPhoto(pixmap);
    return pixmap.size();
}

/*!
  Sets the internal image based on \a px.
*/
void PhotoViewer::setPhotoFromPixmap(QPixmap px) {
    cvImage = QPixmap2Mat(px);
    setPhoto(px);
}

/*!
  Returns color at a point \a px.
*/
cv::Scalar PhotoViewer::colorAtPoint(QPointF pos) {
    float x = limit(0,pos.x(),cvImage.cols);
    float y = limit(0,pos.y(),cvImage.rows);
    qInfo()<<"Number of channels"<<cvImage.channels();
    cv::Scalar col = cv::Scalar(cvImage.at<cv::Vec3b>(cv::Point(x,y)));
    qInfo()<<"type"<<cvImage.type();

    return col;
}

/*! Limits a value to \a maxval and \a minval
 */
float PhotoViewer::limit(int minval, qreal x, int  maxval)
{
    if (x > maxval)
        return maxval;
    else if (x<minval)
        return minval;
    else {
        return x;
    }
}


/*!
  Sets the internal image based on \a pixmap.
*/
void PhotoViewer::setPhoto(QPixmap pixmap) {
    cv::resize(cvImage, cvThumb, cv::Size(200, 200), CV_INTER_LINEAR);

    if (pixmap.isNull() == 0) {
        if (this->viewerType == PhotoScene::PHOTO_VIEWER_TYPE) {
                this->photo->setPixmap(pixmap);
                this->hasPhoto = true;
        }

        QRect viewrect = viewport()->rect();
        setImMask(selectedMask);

        this->fitInView();

        emit zoomed(zoom, 1,  mapToScene(static_cast<int>(viewrect.width()/2.0),
                                          static_cast<int>(round((viewrect.height()+1)/2.0))));
        emit loadedPhoto();
    }
}

/*!
  Converts \a src to a cv::Mat.
*/
cv::Mat PhotoViewer::QImage2Mat(const QImage &src) const {
     cv::Mat tmp(src.height(), src.width(), CV_8UC3,
                 const_cast<uchar*>(src.bits()),
                 static_cast<size_t>(src.bytesPerLine()));
     cv::Mat result;  // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result, CV_BGR2RGB);
     return result;
}

/*!
  \overload QImage2Mat
  Converts \a px to a cv::Mat.
*/
cv::Mat PhotoViewer::QPixmap2Mat(QPixmap px) const {
     QImage src = px.toImage();
     cv::Mat tmp(src.height(), src.width(), CV_8UC3,
                 const_cast<uchar*>(src.bits()),
                 static_cast<size_t>(src.bytesPerLine()));
     cv::Mat result;  // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result, CV_BGR2RGB);
     return result;
}

/*!
  Sets the internal mask based on \a filterName and \a thof.
*/
void PhotoViewer::setImMask(EnumConstants::maskTypes filterName,
                            EnumConstants::threshold_or_filter thof) {
    selectedMask = filterName;

    if (hasPhoto == true) {
        switch (thof) {
            case EnumConstants::FILTER:
                filterControl->filtAndGeneratePixmaps(cvImage, selectedMask);
                break;
            case EnumConstants::THRESH:

                filterControl->im2pixmap(selectedMask);
                break;
        }
        pixmapFilt = filterControl->qImg;
        imMask = filterControl->qAlpha;
        filterIm->setPixmap(pixmapFilt);
        emit changedMask(selectedMask);
    }
}

/*!
  Something based on \a zoom_input, \a factor, \a point.
*/
void PhotoViewer::zoomedInADifferentView(int zoom_input, float factor,
                                         QPointF point) {
    zoom = zoom_input;

    if (zoom > 0) {
        scale(static_cast<qreal>(factor), static_cast<qreal>(factor));
    } else if (zoom ==0) {
        fitInView();
    } else {
        zoom = 0;
    }
    centerOn(point);
}

/*!
  Something
*/
void PhotoViewer::fitInView() {
    int MAX = 100;
    QRectF rect = QRectF(photo->pixmap().rect());
    if (!rect.isNull()) {
        setSceneRect(rect);
        if (this->hasPhoto) {
            QRectF unity = transform().mapRect(QRectF(0, 0, 1, 1));
            if (1.0/unity.width() > MAX || 1.0/unity.height() >MAX)
                    QGraphicsView::fitInView(scene->sceneRect(),
                                             Qt::KeepAspectRatio);
            else
                this->scale(1.0/unity.width(), 1.0/unity.height());
            QRect viewrect = this->viewport()->rect();
            QRectF scenerect = transform().mapRect(rect);

            double factor = fmin(static_cast<double>(viewrect.width()) / scenerect.width(),
                                 static_cast<double>(viewrect.height()) / scenerect.height());
            if (factor > MAX)
                QGraphicsView::fitInView(scene->sceneRect(),
                                         Qt::KeepAspectRatio);
            else
                this->scale(static_cast<qreal>(factor),
                            static_cast<qreal>(factor));
        }
        this->zoom = 0;
    }
}


/*!
  Sets the pan mode to either PAN (\a mode = true) or PAINTBRUSH (\a mode =
  false).
*/
void PhotoViewer::setPanMode(bool mode) {

    if (mode) {
//        setCursor(Qt::ArrowCursor);
        setDragMode(QGraphicsView::ScrollHandDrag);
    } else {
        setDragMode(QGraphicsView::NoDrag);
        resetCursor();
    }
}

/*!
  Sets the cursor to brush cursor of size \a size
*/
void PhotoViewer::setRoundBrushCursor(int size){
    this->brushSize = size;
    cursorState = ROUNDBRUSH;
    updateCursorShape(cursorState);
    setCursor(brushCursor);


}

/*!
  Sets the cursor to brush cursor of size \a size
*/
void PhotoViewer::setSquareBrushCursor(int size){
    this->brushSize = size;
    cursorState = SQUAREBRUSH;
    updateCursorShape(cursorState);
    setCursor(brushCursor);
}

/*!
  Updates the size of the brush cursor
*/
void PhotoViewer::updateCursorShape(cursorStates cs){
    int size = this->brushSize*this->currentZoomFactor;
    QPixmap pixmap(size+1, size+1);
    pixmap.fill(Qt::transparent);
    QPainter *painter = new QPainter(&pixmap);
    QPen pen = QPen(QColor(50,50,50,255));
    pen.setWidth(1);
    painter->setPen(pen);
    if (cs == SQUAREBRUSH)
        painter->drawRect(0, 0,size, size);
    else
        painter->drawEllipse(0, 0,size, size);
    brushCursor = QCursor(pixmap, -size, -size);
}



/*!
  Updates the zoom factor
*/
void PhotoViewer::updateZoomFactor(int zoom_input, float factor, QPointF point){

    this->currentZoomFactor  = transform().m11();
    resetCursor();
}


/*!
  Sets the cursor to arrow cursor
*/
void PhotoViewer::setArrowCursor(){
    setCursor(Qt::ArrowCursor);
    cursorState = ARROW;

}

/*! Resets the cursor to currentState
 */
void PhotoViewer::resetCursor(){
    switch (cursorState)
    {
    case ARROW:
        setArrowCursor();
        break;
    case ROUNDBRUSH:
        setRoundBrushCursor(this->brushSize);
        break;
    case SQUAREBRUSH:
        setSquareBrushCursor(this->brushSize);
        break;
    }

}




/*!
  Resets the internal brush to \a size and style \a capStyle_input.
*/
void PhotoViewer::resetBrush(int size, Qt::PenCapStyle capStyle_input) {
    brushSize = size;

    delete brush;
    pen = QPen();

    pen.setColor(QColor(255, 0, 255, 255));
    pen.setWidth(size);

    capStyle = capStyle_input;
    pen.setCapStyle(capStyle);
    path = QPainterPath();
    brush = new QGraphicsPathItem();
    brush->setPen(pen);
    scene->addItem(brush);
    paintMode = false;
}

/*!
  Sets the brush mode to \a cap.
*/
void PhotoViewer::setBrushMode(Qt::PenCapStyle cap) {
    if (this->hasPhoto) {
        setDragMode(QGraphicsView::NoDrag);
        this->capStyle = cap;
        resetBrush(this->brushSize, this->capStyle);
        this->update();
    }
}

/*!
  \reimp
*/
void PhotoViewer::wheelEvent(QWheelEvent* event) {
    float factor = 1;
    QRect viewrect = this->viewport()->rect();
    if (this->hasPhoto) {
        if (event->angleDelta().y() > 0) {
            factor = 1.25;
            zoom += 1;
        } else {
            factor = 0.8f;
            zoom -= 1;
        }

        if (zoom > 0) {
            this->scale(static_cast<qreal>(factor), static_cast<qreal>(factor));
            this->centerOn(this->mapToScene(viewrect.width()/2,
                                            static_cast<int>(round((viewrect.height()+1)/2.0))));
        } else if (zoom == 0) {
            this->fitInView();
        } else {
            zoom = 0;
        }

        QPointF point = this->mapToScene(static_cast<int>(round(viewrect.width()/2.0)),
                                         static_cast<int>(round((viewrect.height())/2.0)));
        emit zoomed(zoom, factor, point);
    }
}

/*!
  \reimp
*/
void PhotoViewer::resizeEvent(QResizeEvent *event) {
    fitInView();
    QGraphicsView::resizeEvent(event);
}

/*!
  \reimp
*/
void PhotoViewer::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);

    if (dragMode() == QGraphicsView::NoDrag) {
        paintMode = true;
        update();
    }
}

/*!
  \reimp
*/
void PhotoViewer::mouseMoveEvent(QMouseEvent* event) {
    if (hasPhoto) {
        if (!paintMode) {
            QRect viewrect = viewport()->rect();
            QPointF point = this->mapToScene(static_cast<int>(round(viewrect.width()/2.0)),
                                             static_cast<int>(round(viewrect.height()/2.0)));
            emit zoomed(zoom, 1, point);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

/*!
  \reimp
*/
void PhotoViewer::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    paintMode = false;
    update();
}

/*!
  \fn PhotoViewer::zoomed(int i, float a, QPointF point)

  This signal is emitted when PhotoViewer zooms, indicating zoom count with \a i,
  zoom factor, \a a, and center of zoom \a point.
*/

/*!
  \fn PhotoViewer::changedMask(EnumConstants::maskTypes type)

  This signal is emitted when the mask is changed to \a type.
*/

/*!
  \fn PhotoViewer::loadedPhoto()

  This signal is emitted when a photo is loaded.
*/
