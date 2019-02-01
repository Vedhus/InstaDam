# -*- coding: utf-8 -*-
"""
Created on Mon Jan 28 14:45:31 2019

@author: Vedhus
"""
from functools import partial    

# Import filter classes
from filters import *

# Import custom buttons
from customButtons import *

# Import QGraphics Views
from viewers import *


class Window(QtWidgets.QWidget):
    def __init__(self):
        super(Window, self).__init__()
        self.labels = np.array(['Eraser','Crack','Spalling','Exposed\nRebar','Corrosion','Fatigue\nCracks','Asphalt\nCracks'])
        self.colors = [  [255,255,255,128],\
                       [100,100,200,255],\
                    [145,255,145,255],\
                    [206,66,52,255],\
                    [125,125,125,255],\
                    [162,20,47,255],\
                    [126,47,42,255],\
                    [255, 215, 0,255] ]
        
        self.viewer = PhotoViewer(self, self.colors, viewerType = IM_VIEW)
        self.viewerMask = MaskViewer(self, self.viewer, viewerType = MASK_VIEW)
        
        # 'Load image' button
        self.btnLoad = QtWidgets.QToolButton(self)
        self.btnLoad.setText('Load image')
        self.btnLoad.clicked.connect(self.loadImage)
        # 'Save image' button
        self.btnSave= QtWidgets.QToolButton(self)
        self.btnSave.setText('Save image')
        self.btnSave.clicked.connect(self.saveImage)
        
        # Button to change from drag/pan to getting pixel info
        self.btnPanInfo = QtWidgets.QToolButton(self)
        self.btnPanInfo.setText('Pan')
        self.btnPanInfo.clicked.connect(self.selectPan)
        self.viewer.zoomed.connect(self.viewerMask.zoomedInADifferentView)
        self.viewerMask.zoomed.connect(self.viewer.zoomedInADifferentView)
        
        self.BrushSizeLabel = QtWidgets.QLabel(self)
        self.BrushSizeLabel.setText('Brush Size')
        
        # Button for filterOptions
        self.btnFilter = QtWidgets.QPushButton('Filter Options', self)
        self.btnFilter.clicked.connect(self.filterClicked)
        
        self.editPixInfo = QtWidgets.QLineEdit(self)
        self.editPixInfo.setReadOnly(True)
        self.viewer.photoClicked.connect(self.photoClicked)
        self.viewerMask.photoClicked.connect(self.photoClicked)
        
        self.viewerlist = [self.viewer, self.viewerMask]
        # Brush Slider
        self.brushSlider = QtWidgets.QSlider()
        self.brushSlider.setOrientation(Qt.Horizontal)
        self.brushSlider.valueChanged.connect(self.viewer.resetBrush)
        self.brushSlider.valueChanged.connect(self.viewerMask.resetBrush)
        

        #LabelButtons
        VLabelLayout = QtWidgets.QVBoxLayout()        
        self.labelButtons = [LabelButton(label, color, self.viewer) for label, color in zip(self.labels, self.colors)]
        for labelButton, mb in zip(self.labelButtons, range(len(self.labelButtons))):
            for labelConnect, mc in zip(self.labelButtons, range(len(self.labelButtons))):
                if mb!=mc:
                    # Inform all other filters that current filter has been selected so others can get deselected
                    labelButton.checked.connect(labelConnect.otherBoxChecked)
                else:
                    for viewer in self.viewerlist:
                        labelButton.clicked.connect(partial(self.selectBrush,labelButton.name))
 
        for button in self.labelButtons:
            VLabelLayout.addWidget(button)
        self.scrollLabel = QtWidgets.QWidget(self)
        self.scrollLabel.setLayout(VLabelLayout)  
        self.scrollLabelArea = QtWidgets.QScrollArea(self)
        self.scrollLabelArea.setWidgetResizable(True)
        self.scrollLabelArea.setMaximumWidth(80)
        self.scrollLabelArea.setWidget(self.scrollLabel)
        self.scrollLabelArea.show()
        
        #ShapeButtons
        self.shapes = ['Round','Square']
        self.shapeColors = [[125, 125, 125, 255], [125, 125, 125, 255]]
        VshapeLayout = QtWidgets.QVBoxLayout()        
        self.shapeButtons = [ShapeButton(shape, color, self.viewer) for shape, color in zip(self.shapes, self.shapeColors)]
        for shapeButton, mb in zip(self.shapeButtons, range(len(self.shapeButtons))):
            for shapeConnect, mc in zip(self.shapeButtons, range(len(self.shapeButtons))):
                if mb!=mc:
                    # Inform all other filters that current filter has been selected so others can get deselected
                    shapeButton.checked.connect(shapeConnect.otherBoxChecked)
                else:
                    for viewer in self.viewerlist:
                        shapeButton.clicked.connect(partial(self.selectBrushShape,shapeButton.name))
 
        for button in self.shapeButtons:
            VshapeLayout.addWidget(button)
        self.scrollshape = QtWidgets.QWidget(self)
        self.scrollshape.setLayout(VshapeLayout)  
        self.scrollshapeArea = QtWidgets.QScrollArea(self)
        self.scrollshapeArea.setWidgetResizable(True)
        self.scrollshapeArea.setMaximumWidth(80)
        self.scrollshapeArea.setWidget(self.scrollshape)
        self.scrollshapeArea.show()
        
        
        #MaskButtons
        masks = ['Canny', 'Threshold','Blur','Frangi']

        HB2layout = QtWidgets.QHBoxLayout()
        self.maskButtons = [PicButton(mask, self.viewer) for mask in masks]

        
        for maskButton, mb in zip(self.maskButtons, range(len(self.maskButtons))):
            for maskConnect, mc in zip(self.maskButtons, range(len(self.maskButtons))):
                if mb!=mc:
                    # Inform all other filters that current filter has been selected so others can get deselected
                    maskButton.checked.connect(maskConnect.otherBoxChecked)
                else:
                    for viewer in self.viewerlist:
                        maskObj = eval('{0}()'.format(maskButton.filter))
                        # Store all filter mask buttons in a dictionary for reference
                        viewer.maskObjs[maskObj.name] = maskObj
                        maskButton.checked.connect(viewer.setImMask)
                    
                    
        for button in self.maskButtons:
            HB2layout.addWidget(button) 
        
        self.scrollChildArea = QtWidgets.QWidget(self)
        self.scrollChildArea.setLayout(HB2layout)  
        self.scrollArea = QtWidgets.QScrollArea(self)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.scrollArea.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.scrollArea.setMaximumHeight(200)

        self.scrollArea.setWidget(self.scrollChildArea)
        self.scrollArea.show()
 
  

        
        Vlayout = QtWidgets.QVBoxLayout(self)
        HBlayout = QtWidgets.QHBoxLayout()
        HBlayout.setAlignment(QtCore.Qt.AlignLeft)
        HBlayout.addWidget(self.btnLoad)
        HBlayout.addWidget(self.btnSave)
        HBlayout.addWidget(self.btnPanInfo)
        HBlayout.addWidget(self.BrushSizeLabel)
        HBlayout.addWidget(self.brushSlider)
