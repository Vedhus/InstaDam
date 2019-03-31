#!/bin/sh
PATH=/D/Qt/5.12.0/mingw73_64/bin:$PATH
export PATH
QT_PLUGIN_PATH=/D/Qt/5.12.0/mingw73_64/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec "$@"
