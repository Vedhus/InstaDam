#include "photoviewer.h"
#include "pixmapops.h"
#include <math.h>


PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent)
{
    B = new paintBrush;


    viewerType = PHOTO_VIEWER_TYPE;
    scene = new QGraphicsScene(this);
    photo = new QGraphicsPixmapItem();
    labelsTemp = new QGraphicsPixmapItem();
    B->selectedLabelClass = 0;
    //labels = new QGraphicsPixmapItem();
    filterIm = new QGraphicsPixmapItem();
    hasPhoto = false;

    labelsTemp->setOpacity(0);

    brush = new QGraphicsPathItem();

    scene->addItem(labelsTemp);
    scene->addItem(photo);
    scene->addItem(brush);


    setScene(scene);
    zoom = 1;


    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setDragMode(QGraphicsView::NoDrag);
    resetBrush();
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

void PhotoViewer::setPhotoFromFile(QString filename, QVector<QString> labelName)
{


    for (int i = 0; i<labelItems.length(); i++)
    {
        scene->removeItem(labelItems[i]);
        delete labelItems[i];
    }
    labelItems.clear();
    qInfo("The number of labels is %d",labelItems.length());

    //QGraphicsItemGroup * labels;
    QColor whiteColor = QColor(0,0,0,0);
    QPixmap pixmap = QPixmap(filename);
    cvImage = cv::imread(filename.toLocal8Bit().constData(), CV_LOAD_IMAGE_COLOR);


    QVectorIterator<QString> itLabels(labelName);
    int i = 0;

//    scene->clear();

//    scene->addItem(labelsTemp);
//    scene->addItem(photo);
//    scene->addItem(brush);
    while(itLabels.hasNext())
    {
        QString labelFile = itLabels.next();
        qInfo("I will open the labels!");
        qInfo(labelFile.toUtf8().constData());

        QPixmap labelMap;
        if (QFileInfo(labelFile).isFile())
        {

            labelMap = QPixmap(labelFile);

        }
        else
        {
            labelMap = QPixmap(pixmap.size());
            labelMap.fill(whiteColor);

        }

        QGraphicsPixmapItem *labtemp = new QGraphicsPixmapItem();
        labtemp->setPixmap(labelMap);
        labtemp->setOpacity(0.5);
        //QGraphicsItem *labItem = qgraphicsitem_cast<QGraphicsItem*>(labtemp);
        labelItems.append(labtemp);
        //labelGroup.append(labItem);
        scene->addItem(labelItems[i]);

        i+=1;
    }
    //labelGroup = qgraphicsitem_cast<QGraphicsItemGroup *>(labelItems)
    //labels = scene->createItemGroup(labelGroup);

    QPixmap white_temp = QPixmap(pixmap.size());
    white_temp.fill(whiteColor);
    setPhoto(pixmap, white_temp);


}






//void PhotoViewer::setPhotoFromPixmap(QPixmap px, QPixmap lpx)
//{
//    cvImage = QPixmap2Mat(px);

//    QPixmap white_temp = QPixmap(px.size());
//    QColor whiteColor = QColor(0,0,0,0);
//    white_temp.fill(whiteColor);
//    setPhoto(px, lpx, white_temp);

//}

void PhotoViewer::setPhoto(QPixmap pixmap, QPixmap labelsTempPixmap)
{

    cv::resize(cvImage, cvThumb,cv::Size(200,200) , CV_INTER_LINEAR);



    if (pixmap.isNull() == 0)
    {
        if (this->viewerType == PHOTO_VIEWER_TYPE)
        {
                this->photo->setPixmap(pixmap);
                //this->labels->setPixmap(labelsPixmap);
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


void PhotoViewer::setPanMode()
{
    this->B->brushType = PAN;
    resetBrush();

}

void PhotoViewer::setEraserMode()
{
    this->B->brushType = ERASER;
    this->B->color = QColor(255,255,255,125);
    resetBrush();

}

void PhotoViewer::resetBrushDrag()
{
    switch(B->brushType)
    {
    case ERASER:
        setDragMode(QGraphicsView::NoDrag);
        break;
    case PAN:
       setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    case PAINTBRUSH:
        setDragMode(QGraphicsView::NoDrag);
        break;

    }
}


void PhotoViewer::resetBrush()
{

    //emit brushResetting();
    resetBrushDrag();

    qInfo("Hand mode changed");
    scene->removeItem(brush);
    delete brush;
    qInfo("brush deleted");
    pen = QPen();

    pen.setColor(B->color);
    pen.setWidth(B->size);

    pen.setCapStyle(B->capStyle);
    path = QPainterPath();
    brush = new QGraphicsPathItem();
    brush->setPen(pen);
    scene->addItem(brush);
    paintMode = false;
    qInfo("function Changed");




}

void PhotoViewer::labelChanged(int i, QColor col)
{
    if (hasPhoto)
    {
        B->selectedLabelClass = i;
        B->color = col;

        this->B->brushType = PAINTBRUSH;
        resetBrush();

    }

}
void  PhotoViewer::brushSizeChanged(int size)
{
    B->size = size;
    resetBrush();
}

void PhotoViewer::opacityChanged(int i, int op)
{
    if (hasPhoto)
    {
    labelItems[i]->setOpacity(op/100.0);
    }
}

void PhotoViewer::setBrushMode(Qt::PenCapStyle cap)
{
    if (this->hasPhoto)
    {

        this->B->capStyle = cap;
        resetBrush();

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
    qInfo("Before brush drag reset");
    resetBrushDrag();
    if (hasPhoto)
    {
        if (photo->isUnderMouse())
        {
            emit photoClicked(QPoint(event->pos()));
        }
        if (dragMode() == QGraphicsView::NoDrag)
        {
            resetBrush();
            lastPos = event->pos();
            qInfo("Before labelsTemp");
            currentMap = labelsTemp->pixmap();
            if (viewerType == MASK_VIEWER_TYPE)
                currentMap.setMask(imMask.mask());

            paintMode = true;
            update();
            qInfo("after update");
        }

    }

    QGraphicsView::mousePressEvent(event);
}


void PhotoViewer::mouseMoveEvent(QMouseEvent* event)
{

    resetBrushDrag();
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
            QPointF point = this->mapToScene(round(viewrect.width()/2.0), round(viewrect.height()/2.0));
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
                this->labelItems[B->selectedLabelClass]->setPixmap(directPixmaps(labelItems[B->selectedLabelClass]->pixmap(),currentMap, B->brushType));
            }
            else if (viewerType == MASK_VIEWER_TYPE)
                this->labelItems[B->selectedLabelClass]->setPixmap(maskPixmaps(labelItems[B->selectedLabelClass]->pixmap(),currentMap, imMask, B->brushType));
            else
                qInfo("Invalid viewer type");
            paintMode = false;
            resetBrush();
            update();
        }



    }
   QGraphicsView::mouseReleaseEvent(event);


}