#        HBlayout.addWidget(self.btnBrushInfo)
        #HBlayout.addWidget(self.btnEraserInfo)
        HBlayout.addWidget(self.editPixInfo)
        HBlayout.addWidget(self.btnFilter)

        
        HBWidget = QtWidgets.QWidget()
        HBWidget.setLayout(HBlayout)
        
        
        Vlayout.addWidget(HBWidget)
        # Arrange layout
        VBlayout = QtWidgets.QSplitter()
        VBlayout.setOrientation(Qt.Vertical)
        
        HB1Widget = QtWidgets.QWidget()
        
        #VBlayout.addWidget(self.viewer)
        HB1layout = QtWidgets.QHBoxLayout()
        VBToolLayout = QtWidgets.QVBoxLayout()
        VBToolLayout.addWidget(self.scrollLabelArea)
        VBToolLayout.addWidget(self.scrollshapeArea)
        HB1layout.addLayout(VBToolLayout)
        HB1layout.addWidget(self.viewer)
        HB1layout.addWidget(self.viewerMask)
        HB1Widget.setLayout(HB1layout)
        VBlayout.addWidget(HB1Widget)

        VBlayout.addWidget(self.scrollArea)
        
        Vlayout.addWidget(VBlayout)
        
  

    def loadImage(self):
        filename = QtWidgets.QFileDialog.getOpenFileName()
        if filename[0] != '':
            self.filename = filename
        else:
            return 0
        print(self.filename)

        im = QtGui.QPixmap(self.filename[0])
        for viewer in self.viewerlist:
            viewer.cvImage = cv2.imread(self.filename[0])
            viewer.thumbImage = cv2.resize(viewer.cvImage, (200,200))
            viewer.setPhoto(im)
    def saveImage(self):
        print(self.filename)
        self.dirname = os.path.dirname(self.filename[0])
        self.basename = os.path.basename(self.filename[0])
        self.name = os.path.splitext(self.basename)[0]
        print(self.name)
        dirs = os.path.join(self.dirname, 'labels')
        if not os.path.exists(dirs):
            os.mkdir(dirs)
            
        self.savefilename = os.path.join(dirs,self.name +'.npz')
        self.savelabelsname = os.path.join(dirs,self.name +'.jpg')
        

        channels_count = 4
        pixmap = self.viewer.labels.pixmap()
        image = pixmap.toImage()
        image.save (self.savelabelsname)
        print(self.savelabelsname)
        
        
    def keyPressEvent(self, event):
        if event.key()==Qt.Key_BracketRight :
            self.brushSlider.setValue(self.brushSlider.value() + 5)
        elif event.key()==Qt.Key_BracketLeft:
            self.brushSlider.setValue(self.brushSlider.value() - 5)
        else:
            QWidget.keyPressEvent(self, event)

    def selectPan(self):
        for viewer in self.viewerlist:
            viewer.setPanMode()
            
    def selectBrush(self,  brushType = 'Eraser', rect = None):
        self.selectedLabel = brushType
        
        for viewer in self.viewerlist:
            viewer.setBrushMode(np.argmax(self.labels==self.selectedLabel))
    def selectBrushShape(self, brushShape = 'Round', rect = None):
        self.selectedShape = brushShape
        for viewer in self.viewerlist:
            viewer.setBrushShape(brushShape)
        
    
    def selectEraser(self):
        for viewer in self.viewerlist:
            viewer.setBrushMode(ERASER)

    def photoClicked(self, pos):
        for viewer in self.viewerlist:
            if viewer.dragMode()  == QtWidgets.QGraphicsView.NoDrag:
                self.editPixInfo.setText('%d, %d' % (pos.x(), pos.y()))
    def filterClicked(self):       
        if self.viewer.imageSet:
            self._dialog = filterProps(self.viewerMask, self.viewerlist)
            self._dialog.show()


