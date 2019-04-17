TEMPLATE = lib
TARGET = ../selector
INCLUDEPATH += $${PWD} $${PWD}/.. $${PWD}/../app
HEADERS = \
    $$PWD/polygonSelect.h \
    $$PWD/commands.h \
    $$PWD/ellipseSelect.h \
    $$PWD/photoScene.h \
    $$PWD/rectangleSelect.h \
    $$PWD/selectItem.h \
    $$PWD/freeDrawSelect.h \
    boxbasedselector.h \
    label.h \
    freeDrawErase.h \
    app/photoviewer.h


SOURCES = \
    $$PWD/polygonSelect.cpp \
    $$PWD/commands.cpp \
    $$PWD/ellipseSelect.cpp \
    $$PWD/photoScene.cpp \
    $$PWD/rectangleSelect.cpp \
    $$PWD/selectItem.cpp \
    $$PWD/freeDrawSelect.cpp \
    boxbasedselector.cpp \
    label.cpp \
    freeDrawErase.cpp

FORMS += \
        ../app/instadam.ui \
        ../app/newproject.ui \
        blankFrame.ui \
        polygonSelect.ui \
        freeSelect.ui \
        ../app/labeldialog.ui \
        ../app/register.ui \
        ../app/startingwidget.ui \
        ../app/login.ui \
        ../app/projectlist.ui \
        ../app/imagelist.ui \
        ../app/projectDialog.ui \
        ../app/serverprojectname.ui \
    ../app/addusertoproject.ui \
    ../app/userprivilege.ui

LIBS += -lQt5Widgets

QT += core gui widgets network

## Change paths here depending on your system installation location

#   INCLUDEPATH += C:\opencv\opencv\build\include $${PWD}/..
#   LIBS += C:\opencv\opencv-build\bin\libopencv_core401.dll
#   LIBS += C:\opencv\opencv-build\bin\libopencv_highgui401.dll
#   LIBS += C:\opencv\opencv-build\bin\libopencv_imgcodecs401.dll
#   LIBS += C:\opencv\opencv-build\bin\libopencv_imgproc401.dll
#   LIBS += C:\opencv\opencv-build\bin\libopencv_features2d401.dll
#   LIBS += C:\opencv\opencv-build\bin\libopencv_calib3d401.dll

#   INCLUDEPATH += D:\opencv\build\include $${PWD}/..
#   LIBS += D:\opencv-build\bin\libopencv_core401.dll
#   LIBS += D:\opencv-build\bin\libopencv_highgui401.dll
#   LIBS += D:\opencv-build\bin\libopencv_imgcodecs401.dll
#   LIBS += D:\opencv-build\bin\libopencv_imgproc401.dll
#   LIBS += D:\opencv-build\bin\libopencv_features2d401.dll
#   LIBS += D:\opencv-build\bin\libopencv_calib3d401.dll

  # LIBS += -L$${PWD}/../../build-InstaDam-Desktop_Qt_5_11_3_MinGW_32bit-Release/Selector

   INCLUDEPATH += D:\opencv\opencv401\opencv\build\include $${PWD}/..
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_core401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_highgui401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_imgcodecs401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_imgproc401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_features2d401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_calib3d401.dll
