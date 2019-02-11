#include "photoviewer.h"

PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    photo = new QGraphicsPixmapItem();
    hasPhoto = false;
    scene->addItem(photo);
    setScene(scene);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
}

void PhotoViewer::testPixmap()
{
    QPixmap pixmap2 =  QPixmap("G:/My Drive/SSTL/DCNN/Labellers/Python Labeller/InstaDamPublic/Qt/InstaDam/im2.JPG");
    this->photo->setPixmap(pixmap2);
}

void PhotoViewer::setPhoto(QPixmap pixmap)
{
//    TODO: Add error checking
    this->photo->setPixmap(pixmap);
}


//void PhotoViewer::fitInView(bool scale = true)
//{
//    QRectF rect = QRectF(photo->pixmap().rect());
//    if (!rect.isNull())
//    {
//        setSceneRect(rect);
//        if (this->hasPhoto)
//        {
//            QRectF unity = transform().mapRect(QRectF(0,0,1,1));
//            this->scale(1.0/unity.width(), 1.0/unity.height());
//            QRect viewrect = this->viewport().rect();
//            scenerect = transform().mapRect(QRect);
//        }
//    }

//}
