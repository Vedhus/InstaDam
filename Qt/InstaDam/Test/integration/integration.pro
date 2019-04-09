QT += widgets network testlib
INCLUDEPATH += $${PWD}/../SelectorTest
INCLUDEPATH += $${PWD}/../appTest
INCLUDEPATH += $${PWD}/..
SOURCES = integrationTest.cpp

HEADERS = integrationTest.h

target.path = ../.
INSTALLS += target
LIBS += -L$${PWD}/.. -lselector -linstadam -lquazip
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
