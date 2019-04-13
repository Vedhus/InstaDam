
QT += widgets testlib

TEMPLATE = subdirs

unix {SUBDIRS = \
          integration \
          appTest \
          selectorTest\
            quazip
    }

win32{SUBDIRS = \
          integration \
          appTest \
          selectorTest}

            #quazip
appTest.subdir = appTest
integration.subdir = integration
selectorTest.subdir = SelectorTest

unix{
quazip.subdir = quazip
}
unix: appTest.depends = selectorTest quazip
win32: appTest.depends = selectorTest
integration.depends = appTest selectorTest quazip

target.path = .
INSTALLS += target
#LIBS += -L$${PWD}/../.. -lselector
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

DEFINES += TEST
