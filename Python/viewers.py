# -*- coding: utf-8 -*-
"""
Created on Mon Jan 28 14:39:04 2019

@author: Vedhus
"""

# Photo viewer is a custom class created based on QGraphicsView. It displays the 
# the image on the left where the user can directly make labels
# Mask viewer inherits from photo viewer and is used to display the selected mask
# Some pixmaps are shared between the two viewers.

# Import filter classes
from filters import *

# Import custom buttons
from customButtons import *

# Import main window class
from windows import *

# Import pixmap ops
from pixmapops import *

class PhotoViewer(QtWidgets.QGraphicsView):
    photoClicked = QtCore.pyqtSignal(QtCore.QPoint)
    zoomed = QtCore.pyqtSignal(int, float, QtCore.QPointF)
    loadedPhoto = QtCore.pyqtSignal(bool)
    

    def __init__(self, parent, colors, viewerType = IM_VIEW):
        
        super(PhotoViewer, self).__init__(parent)
        self.colors = colors
        self.name = 'PhotoViewer'
        self._zoom = 0
        self._empty = True
        self.scene = QtWidgets.QGraphicsScene(self)
        self.imMask = None
#        self.mask = QPixmap()
#        self.mask.fill( QtGui.QColor(0,0,0,0))
        self.photo = QtWidgets.QGraphicsPixmapItem()
        self.labels = QtWidgets.QGraphicsPixmapItem()
        self.labelsTemp = QtWidgets.QGraphicsPixmapItem()
        self.labelsTemp.setOpacity(0)
        self.filterIm = QtWidgets.QGraphicsPixmapItem()
        self.scene.addItem(self.labelsTemp)
        self.labels.setOpacity(1)
        self.scene.addItem(self.photo)

        #self.scene.addItem(self.filterIm)
        self.scene.addItem(self.labels)
        #self.scene.addItem(self.filterIm)
        
        self.viewerType = viewerType
        self.setScene(self.scene)
        self.setTransformationAnchor(QtWidgets.QGraphicsView.AnchorUnderMouse)
        self.setResizeAnchor(QtWidgets.QGraphicsView.AnchorUnderMouse)
        self.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.setBackgroundBrush(QtGui.QBrush(QtGui.QColor(30, 30, 30)))
        self.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.lastPos = None
        self.brush = QGraphicsPathItem()
        self.scene.addItem(self.brush)
        self.brushType = ERASER
        self._brushSize = 10
        self._capStyle = Qt.RoundCap
        self.resetBrush(self._brushSize)
        self.cvImage = None
        self.imageSet = False
        
        self.paintMode = False
        self.maskObjs = {}
        self.selectedFilter = None
        self.parent = parent
        
    def resetBrush(self, size = 10, capStyle = None):
        self._brushSize = size
        self.scene.removeItem(self.brush)
        self.pen = QPen()
        color = self.colors[self.brushType]
        self.pen.setColor(QtGui.QColor(color[0], color[1], color[2], color[3]))
        self.pen.setWidth(size)
        if capStyle== None:
            self._capStyle = Qt.RoundCap
        else:
            self._capStyle = capStyle
        self.pen.setCapStyle(self._capStyle)
        self.path = QPainterPath()        
        self.brush = QGraphicsPathItem()
        self.brush.setPen(self.pen)
        self.scene.addItem(self.brush)
        self.paintMode = False        
        
    def hasPhoto(self):
        return not self._empty

    def fitInView(self, scale=True):
        rect = QtCore.QRectF(self.photo.pixmap().rect())
        if not rect.isNull():
            self.setSceneRect(rect)
            if self.hasPhoto():
                unity = self.transform().mapRect(QtCore.QRectF(0, 0, 1, 1))
                self.scale(1 / unity.width(), 1 / unity.height())
                self.viewrect = self.viewport().rect()
                scenerect = self.transform().mapRect(rect)
                factor = min(self.viewrect.width() / scenerect.width(),
                             self.viewrect.height() / scenerect.height())
                self.scale(factor, factor)
            self._zoom = 0

    def setPhoto(self, pixmap=None):
        self._zoom = 0
        white = QtGui.QPixmap(pixmap.size())
        white2 = QtGui.QPixmap(pixmap.size())
        whiteColor = QtGui.QColor(0,0,0,0)
        white2.fill(whiteColor)   
        white.fill(whiteColor) 
        if pixmap and not pixmap.isNull():            
            self._empty = False
            self.setDragMode(QtWidgets.QGraphicsView.ScrollHandDrag)
            
            white = QtGui.QPixmap(pixmap.size())
            white2 = QtGui.QPixmap(pixmap.size())
            whiteColor = QtGui.QColor(0,0,0,0)
            white2.fill(whiteColor)
            #whiteColor.setAlpha(0)
            white.fill(whiteColor) 
            if self.viewerType == IM_VIEW:
                self.photo.setPixmap(pixmap)          
                self.labels.setPixmap(white) 
                self.labelsTemp.setPixmap(white2)
            self.imageSet = True
            self.setImMask()
            self.setPicButtonThumbnails()
        else:
            self._empty = True
            self.setDragMode(QtWidgets.QGraphicsView.NoDrag)
            self.photo.setPixmap(QtGui.QPixmap())
            self.labels.setPixmap(white)
            self.labelsTemp.setPixmap(white2)
        self.fitInView()
        self.viewrect = self.viewport().rect()
        self.zoomed.emit( self._zoom, 1,  self.mapToScene(self.viewrect.width()/2.0, np.round((self.viewrect.height()+1)/2.0)))
        
        self.loadedPhoto.emit(True)
    
    def setImMask(self,  maskType = 'Canny', rect = None):
        #if self.cvImage:
        if self.imageSet == True:
            self.selectedFilter = maskType
            self.maskObjs[maskType].filt(self.cvImage)            
            filterIm, filterMask= self.maskObjs[maskType].im2pixmap()
            self.filterIm.setPixmap(filterIm)
            self.imMask = filterMask
    
    def getThumbPixmap(self, maskType):
        self.maskObjs[maskType].filtThumb(self.thumbImage)
        self.thumbFilterIm, _ = self.maskObjs[maskType].thumb2pixmap()
        return self.thumbFilterIm
    def setPicButtonThumbnails(self):
        for button in self.parent.maskButtons:
            thumbnail= self.getThumbPixmap(button.filter)
            button.resetPixmaps(thumbnail)
            
    def getMaskFromMaskView(self, filterIm, filterMask, maskType):
            self.selectedFilter = maskType
            self.filterIm.setPixmap(filterIm)
            self.imMask = filterMask

    
    def modifyMaskThreshold(self,  val = None):
                
        if self.imageSet == True:
            filterIm, filterMask= self.maskObjs[self.selectedFilter].im2pixmap()
            self.filterIm.setPixmap(filterIm)
            self.imMask = filterMask
            
    
    def zoomedInADifferentView(self, zoom, factor, point):
        self._zoom = zoom
        
        if self._zoom > 0:   
            #self.centerOn(point)
            self.scale(factor, factor)
            
        elif self._zoom == 0:
            self.fitInView()
        else:
            self._zoom = 0
        #self.setSceneRect(rect)
        self.centerOn(point)
        

    def wheelEvent(self, event):
        factor = 1
        self.viewrect = self.viewport().rect()
        if self.hasPhoto():
            if event.angleDelta().y() > 0:
                factor = 1.25
                self._zoom += 1
            else:
                factor = 0.8
                self._zoom -= 1
                
            if self._zoom > 0:
                
                self.scale(factor, factor)
                self.centerOn(self.mapToScene(self.viewrect.width()/2, np.round((self.viewrect.height()+1)/2.0)))
                
            elif self._zoom == 0:
                self.fitInView()
            else:
                
                self._zoom = 0
                
            point = self.mapToScene(self.viewrect.width()/2.0, np.round((self.viewrect.height()+1)/2.0))
            self.zoomed.emit(self._zoom, factor, point)

    def setPanMode(self):
        self.setDragMode(QtWidgets.QGraphicsView.ScrollHandDrag)
        
    def setBrushMode(self, brushType):
        if self.imageSet:
            self.setDragMode(QtWidgets.QGraphicsView.NoDrag)
            self.brushType = brushType
            self.resetBrush(self._brushSize, self._capStyle)
            self.update()
            
    def setBrushShape(self, brushShape = 'Round', rect = None):
        if self.imageSet:
            self.setDragMode(QtWidgets.QGraphicsView.NoDrag)
        if brushShape == 'Round':
            self.resetBrush(self._brushSize, Qt.RoundCap)
        else:
            self.resetBrush(self._brushSize, Qt.SquareCap)
    def mousePressEvent(self, event): 

        if self.imageSet:
            if self.photo.isUnderMouse():
                
                self.photoClicked.emit(QtCore.QPoint(event.pos()))  
                #self.update()
                
            if self.dragMode() == QtWidgets.QGraphicsView.NoDrag:
                
                
                self.resetBrush(self._brushSize, self._capStyle)

                self.lastPos = event.pos() #self.mapToScene(event.pos())

                
                
                self.currentMap = self.labelsTemp.pixmap()
                if self.viewerType == MASK_VIEW:
                    self.currentMap.setMask(self.imMask.mask())
                self.paintMode = True
                self.update()

        super(PhotoViewer, self).mousePressEvent(event) #       print('MPE {0}s\n'.format(t-time.time()))
    
    def mouseMoveEvent(self, event):
        if self.imageSet:
            
            if self.lastPos and self.paintMode:
                
                
                p = QPainter(self.currentMap)
                p.setPen(self.pen)
                p.drawLine(self.mapToScene(self.lastPos), self.mapToScene(event.pos()))          
    
                self.end = self.mapToScene(event.pos())
                self.path.moveTo(self.mapToScene(self.lastPos))
                self.path.lineTo(self.end)
                self.lastPos = event.pos()
                self.brush.setPath(self.path)
            else:
                self.viewrect = self.viewport().rect()
                point = self.mapToScene(self.viewrect.width()/2.0, np.round((self.viewrect.height()+1)/2.0))
                self.zoomed.emit(self._zoom, 1, point)
            
        super(PhotoViewer, self).mouseMoveEvent(event)
    def mouseReleaseEvent(self, event):
        if self.imageSet:
            if self.paintMode:
                if self.viewerType == IM_VIEW:
                    self.labels.setPixmap(directPixmaps(self.labels.pixmap(),self.currentMap, self.brushType))
                elif self.viewerType == MASK_VIEW:
                    self.labels.setPixmap(maskPixmaps(self.labels.pixmap(),self.currentMap, self.imMask, self.brushType))
                else:
                    assert('Invalid viewer type')
                self.paintMode = False
                self.resetBrush(self._brushSize, self._capStyle)
                self.update()
        super(PhotoViewer, self).mouseReleaseEvent(event)


