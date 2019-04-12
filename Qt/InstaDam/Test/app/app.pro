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
    testFilters.cpp \
    testproject.cpp \
    testlabelbutton.cpp

HEADERS = testSelect.h \
    testapp.h

target.path = ../.
INSTALLS += target
LIBS += -L$${PWD}/.. -lselector -linstadam -lquazip
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
