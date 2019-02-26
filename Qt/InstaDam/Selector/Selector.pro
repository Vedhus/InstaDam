# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = Selector

HEADERS = \
    $$PWD/polygonSelect.h \
    $$PWD/commands.h \
    $$PWD/ellipseSelect.h \
    $$PWD/mainwindow.h \
    $$PWD/photoScene.h \
    $$PWD/rectangleSelect.h \
    $$PWD/selectItem.h \
    $$PWD/freeDrawSelect.h \
    boxbasedselector.h \
    selectgroup.h

SOURCES = \
    $$PWD/polygonSelect.cpp \
    $$PWD/commands.cpp \
    $$PWD/ellipseSelect.cpp \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/photoScene.cpp \
    $$PWD/rectangleSelect.cpp \
    $$PWD/selectItem.cpp \
    $$PWD/freeDrawSelect.cpp \
    boxbasedselector.cpp \
    selectgroup.cpp

INCLUDEPATH = \
    $$PWD/. \
    /opt/Qt5/5.12.0/gcc_64/include/QtWidgets \
    /opt/Qt5/5.12.0/gcc_64/include/QtCore \
    /opt/Qt5/5.12.0/gcc_64/include/QtGui \
    /opt/Qt5/5.12.0/gcc_64/include/QtWidgets/5.12.0 \
    /opt/Qt5/5.12.0/gcc_64/include/QtGui/5.12.0 \
    /opt/Qt5/5.12.0/gcc_64/include/QtCore/5.12.0 \
    /opt/Qt5/5.12.0/gcc_64/include/QtWidgets/5.12.0/QtWidgets \
    /opt/Qt5/5.12.0/gcc_64/include/QtGui/5.12.0/QtGui \
    /opt/Qt5/5.12.0/gcc_64/include/QtCore/5.12.0/QtCore


LIBS += -lQt5Widgets
#DEFINES =

