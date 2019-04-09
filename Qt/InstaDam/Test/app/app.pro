QT += widgets network testlib
INCLUDEPATH += $${PWD}/../SelectorTest
INCLUDEPATH += $${PWD}/../appTest
INCLUDEPATH += $${PWD}/..
SOURCES = testPolygonSelect.cpp \
    testLabel.cpp \
    testPhotoScene.cpp \
    testCommands.cpp \
    testRectangleSelect.cpp \
    testEllipseSelect.cpp \
    testFreeDrawSelect.cpp \
    testFilterProperty.cpp

HEADERS = testSelect.h \
    testapp.h

target.path = ../.
INSTALLS += target
LIBS += -L$${PWD}/.. -lselector -linstadam
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
