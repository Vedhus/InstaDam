QT += widgets testlib
SOURCES = ../SelectorTest/testRectangleSelect.cpp \
          ../SelectorTest/testEllipseSelect.cpp \
          ../SelectorTest/testfree.cpp

HEADERS = ../SelectorTest/testSelect.h

target.path = ../.
INSTALLS += target
LIBS += -L$${PWD}/.. -lselector
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