class MaskViewer(PhotoViewer):
    photoClicked = QtCore.pyqtSignal(QtCore.QPoint)

    def __init__(self, parent, viewer, viewerType = MASK_VIEW):
        super(MaskViewer, self).__init__(parent, viewer.colors)
        self.name = 'MaskViewer'
        self._zoom = 0
        self._empty = True
        self.photoViewer = viewer
        self.viewerType = viewerType
        self.photo = viewer.filterIm
        self.scene = QtWidgets.QGraphicsScene(self)
        self.labels = viewer.labels
        #self.labels.setOpacity(0.5)
        self.labelsTemp = viewer.labelsTemp
        self.imMask = viewer.imMask
        #self.imMask.setOpacity(0)
        self.scene.addItem(self.imMask)
        
        self.scene.addItem(self.photo)
        self.setScene(self.scene)
        self._empty = False
    def setImMask(self,  maskType = 'Canny', rect = None):
        #if self.cvImage:
        
        if self.imageSet == True:
           
            t = time.time()
            self.selectedFilter = maskType
            self.maskObjs[maskType].filt(self.cvImage)    
            print('Filitered Image {0}'.format(t-time.time()))
            
            t = time.time()            
            self.filterIm, self.filterMask= self.maskObjs[maskType].im2pixmap()
            print('Converted to pixmap {0}'.format(t-time.time()))
            
            t = time.time()
            self.photo.setPixmap(self.filterIm)
            print('Set Pixmap {0}'.format(t-time.time()))
            
            self.imMask = self.filterMask

        
    def modifyMaskFilter(self, val = None):
    #if self.cvImage:
        self.setImMask(maskType = self.selectedFilter)
        self.photoViewer.getMaskFromMaskView(self.filterIm, self.filterMask, self.selectedFilter)
    
            
    def modifyMaskThreshold(self,  val = None):
                
        if self.imageSet == True:
            filterIm, filterMask= self.maskObjs[self.selectedFilter].im2pixmap()
            self.photo.setPixmap(filterIm)
            self.imMask = filterMask
            

