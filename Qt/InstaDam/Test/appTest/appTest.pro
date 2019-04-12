TEMPLATE = lib
TARGET = ../instadam
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
INCLUDEPATH += $${PWD}/..
INCLUDEPATH += /usr/include/opencv4
QT += widgets network core testlib

HEADERS = \
        instadam.h \
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
    enumconstants.h

SOURCES = \
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
    enumconstants.cpp

LIBS += -lQt5Widgets -L$${PWD}/.. -L$${OUT_PWD}/.. -L$${PWD} -lselector -lquazip
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc -lopencv_features2d -lopencv_calib3d -lopencv_highgui -lopencv_core

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
    imagelist.ui \
    projectDialog.ui \
    serverprojectname.ui

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
