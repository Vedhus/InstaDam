/****************************************************************************
** Meta object code from reading C++ file 'instadam.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../InstaDam/instadam.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instadam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InstaDam_t {
    QByteArrayData data[13];
    char stringdata0[257];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InstaDam_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InstaDam_t qt_meta_stringdata_InstaDam = {
    {
QT_MOC_LITERAL(0, 0, 8), // "InstaDam"
QT_MOC_LITERAL(1, 9, 28), // "on_actionOpen_File_triggered"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(4, 62, 7), // "Project"
QT_MOC_LITERAL(5, 70, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(6, 94, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(7, 118, 20), // "on_panButton_clicked"
QT_MOC_LITERAL(8, 139, 21), // "on_roundBrush_clicked"
QT_MOC_LITERAL(9, 161, 22), // "on_squareBrush_clicked"
QT_MOC_LITERAL(10, 184, 24), // "on_pushButton_14_clicked"
QT_MOC_LITERAL(11, 209, 28), // "on_actionSave_File_triggered"
QT_MOC_LITERAL(12, 238, 18) // "resetPixmapButtons"

    },
    "InstaDam\0on_actionOpen_File_triggered\0"
    "\0on_actionNew_triggered\0Project\0"
    "on_actionOpen_triggered\0on_actionSave_triggered\0"
    "on_panButton_clicked\0on_roundBrush_clicked\0"
    "on_squareBrush_clicked\0on_pushButton_14_clicked\0"
    "on_actionSave_File_triggered\0"
    "resetPixmapButtons"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InstaDam[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       5,    0,   66,    2, 0x08 /* Private */,
       6,    0,   67,    2, 0x08 /* Private */,
       7,    0,   68,    2, 0x08 /* Private */,
       8,    0,   69,    2, 0x08 /* Private */,
       9,    0,   70,    2, 0x08 /* Private */,
      10,    0,   71,    2, 0x08 /* Private */,
      11,    0,   72,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    0x80000000 | 4,
    0x80000000 | 4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void InstaDam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InstaDam *_t = static_cast<InstaDam *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionOpen_File_triggered(); break;
        case 1: { Project _r = _t->on_actionNew_triggered();
            if (_a[0]) *reinterpret_cast< Project*>(_a[0]) = std::move(_r); }  break;
        case 2: { Project _r = _t->on_actionOpen_triggered();
            if (_a[0]) *reinterpret_cast< Project*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->on_actionSave_triggered(); break;
        case 4: _t->on_panButton_clicked(); break;
        case 5: _t->on_roundBrush_clicked(); break;
        case 6: _t->on_squareBrush_clicked(); break;
        case 7: _t->on_pushButton_14_clicked(); break;
        case 8: _t->on_actionSave_File_triggered(); break;
        case 9: _t->resetPixmapButtons(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InstaDam::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_InstaDam.data,
      qt_meta_data_InstaDam,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *InstaDam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InstaDam::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InstaDam.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int InstaDam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
