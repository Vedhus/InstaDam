
QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
wasm: DEFINES += WASM_BUILD
TARGET = ../InstaDam
TEMPLATE = app
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
SOURCES += \
        main.cpp \
        instadam.cpp \
        project.cpp \
        maskviewer.cpp \
        photoviewer.cpp \
        pixmapops.cpp \
        filterproperty.cpp \
        filters.cpp \
        picpushbutton.cpp \
        maskobjects.cpp \
        filtercontrols.cpp \
        fslider.cpp \
        newproject.cpp \
        labelButton.cpp \
    register.cpp \
    startingwidget.cpp \
    login.cpp \
    projectlist.cpp \
    imagelist.cpp

wasm: SOURCES += colordialog.cpp

HEADERS += \
        instadam.h \
        project.h \
        photoviewer.h \
        maskviewer.h \
        pixmapops.h \
        filterproperty.h \
        filters.h \
        picpushbutton.h \
        maskobjects.h \
        filtercontrols.h \
        filterproperty.h \
        fslider.h \
        newproject.h \
        labelButton.h \
    register.h \
    startingwidget.h \
    login.h \
    projectlist.h \
    imagelist.h
wasm: HEADERS += colordialog.h

FORMS += \
        instadam.ui \
        newproject.ui \
        ../Selector/blankFrame.ui \
        ../Selector/polygonSelect.ui \
        ../Selector/freeSelect.ui \
        labeldialog.ui \
    register.ui \
    startingwidget.ui \
    login.ui \
    projectlist.ui \
    imagelist.ui


win32 {



  CONFIG += c++1z
   INCLUDEPATH += $${PWD}\..\quazip\include
## Change paths here depending on your system installation location
   INCLUDEPATH +=  D:\opencv\build\include $${PWD}/..
    LIBS += D:\opencv-build\bin\libopencv_core401.dll
   LIBS += D:\opencv-build\bin\libopencv_highgui401.dll
   LIBS += D:\opencv-build\bin\libopencv_imgcodecs401.dll
   LIBS += D:\opencv-build\bin\libopencv_imgproc401.dll
   LIBS += D:\opencv-build\bin\libopencv_features2d401.dll
   LIBS += D:\opencv-build\bin\libopencv_calib3d401.dll
   LIBS += -L$${PWD}/../../build-InstaDam-Desktop_Qt_5_11_3_MinGW_32bit-Release/Selector
##End change paths

  LIBS += -L$${PWD}\..\quazip\lib
  LIBS += -lquazip -lz
  DEFINES+=ZLIB_WINAPI
  LIBS += -L$${PWD}/..

  LIBS +=  -lselector

wasm: LIBS += -L$${PWD}/../htmlFileHandler -lhtmlFileHandler -L/usr/lib/emscripten
}
unix {
  LIBS += -lQt5Widgets
  LIBS += -lopencv_imgcodecs
  LIBS += -lopencv_imgproc -lopencv_features2d -lopencv_calib3d -lopencv_highgui -lopencv_core
  LIBS += -L$${PWD}/.. -lselector -lquazip
wasm: LIBS += -lhtmlFileHandler -L/usr/lib/emscripten
  INCLUDEPATH += /usr/include/opencv4 $${PWD}/..

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
