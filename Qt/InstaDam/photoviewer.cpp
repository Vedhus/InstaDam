#include "photoviewer.h"
#include "pixmapops.h"
#include "Selector/photoScene.h"
#include <math.h>


PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent)
{
    viewerType = PHOTO_VIEWER_TYPE;
    scene = new PhotoScene(this);
    photo = new QGraphicsPixmapItem();
    labelsTemp = new QGraphicsPixmapItem();
    labels = new QGraphicsPixmapItem();

    hasPhoto = false;

    labelsTemp->setOpacity(0);
    labels->setOpacity(1);

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

}

void PhotoViewer::testPixmap()
{
    QPixmap pixmap2 =  QPixmap("G:/My Drive/SSTL/DCNN/Labellers/Python Labeller/InstaDamPublic/Qt/InstaDam/im2.JPG");
    this->photo->setPixmap(pixmap2);
}

void PhotoViewer::setPhoto(QPixmap pixmap)
{



    QPixmap white = QPixmap(pixmap.size());
    QPixmap white_temp = QPixmap(pixmap.size());
    QColor whiteColor = QColor(0,0,0,0);
    white.fill(whiteColor);
    white_temp.fill(whiteColor);

    if (pixmap.isNull() == 0)
    {
        if (this->viewerType == PHOTO_VIEWER_TYPE)
        {
                this->photo->setPixmap(pixmap);
                this->labels->setPixmap(white);
                this->labelsTemp->setPixmap(white_temp);
                this->hasPhoto = true;

        }
        this->fitInView(true);

    }
}


void PhotoViewer::fitInView(bool scale = true)
{
    QRectF rect = QRectF(photo->pixmap().rect());
    if (!rect.isNull())
    {
        setSceneRect(rect);
        if (this->hasPhoto)
        {
            QRectF unity = transform().mapRect(QRectF(0,0,1,1));
            this->scale(1.0/unity.width(), 1.0/unity.height());
            QRect viewrect = this->viewport()->rect();
            QRectF scenerect = transform().mapRect(viewrect);

            float factor = fmin(viewrect.width() / scenerect.width(), viewrect.height() / scenerect.height());
            this->scale(factor, factor);

        }
        this->zoom = 0;
    }

}


void PhotoViewer::setPanMode()
{
    this->brushType = PAN;
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void PhotoViewer::resetBrush(int size = 10, Qt::PenCapStyle capStyle_input = Qt::RoundCap)
{
    brushSize = size;
    scene->removeItem(brush);
    pen = QPen();

    pen.setColor(QColor(255,0,255,255));
    pen.setWidth(size);

    capStyle = capStyle_input;
    pen.setCapStyle(capStyle);
    path = QPainterPath();
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



void PhotoViewer::setImMask(maskTypes filterName, QRect *rect)
{
    if (hasPhoto)
    {
        selectedMask = filterName;

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
            factor = 0.8;
            zoom -= 1;
        }

        if (zoom > 0)
        {
            this->scale(factor, factor);
            this->centerOn(this->mapToScene(viewrect.width()/2, round((viewrect.height()+1)/2.0)));
        }

        else if (zoom == 0)
        {
            this->fitInView(true);
        }
        else
        {
            zoom = 0;
        }

        QPointF point = this->mapToScene(viewrect.width()/2.0, round((viewrect.height()+1)/2.0));

    }

}



void PhotoViewer::mousePressEvent(QMouseEvent* event)
{

    if (hasPhoto)
    {
        if (photo->isUnderMouse())
        {
            emit photoClicked(QPoint(event->pos()));
        }
        if (dragMode() == QGraphicsView::NoDrag)
        {
            resetBrush(brushSize, capStyle);
            lastPos = event->pos();
            currentMap = labelsTemp->pixmap();
            if (viewerType == MASK_VIEWER_TYPE)
                currentMap.setMask(imMask.mask());
            paintMode = true;
            update();
        }

    }
    QGraphicsView::mousePressEvent(event);
}


void PhotoViewer::mouseMoveEvent(QMouseEvent* event)
{

    if(hasPhoto)
    {
        if (paintMode)
        {
            QPainter p(&currentMap);
            p.setPen(pen);
            p.drawLine(mapToScene(lastPos), mapToScene(event->pos()));
            QPointF endPoint = mapToScene(event->pos());
            path.moveTo(mapToScene(lastPos));
            path.lineTo(endPoint);
            lastPos = event->pos();
            brush->setPath(path);
        }
        else
        {
            QRect viewrect = viewport()->rect();
            QPointF point = this->mapToScene(viewrect.width()/2.0, round((viewrect.height()+1)/2.0));
            emit zoomed(zoom, 1, point);
        }

    }
    QGraphicsView::mouseMoveEvent(event);


}

void PhotoViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if(hasPhoto)
    {
        if (paintMode)
        {
            if (viewerType == PHOTO_VIEWER_TYPE)
            {
                this->labels->setPixmap(directPixmaps(labels->pixmap(),currentMap, brushType));
            }
            else if (viewerType == MASK_VIEWER_TYPE)
                this->labels->setPixmap(maskPixmaps(labels->pixmap(),currentMap, imMask, brushType));
            else
                qInfo("Invalid viewer type");
            paintMode = false;
            resetBrush(brushSize, capStyle);
            update();
        }



    }
   QGraphicsView::mouseReleaseEvent(event);


}
