TEMPLATE = lib
TARGET = ../htmlFileHandler

LIBS += -L/usr/lib/emscripten
INCLUDEPATH += -I/usr/include/emscripten
SOURCES += qhtml5file.cpp
HEADERS += qhtml5file.h

QT = core
