# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = Selector

HEADERS = \
   $$PWD/commands.h \
   $$PWD/selectItem.h \
   $$PWD/photoScene.h \
   $$PWD/mainwindow.h \
   $$PWD/rectangleSelect.h \
   $$PWD/ellipseSelect.h

SOURCES = \
   $$PWD/commands.cpp \
   $$PWD/selectItem.cpp \
   $$PWD/photoScene.cpp \
   $$PWD/main.cpp \
   $$PWD/mainwindow.cpp \
   $$PWD/rectangleSelect.cpp \
   $$PWD/ellipseSelect.cpp

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

