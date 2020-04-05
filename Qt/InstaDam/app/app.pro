
QT       += core gui
QT       += network
DEFINES += QT_SHAREDPOINTER_TRACK_POINTERS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
wasm: DEFINES += WASM_BUILD
TARGET = ../InstaDam
TEMPLATE = app
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
#INCLUDEPATH += D:\PythonQt\PythonQt\include\PythonQt\

SOURCES += \
        freedrawstack.cpp \
        main.cpp \
        instadam.cpp \
        project.cpp \
        maskviewer.cpp \
        photoviewer.cpp \
        pixmapops.cpp \
        filterproperty.cpp \
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
        imagelist.cpp \
        serverprojectname.cpp \
        enumconstants.cpp \
        addusertoproject.cpp \
        userprivilege.cpp \
        chooselabeldialog.cpp \
        commands.cpp \
        projectdeletionconfirmation.cpp \
        polygonSelect.cpp \
        ellipseSelect.cpp \
        photoScene.cpp \
        rectangleSelect.cpp \
        selectItem.cpp \
        freeDrawSelect.cpp \
        boxbasedselector.cpp \
        label.cpp \
        freeDrawErase.cpp



wasm: SOURCES += colordialog.cpp

HEADERS += \
    freedrawstack.h \
        instadam.h \
    mat.h \
    matrix.h \
        project.h \
        photoviewer.h \
        maskviewer.h \
        pixmapops.h \
        filterproperty.h \
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
        imagelist.h \
        serverprojectname.h \
        enumconstants.h \
    addusertoproject.h \
    tmwtypes.h \
    userprivilege.h \
    chooselabeldialog.h \
    commands.h \
    projectdeletionconfirmation.h \
    polygonSelect.h \
    ellipseSelect.h \
    photoScene.h \
    rectangleSelect.h \
    selectItem.h \
    freeDrawSelect.h \
    boxbasedselector.h \
    label.h \
    freeDrawErase.h \
    jsonConstants.h



wasm: HEADERS += colordialog.h

FORMS += \
        instadam.ui \
        newproject.ui \
        blankFrame.ui \
        polygonSelect.ui \
        freeSelect.ui \
        labeldialog.ui \
        register.ui \
        startingwidget.ui \
        login.ui \
        projectlist.ui \
        imagelist.ui \
        projectDialog.ui \
        serverprojectname.ui \
    addusertoproject.ui \
    userprivilege.ui \
    projectdeletionconfirmation.ui \


win32 {

  CONFIG += c++1z
  INCLUDEPATH += $${PWD}\..\quazip\include
  INCLUDEPATH += D:\cnpy\install\include
  INCLUDEPATH += D:\zlib1211\zlib-install\include
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

   LIBS += -L$${PWD}/../../build-InstaDam-Desktop_Qt_5_11_3_MinGW_32bit-Release/Selector
   LIBS += -L$${PWD}/../../build-InstaDam-Desktop_Qt_5_11_3_MinGW_32bit-Debug/Selector

   INCLUDEPATH += D:\opencv\opencv401\opencv-build\install\include $${PWD}/..
   INCLUDEPATH += D:\opencv\opencv401\opencv\build\include
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_core401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_highgui401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_imgcodecs401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_imgproc401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_features2d401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_calib3d401.dll
   LIBS += D:\opencv\opencv401\opencv-build\bin\libopencv_ximgproc401.dll
   LIBS += D:\cnpy\install\bin\libcnpy.dll
   LIBS += D:/zlib1211/zlib-install/bin/libzlib.dll
#   LIBS += D:\PythonQt\PythonQt\bin\libPythonQt.dll
##End change paths

  LIBS += -L$${PWD}\..\quazip\lib
  LIBS += -LD:\cnpy\install\lib
  LIBS += -lquazip -lcnpy -lz #-lpythoqt
  DEFINES+=ZLIB_WINAPI
  LIBS += -L$${PWD}/..
#  LIBS += -LD:\PythonQt\PythonQt\lib

#  LIBS +=  -lselector

wasm: LIBS += -L$${PWD}/../htmlFileHandler -lhtmlFileHandler -L/usr/lib/emscripten
}
unix {
  LIBS += -lQt5Widgets
  LIBS += -lopencv_imgcodecs
  LIBS += -lopencv_imgproc -lopencv_features2d -lopencv_calib3d -lopencv_highgui -lopencv_core
  LIBS += -L$${PWD}/.. -lselector -lquazip
wasm: LIBS += -lhtmlFileHandler -L/usr/lib/emscripten
#  INCLUDEPATH += /usr/include/opencv4 $${PWD}/..
#  INCLUDEPATH += C:/Users/hoskere2/AppData/Local/Programs/Python/Python37-32/include/
# INCLUDEPATH += D:\opencv\opencv401\opencv-build\install\include\

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
