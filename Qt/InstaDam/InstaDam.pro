
QT = core gui

TEMPLATE = subdirs

unix {SUBDIRS = \
          app \
            quazip
    }

win32{SUBDIRS = \
          app }

wasm: SUBDIRS += filehandler

app.subdir = app



unix{
quazip.subdir = quazip
}
wasm {
    filehandler.subdir = htmlFileHandler
    unix: app.depends = filehandler quazip
    win32: app.depends = filehandler
}
!wasm{
    unix: app.depends =  quazip

}

DISTFILES += \
    instadam.qdocconf \
    mirrors.qdoc \
    select.qdoc \
    app/main.qdoc
