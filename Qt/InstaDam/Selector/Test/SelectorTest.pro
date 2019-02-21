QT += widgets testlib

SOURCES = testRectangleSelect.cpp \
          testEllipseSelect.cpp \
          ../rectangleSelect.cpp \
          ../ellipseSelect.cpp \
          ../selectItem.cpp

HEADERS = \
          ../rectangleSelect.h \
          ../ellipseSelect.h \
          ../selectItem.h \
    testSelect.h

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
