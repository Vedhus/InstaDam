# -*- coding: utf-8 -*-
"""
Created on Tu Jan 01 14:26:23 2019

@author: Vedhus
"""
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPainter, QPen, QPainterPath
from PyQt5.QtWidgets import QGraphicsPathItem

# Math and CV packages
import cv2
import numpy as np
from skimage.filters import frangi
import time

## Global constants         
IM_VIEW = 1
MASK_VIEW = 2
SLIDER_MAX = 1000
SLIDER_MIN = 0
ERASER = 0

# Filter properties
ANY = 0
ODD_ONLY = 1
EVEN_ONLY = 2

  
def im2pixmap(imFilter, image):
    im = imFilter(image)

    im[im>128] = 255
    im[im<=128] = 0
    im[im==255] = 255
   
    height, width = im.shape
        
    bytesPerLine = width

    qImg = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Grayscale8)
    qAlpha = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Alpha8)
    return QtGui.QPixmap(qImg), QtGui.QPixmap(qAlpha)

class filterProperty(QtCore.QObject):
    valchanged = QtCore.pyqtSignal(int) 
    def __init__(self, name, btnType, propMax = None, propVal = None, propMin = None,\
                 dataType = int, evenOdd = ANY, thof  = 'Filter', signalBool = False):
        super().__init__()
        self.name = name
        self.btnType = btnType
        self.val = propVal
        self.max = propMax
        self.min = propMin
        self.type = dataType
        self.evenOdd = evenOdd
        self.thof = thof
        self.signalBool = signalBool
        
    def sliderAssign(self, sliderVal):
        print(sliderVal)
        val = self.type(sliderVal)
        if self.evenOdd == ODD_ONLY:
            val = int(val/2)*2+1
        elif self.evenOdd == EVEN_ONLY:
            val = int(val/2)*2
        self.val = val
        print(self.val)
    
    def valForSlider(self):
        return self.val

class Canny:
    def __init__(self):
        self.name = 'Canny'
        self.properties = {}
        self.properties['X'] = filterProperty('Filter X','slider',propMin = 3, \
                       propVal = 101, propMax = 501, dataType = int, evenOdd = ODD_ONLY, thof = 'Filter')
        self.properties['Y'] = filterProperty('Filter Y','slider',propMin = 3, \
                       propVal = 101, propMax = 501, dataType = int, evenOdd = ODD_ONLY, thof =  'Filter')
        self.properties['TH'] = filterProperty('Threshold','slider',propMin = 0,\
                       propVal = 128, propMax = 255, dataType = int, evenOdd = ANY, thof =  'Threshold')
        self.properties['I'] = filterProperty('Invert','checkBox',propMin = 0,\
               propVal = 0, propMax = 1, dataType = int, evenOdd = ANY,  thof = 'Threshold')
        self.img = None
        self.edges = None
        
    def filterFunc(self, image):
        return cv2.Canny(image, self.properties['X'].val,self.properties['Y'].val)
    
    def filt(self, image):
        self.img = image
        self.edges = self.filterFunc(image)
        return(self.edges)
        
    def filtThumb(self, thumb):
        self.thumb = thumb
        self.thumbEdges = self.filterFunc(self.thumb)
        return(self.thumbEdges)
    
    def updateX(self,  sliderVal = 5):        
        self.properties['X'].sliderAssign(sliderVal)
        
    def updateY(self,  sliderVal = 5):
        self.properties['Y'].sliderAssign(sliderVal)
    def updateTH(self,  sliderVal = 5):
        self.properties['TH'].sliderAssign(sliderVal)
    def updateI(self, val = 0):

        self.properties['I'].val = int(val)
    def im2pixmap(self):

            
        im = self.edges.copy()
        self.th = self.properties['TH'].val
        im[im>self.th] = 255
        im[im<=self.th] = 0
        im[im==self.th] = 255
        print(self.properties['I'].val)
        if self.properties['I'].val==2:
            im = 255-im       
        height, width = im.shape
            
        bytesPerLine = width
    
        qImg = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Grayscale8)
        qAlpha = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Alpha8)
        return QtGui.QPixmap(qImg), QtGui.QPixmap(qAlpha)
    
    def thumb2pixmap(self):
        im = self.thumbEdges.copy()
        self.th = self.properties['TH'].val
        im[im>self.th] = 255
        im[im<=self.th] = 0
        im[im==self.th] = 255
        print(self.properties['I'].val)
        if self.properties['I'].val==2:
            im = 255-im       
        height, width = im.shape
            
        bytesPerLine = width
    
        qImg = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Grayscale8)
        qAlpha = QtGui.QImage(im.data, width, height, bytesPerLine, QtGui.QImage.Format_Alpha8)
        return QtGui.QPixmap(qImg), QtGui.QPixmap(qAlpha)
        
        
