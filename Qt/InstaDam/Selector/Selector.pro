TEMPLATE = lib
TARGET = ../selector
INCLUDEPATH += $${PWD} $${PWD}/.. $${PWD}/../app
HEADERS = \
    $$PWD/polygonSelect.h \
    $$PWD/ellipseSelect.h \
    $$PWD/photoScene.h \
    $$PWD/rectangleSelect.h \
    $$PWD/selectItem.h \
    $$PWD/freeDrawSelect.h \
    boxbasedselector.h \
    label.h \
    freeDrawErase.h \
    jsonConstants.h


SOURCES = \
    $$PWD/polygonSelect.cpp \
    $$PWD/ellipseSelect.cpp \
    $$PWD/photoScene.cpp \
    $$PWD/rectangleSelect.cpp \
    $$PWD/selectItem.cpp \
    $$PWD/freeDrawSelect.cpp \
    boxbasedselector.cpp \
    label.cpp \
    freeDrawErase.cpp

LIBS += -lQt5Widgets

QT += core gui widgets network
