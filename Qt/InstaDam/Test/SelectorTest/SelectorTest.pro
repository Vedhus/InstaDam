TEMPLATE = lib
TARGET = ../selector
INCLUDEPATH += $${PWD} $${PWD}/.. $${PWD}/../appTest

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
    freeDrawErase.h \
    app/photoviewer.h

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
    freeDrawErase.cpp

FORMS += \
        ../appTest/instadam.ui \
        ../appTest/newproject.ui \
        blankFrame.ui \
        polygonSelect.ui \
        freeSelect.ui \
        ../appTest/labeldialog.ui \
        ../appTest/register.ui \
        ../appTest/startingwidget.ui \
        ../appTest/login.ui \
        ../appTest/projectlist.ui \
        ../appTest/imagelist.ui \
        ../appTest/projectDialog.ui \
        ../appTest/serverprojectname.ui \
    ../appTest/addusertoproject.ui \
    ../appTest/userprivilege.ui

LIBS += -lQt5Widgets

QT += widgets testlib network

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
