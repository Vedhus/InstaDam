
QT = core gui

TEMPLATE = subdirs

unix {SUBDIRS = \
          app \
          selector\
            quazip
    }

win32{SUBDIRS = \
          app \
          selector}

wasm: SUBDIRS += filehandler #quazip

app.subdir = app


selector.subdir = Selector
unix{
quazip.subdir = quazip
}
wasm {
    filehandler.subdir = htmlFileHandler
    unix: app.depends = filehandler selector quazip
    win32: app.depends = filehandler selector
}
!wasm{
    unix: app.depends = selector quazip
    win32: app.depends = selector
}