class Threshold(Canny):
    def __init__(self):
        super().__init__()
        self.name = 'Threshold'
        self.properties ={}
        self.properties['TH'] = filterProperty('Threshold','slider',propMin = 0,\
                       propVal = 128, propMax = 255, dataType = int, evenOdd = ANY, thof =  'Threshold')
        self.properties['I'] = filterProperty('Invert','checkBox',propMin = 0,\
               propVal = 0, propMax = 1, dataType = int, evenOdd = ANY,  thof = 'Threshold')
    def filterFunc(self, image):
        return cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    
class threshold1(Canny):
    def __init__(self):
        super().__init__()
        self.name = 'threshold1'
        self.properties ={}
        self.properties['TH'] = filterProperty('Threshold','slider',propMin = 0,\
                       propVal = 128, propMax = 255, dataType = int, evenOdd = ANY, thof =  'Threshold')
        self.properties['I'] = filterProperty('Invert','checkBox',propMin = 0,\
               propVal = 0, propMax = 1, dataType = int, evenOdd = ANY,  thof = 'Threshold')
    def filterFunc(self, image):
        return cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    
class threshold2(Canny):
    def __init__(self):
        super().__init__()
        self.name = 'threshold2'
        self.properties ={}
        self.properties['TH'] = filterProperty('Threshold','slider',propMin = 0,\
                       propVal = 128, propMax = 255, dataType = int, evenOdd = ANY, thof =  'Threshold')
        self.properties['I'] = filterProperty('Invert','checkBox',propMin = 0,\
               propVal = 0, propMax = 1, dataType = int, evenOdd = ANY,  thof = 'Threshold')
    def filterFunc(self, image):
        return cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

        
        
class Blur(Canny):
    def __init__(self):
        super().__init__()
        self.name = 'Blur'
        self.properties['SD'] = filterProperty('SD','slider',propMin = 1, \
                       propVal = 3, propMax = 55, dataType = int, evenOdd = ANY)
    def filterFunc(self, image):
        return cv2.cvtColor(cv2.GaussianBlur(image,(self.properties['X'].val,self.properties['Y'].val),\
                                                        self.properties['SD'].val), cv2.COLOR_RGB2GRAY)
        
        
    def updateSD(self,  sliderVal = 5):
        self.properties['SD'].sliderAssign(sliderVal)

     
class Frangi(Canny):
    def __init__(self):
        super().__init__()
        self.name = 'Frangi'
        self.properties ={}
        self.properties['SMin'] = filterProperty('Min Scale','slider',propMin = 1,\
                       propVal = 2, propMax = 60, dataType = int, evenOdd = ANY, \
                       thof =  'Filter', signalBool = True)
        self.properties['SMax'] = filterProperty('Max Scale','slider',propMin = 2,\
                       propVal = 10, propMax = 80, dataType = int, evenOdd = ANY, \
                       thof =  'Filter', signalBool = True)
        self.properties['NScales'] = filterProperty('N Scale','slider',propMin = 1,\
                       propVal = 3, propMax = 80, dataType = int, evenOdd = ANY, thof =  'Filter')
        self.properties['TH'] = filterProperty('Threshold','slider',propMin = 0,\
                       propVal = 128, propMax = 255, dataType = int, evenOdd = ANY, thof =  'Threshold')
        self.properties['I'] = filterProperty('Invert','checkBox',propMin = 0,\
               propVal = 0, propMax = 1, dataType = int, evenOdd = ANY,  thof = 'Threshold')
    def filterFunc(self, image):
        NScales = int((self.properties['SMax'].val-self.properties['SMin'].val)/float(self.properties['NScales'].val))
        NScales= np.max([NScales,1])
        edges = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
        edges = frangi(edges, scale_range = (self.properties['SMin'].val,self.properties['SMax'].val),\
                            scale_step = NScales)
        
        return cv2.convertScaleAbs(255*edges/np.max(edges))
        
        
    
    def updateSMin(self,  sliderVal = 5):        
        self.properties['SMin'].sliderAssign(sliderVal)
        if self.properties['SMax'].val <=self.properties['SMin'].val:
            self.properties['SMax'].val = self.properties['SMin'].val+1
        self.properties['SMax'].valchanged.emit(self.properties['SMax'].val)
    def updateSMax(self,  sliderVal = 5):        
        self.properties['SMax'].sliderAssign(sliderVal)
        if self.properties['SMax'].val <=self.properties['SMin'].val:
            self.properties['SMin'].val = self.properties['SMin'].val-1
        self.properties['SMin'].valchanged.emit(self.properties['SMin'].val)
    def updateNScales(self,  sliderVal = 5):        
        self.properties['NScales'].sliderAssign(sliderVal)
        
        
 