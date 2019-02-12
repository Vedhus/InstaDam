#include "photoviewer.h"
#include <math.h>

PhotoViewer::PhotoViewer(QWidget *parent):QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    photo = new QGraphicsPixmapItem();
    hasPhoto = false;
    scene->addItem(photo);
    setScene(scene);
    zoom = 1;

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
    this->photo->setPixmap(pixmap);
    hasPhoto = true;
    this->fitInView(true);
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
        //this->zoom = 0;
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

//def setPanMode(self):
//    self.setDragMode(QtWidgets.QGraphicsView.ScrollHandDrag)

//def setBrushMode(self, brushType):
//    if self.imageSet:
//        self.setDragMode(QtWidgets.QGraphicsView.NoDrag)
//        self.brushType = brushType
//        self.resetBrush(self._brushSize, self._capStyle)
//        self.update()

//def setBrushShape(self, brushShape = 'Round', rect = None):
//    if self.imageSet:
//        self.setDragMode(QtWidgets.QGraphicsView.NoDrag)
//    if brushShape == 'Round':
//        self.resetBrush(self._brushSize, Qt.RoundCap)
//    else:
//        self.resetBrush(self._brushSize, Qt.SquareCap)
//def mousePressEvent(self, event):

//    if self.imageSet:
//        if self.photo.isUnderMouse():

//            self.photoClicked.emit(QtCore.QPoint(event.pos()))
//            #self.update()

//        if self.dragMode() == QtWidgets.QGraphicsView.NoDrag:


//            self.resetBrush(self._brushSize, self._capStyle)

//            self.lastPos = event.pos() #self.mapToScene(event.pos())



//            self.currentMap = self.labelsTemp.pixmap()
//            if self.viewerType == MASK_VIEW:
//                self.currentMap.setMask(self.imMask.mask())
//            self.paintMode = True
//            self.update()

//    super(PhotoViewer, self).mousePressEvent(event) #       print('MPE {0}s\n'.format(t-time.time()))

//def mouseMoveEvent(self, event):
//    if self.imageSet:

//        if self.lastPos and self.paintMode:


//            p = QPainter(self.currentMap)
//            p.setPen(self.pen)
//            p.drawLine(self.mapToScene(self.lastPos), self.mapToScene(event.pos()))

//            self.end = self.mapToScene(event.pos())
//            self.path.moveTo(self.mapToScene(self.lastPos))
//            self.path.lineTo(self.end)
//            self.lastPos = event.pos()
//            self.brush.setPath(self.path)
//        else:
//            self.viewrect = self.viewport().rect()
//            point = self.mapToScene(self.viewrect.width()/2.0, np.round((self.viewrect.height()+1)/2.0))
//            self.zoomed.emit(self._zoom, 1, point)

//    super(PhotoViewer, self).mouseMoveEvent(event)
//def mouseReleaseEvent(self, event):
//    if self.imageSet:
//        if self.paintMode:
//            if self.viewerType == IM_VIEW:
//                self.labels.setPixmap(directPixmaps(self.labels.pixmap(),self.currentMap, self.brushType))
//            elif self.viewerType == MASK_VIEW:
//                self.labels.setPixmap(maskPixmaps(self.labels.pixmap(),self.currentMap, self.imMask, self.brushType))
//            else:
//                assert('Invalid viewer type')
//            self.paintMode = False
//            self.resetBrush(self._brushSize, self._capStyle)
//            self.update()
//    super(PhotoViewer, self).mouseReleaseEvent(event)
