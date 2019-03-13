
QT = core gui

TEMPLATE = subdirs

SUBDIRS = \
          app \
          selector
wasm: SUBDIRS += filehandler

app.subdir = app
selector.subdir = Selector
wasm {
    filehandler.subdir = htmlFileHandler
    app.depends = filehandler selector
}
!wasm{
    app.depends = selector
}
