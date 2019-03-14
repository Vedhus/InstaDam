
QT       += core gui

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
        labelButton.cpp

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
        labelButton.h
wasm: HEADERS += colordialog.h

FORMS += \
        instadam.ui \
        newproject.ui \
        ../Selector/blankFrame.ui \
        ../Selector/polygonSelect.ui \
        ../Selector/freeSelect.ui \
        labeldialog.ui


win32 {
  INCLUDEPATH += D:\opencv\build\include $${PWD}/..
  CONFIG += c++1z

  LIBS += D:\opencv\opencv-build\bin\libopencv_core401.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_highgui401.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_imgcodecs401.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_imgproc401.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_features2d401.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_calib3d401.dll

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
