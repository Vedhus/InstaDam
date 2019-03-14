
QT = core gui

TEMPLATE = subdirs

SUBDIRS = \
          app \
          selector \
          quazip
wasm: SUBDIRS += filehandler quazip

app.subdir = app
selector.subdir = Selector
    quazip.subdir = quazip
wasm {
    filehandler.subdir = htmlFileHandler
    app.depends = filehandler selector quazip
}
!wasm{
    app.depends = selector quazip
}
