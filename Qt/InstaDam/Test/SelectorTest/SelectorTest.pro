TEMPLATE = lib
TARGET = ../selector

HEADERS = \
    polygonSelect.h \
    commands.h \
    ellipseSelect.h \
    photoScene.h \
    rectangleSelect.h \
    selectItem.h \
    freeDrawSelect.h \
    boxbasedselector.h \
    label.h \
    freeDrawErase.h

SOURCES = \
    polygonSelect.cpp \
    commands.cpp \
    ellipseSelect.cpp \
    photoScene.cpp \
    rectangleSelect.cpp \
    selectItem.cpp \
    freeDrawSelect.cpp \
    boxbasedselector.cpp \
    label.cpp \
    freeDrawErase.cpp \
    testpolygonselect.cpp \
    testlabel.cpp \
    testphotoscene.cpp \
    testcommands.cpp

LIBS += -lQt5Widgets

QT += widgets testlib

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
