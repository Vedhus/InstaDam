QT += widgets network testlib
TARGET = ../IntegrationTest

INCLUDEPATH += $${PWD}/../SelectorTest
INCLUDEPATH += $${PWD}/../appTest
INCLUDEPATH += $${PWD}/..
INCLUDEPATH += $${OUT_PWD}/../SelectorTest
INCLUDEPATH += $${OUT_PWD}/../appTest
INCLUDEPATH += $${OUT_PWD}/..

SOURCES = integrationTest.cpp

HEADERS = integrationTest.h

target.path = ../.
INSTALLS += target
LIBS += -L$${PWD}/.. -L$${OUT_PWD}/.. -lselector -linstadam -lquazip
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
