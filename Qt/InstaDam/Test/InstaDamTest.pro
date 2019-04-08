
QT += widgets testlib

TEMPLATE = subdirs

unix {SUBDIRS = \
          appTest \
          selectorTest\
            quazip
    }

win32{SUBDIRS = \
          appTest \
          selectorTest}

            #quazip
wasm: SUBDIRS += filehandler #quazip

appTest.subdir = appTest


selectorTest.subdir = SelectorTest
unix{
quazip.subdir = quazip
}
wasm {
    filehandler.subdir = htmlFileHandler
    unix: appTest.depends = filehandler selector quazip
    win32: appTest.depends = filehandler selector
}
!wasm{
    unix: appTest.depends = selectorTest quazip
    win32: appTest.depends = selectorTest
}

target.path = .
INSTALLS += target
#LIBS += -L$${PWD}/../.. -lselector
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
