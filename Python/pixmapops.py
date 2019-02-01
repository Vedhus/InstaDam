# -*- coding: utf-8 -*-
"""
Created on Mon Jan 28 14:57:07 2019

@author: Vedhus
"""
from PyQt5 import QtCore, QtGui, QtWidgets
from filters import *

def testPixmap(r = 255,g = 0,b = 0, a = 255,size =(200,200)):
    px = QtGui.QPixmap(size[0],size[1])
    color = QtGui.QColor(r,g,b,a)
    px.fill(color) 
    return px

def maskPixmaps(p1, p2, mask, brushType):
    maskp2 = joinPixmaps(mask, p2, QtGui.QPainter.CompositionMode_SourceIn)
    result = directPixmaps(p1,maskp2, brushType)
    return result

def directPixmaps(p1, p2, brushType):
    if brushType == ERASER:
        mode =QtGui.QPainter.CompositionMode_DestinationOut
    else:
        mode = QtGui.QPainter.CompositionMode_SourceOver
        
    
    return joinPixmaps(p1, p2, mode)

def joinPixmaps(p1, p2, mode=QtGui.QPainter.CompositionMode_SourceOver):
    s = p1.size().expandedTo(p2.size())
    result =  QtGui.QPixmap(s)
    result.fill(QtCore.Qt.transparent)
    painter = QtGui.QPainter(result)
    painter.setRenderHint(QtGui.QPainter.Antialiasing)
    painter.drawPixmap(QtCore.QPoint(), p1)
    painter.setCompositionMode(mode)
    painter.drawPixmap(result.rect(), p2, p2.rect())
    painter.end()
    return result

    
#def maskPixmaps(p1, p2, mask):
#    maskp2 = joinPixmaps(mask, p2, QtGui.QPainter.CompositionMode_SourceIn)
#    result = joinPixmaps(maskp2,p1)
#    return result