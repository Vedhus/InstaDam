
QT += widgets testlib

TEMPLATE = subdirs

unix {SUBDIRS = \
          app \
          appTest \
          selectorTest\
            quazip
    }

win32{SUBDIRS = \
          app \
          appTest \
          selectorTest}

            #quazip
wasm: SUBDIRS += filehandler #quazip

appTest.subdir = appTest
app.subdir = app
selectorTest.subdir = SelectorTest

unix{
quazip.subdir = quazip
}
wasm {
    filehandler.subdir = htmlFileHandler
    unix: appTest.depends = filehandler selector quazip
    win32: appTest.depends = filehandler selector
    app.depends = appTest
}
!wasm{
    unix: appTest.depends = selectorTest quazip
    win32: appTest.depends = selectorTest
    app.depends = appTest selectorTest quazip
}

target.path = .
INSTALLS += target
#LIBS += -L$${PWD}/../.. -lselector
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
