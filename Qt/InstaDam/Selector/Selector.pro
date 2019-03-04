TEMPLATE = lib
TARGET = ../selector


HEADERS = \
    $$PWD/polygonSelect.h \
    $$PWD/commands.h \
    $$PWD/ellipseSelect.h \
    $$PWD/photoScene.h \
    $$PWD/rectangleSelect.h \
    $$PWD/selectItem.h \
    $$PWD/freeDrawSelect.h \
    boxbasedselector.h \
    selectgroup.h \
    label.h
    //freedrawerase.h

SOURCES = \
    $$PWD/polygonSelect.cpp \
    $$PWD/commands.cpp \
    $$PWD/ellipseSelect.cpp \
    $$PWD/photoScene.cpp \
    $$PWD/rectangleSelect.cpp \
    $$PWD/selectItem.cpp \
    $$PWD/freeDrawSelect.cpp \
    boxbasedselector.cpp \
    selectgroup.cpp \
    label.cpp
    //freedrawerase.cpp

LIBS += -lQt5Widgets

QT += core gui widgets
