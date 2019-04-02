QT += widgets testlib

SOURCES = testRectangleSelect.cpp \
          testEllipseSelect.cpp \
          testfree.cpp

HEADERS = \
          ../boxbasedselector.h \
          ../rectangleSelect.h \
          ../ellipseSelect.h \
          ../selectItem.h \
          ../freeDrawSelect.h \
          ../freeDrawErase.h \
          ../label.h \
          testSelect.h

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
LIBS += -L$${PWD}/../.. -lselector
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
