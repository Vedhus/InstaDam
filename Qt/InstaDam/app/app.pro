
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
wasm: DEFINES += WASM_BUILD
TARGET = ../InstaDam
TEMPLATE = app

SOURCES += \
        main.cpp \
        instadam.cpp \
        label.cpp \
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

HEADERS += \
        instadam.h \
        label.h \
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

FORMS += \
        instadam.ui \
        newproject.ui \
        ../Selector/blankFrame.ui \
        ../Selector/polygonSelect.ui \
        ../Selector/freeSelect.ui


win32 {
  INCLUDEPATH += D:\opencv\opencv\build\include $${PWD}/..
  CONFIG += c++1z

  LIBS += D:\opencv\opencv-build\bin\libopencv_core320.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_highgui320.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_imgcodecs320.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_imgproc320.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_features2d320.dll
  LIBS += D:\opencv\opencv-build\bin\libopencv_calib3d320.dll

wasm: LIBS += -L$${PWD}/../htmlFileHandler -lhtmlFileHandler -L/usr/lib/emscripten
}
unix {
  LIBS += -lQt5Widgets
  LIBS += -lopencv_imgcodecs
  LIBS += -lopencv_imgproc -lopencv_features2d -lopencv_calib3d -lopencv_highgui -lopencv_core
  LIBS += -L$${PWD}/.. -lselector
wasm: LIBS += -lhtmlFileHandler -L/usr/lib/emscripten
  INCLUDEPATH += /usr/include/opencv4 $${PWD}/..
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target