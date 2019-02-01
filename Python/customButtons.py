# -*- coding: utf-8 -*-
"""
Created on Mon Jan 28 14:37:41 2019

@author: Vedhus
"""

# Import filter classes
from filters import *

# Import pixmap ops
from pixmapops import *


        
class PicButton(QtWidgets.QAbstractButton):
    checked = QtCore.pyqtSignal(object, QtCore.QRect)
    def __init__(self, name, viewer = None, parent=None, w = 200, h = 200):
        self.w = w
        self.h = h
        super(PicButton, self).__init__(parent)
        self.filter = name
        pixmap = testPixmap(255,0,0)
        self.resetPixmaps(pixmap)        
        self.pixmap = pixmap
        self.setCheckable(True)
        self.setChecked(False)        
        self.pressed.connect(self.update)
        self.released.connect(self.blank)

    def resetPixmaps(self, pixmap):
        hoverOverlayPixmap = self.addText(testPixmap(20,125,200,128), self.filter)
        pressedOverlayPixmap = self.addText(testPixmap(30,180,200,128), self.filter)
        self.pixmap = pixmap
        self.pixmap_hover = joinPixmaps(self.pixmap, hoverOverlayPixmap)
        self.pixmap_pressed = joinPixmaps(self.pixmap, pressedOverlayPixmap)
        
    def blank(self):
        self.setChecked(True)
    
    def addText(self, pixmap, name):
        painter = QPainter()
        font = painter.font()
        font.setPointSize(24)
        painter.begin(pixmap)
        position  = QtCore.QRect(0, 0, self.w, self.h)
        painter.setFont(font);
        painter.drawText(position, Qt.AlignCenter, name);
        painter.end()
        return pixmap
        
    def paintEvent(self, event):
        
        pix = self.pixmap_hover if self.underMouse() else self.pixmap        
        if self.isChecked():
            self.checked.emit( self.filter, event.rect())    
            pix = self.pixmap_pressed
        size = self.size()
        scaledPix = pix.scaledToHeight(size.height(), Qt.SmoothTransformation)
        
        # start painting the label from left upper corner
        point = QtCore.QPoint(0,0)
        point.setX((size.width() - scaledPix.width())/2)
        point.setY((size.height() - scaledPix.height())/2)

        painter = QPainter(self)
        
        painter.drawPixmap(point, scaledPix)
        
    def otherBoxChecked(self, func, rect):
        if self.isChecked():
            pix = self.pixmap
            painter = QPainter(self)
            painter.drawPixmap(rect, pix)
            self.setChecked(False)
            
        
    def enterEvent(self, event):
        self.update()

    def leaveEvent(self, event):
        self.update()

    def sizeHint(self):
        return QtCore.QSize(self.w, self.h)
    

class LabelButton(QtWidgets.QAbstractButton):
    checked = QtCore.pyqtSignal(object, QtCore.QRect)

    def __init__(self, name, color, viewer = None, parent=None, w = 60, h = 40):
        self.w = w
        self.h = h
        super(LabelButton, self).__init__(parent)
        self.name = name

        if color[0]+color[1]+color[2] > 255*3/2:
            self.textColor = QtGui.QColor(0,0,0,255)
        else:
            self.textColor = QtGui.QColor(255,255,255,255)
        pixmap = self.addText( testPixmap(color[0], color[1], color[2], 255, (w,h)), self.name)
        self.resetPixmaps(pixmap)        
        self.pixmap = pixmap
        self.setCheckable(True)
        self.setChecked(False)        
        self.pressed.connect(self.update)
        self.released.connect(self.blank)

    def resetPixmaps(self, pixmap):
        hoverOverlayPixmap = testPixmap(200,125,200,128,(self.w,self.h))
        pressedOverlayPixmap = testPixmap(200,180,200,128,(self.w,self.h))
        self.pixmap = pixmap
        self.pixmap_hover = joinPixmaps(self.pixmap, hoverOverlayPixmap)
        self.pixmap_pressed = joinPixmaps(self.pixmap, pressedOverlayPixmap)
        
    def blank(self):
        self.setChecked(True)
    
    def addText(self, pixmap, name):
        painter = QPainter()
        self.pen = QPen()
        self.pen.setColor(self.textColor)

        font = painter.font()
        font.setPointSize(9);
        
        painter.begin(pixmap)
        position  = QtCore.QRect(0, 0, self.w, self.h)
        painter.setFont(font);
        painter.setPen(self.pen)
        painter.drawText(position, Qt.AlignCenter, name);
        painter.end()
        return pixmap
    
        
    def paintEvent(self, event):
        
        pix = self.pixmap_hover if self.underMouse() else self.pixmap        
        if self.isChecked():

            self.checked.emit( self.name, event.rect())    
            pix = self.pixmap_pressed
        size = self.size()
        scaledPix = pix.scaledToHeight(size.height(), Qt.SmoothTransformation)
        
        # start painting the label from left upper corner
        point = QtCore.QPoint(0,0)
        point.setX((size.width() - scaledPix.width())/2)
        point.setY((size.height() - scaledPix.height())/2)

        painter = QPainter(self)
        
        painter.drawPixmap(point, scaledPix)
        
    def otherBoxChecked(self, func, rect):
        if self.isChecked():
            pix = self.pixmap
            painter = QPainter(self)
            painter.drawPixmap(rect, pix)
            self.setChecked(False)
            
        
    def enterEvent(self, event):
        self.update()

    def leaveEvent(self, event):
        self.update()

    def sizeHint(self):
        return QtCore.QSize(self.w, self.h)
    
class ShapeButton(LabelButton):
        def __init__(self, name, color, viewer = None, parent=None, w = 60, h = 40):
            super().__init__(name, color, viewer, parent, 60, 40)

    

