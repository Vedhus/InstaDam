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