class filterProps(QtWidgets.QDialog):
    def __init__(self, viewer, viewers):
        super().__init__()
        filterObj = viewer.maskObjs[viewer.selectedFilter]
        self.setWindowTitle('Filter Options')        
        VBlayout = QtWidgets.QVBoxLayout(self)
        for key, value in filterObj.properties.items():
            name = QtWidgets.QLabel()
            name.setText(value.name)
            if value.btnType == 'slider':
                #item = QtWidgets.QSlider()
                itemSlider = QtWidgets.QSlider() 
                itemNumbox = QtWidgets.QSpinBox()
                itemSlider.valueChanged.connect(itemNumbox.setValue)
                itemSlider.setOrientation(Qt.Horizontal)
                itemNumbox.valueChanged.connect(itemSlider.setValue)
                HBlayout = QtWidgets.QHBoxLayout()
                HBlayout.addWidget(name)
                for item in [itemSlider, itemNumbox]:
                    item.setMaximum(value.max)
                    item.setMinimum(value.min)
                    item.setValue(value.val)
                    if value.signalBool:
                        value.valchanged.connect(item.setValue)
                    item.valueChanged.connect(eval('filterObj.update{0}'.format(key))) 
                    item.valueChanged.connect(eval('viewer.modifyMask{0}'.format(value.thof)))
                    HBlayout.addWidget(item)

                
            elif value.btnType == 'checkBox':
                item = QtWidgets.QCheckBox()
                item.setChecked = 'False'
                item.stateChanged.connect(eval('filterObj.update{0}'.format(key)))
                item.stateChanged.connect(eval('viewer.modifyMask{0}'.format(value.thof)))               
                HBlayout = QtWidgets.QHBoxLayout()
                HBlayout.addWidget(name)
                HBlayout.addWidget(item)
            VBlayout.addLayout(HBlayout)