/****************************************************************************
** Meta object code from reading C++ file 'instadam.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../InstaDam/instadam.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instadam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InstaDam_t {
    QByteArrayData data[6];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InstaDam_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InstaDam_t qt_meta_stringdata_InstaDam = {
    {
QT_MOC_LITERAL(0, 0, 8), // "InstaDam"
QT_MOC_LITERAL(1, 9, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(2, 32, 7), // "Project"
QT_MOC_LITERAL(3, 40, 0), // ""
QT_MOC_LITERAL(4, 41, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(5, 65, 23) // "on_actionSave_triggered"

    },
    "InstaDam\0on_actionNew_triggered\0Project\0"
    "\0on_actionOpen_triggered\0"
    "on_actionSave_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InstaDam[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    3, 0x08 /* Private */,
       4,    0,   30,    3, 0x08 /* Private */,
       5,    0,   31,    3, 0x08 /* Private */,

 // slots: parameters
    0x80000000 | 2,
    0x80000000 | 2,
    QMetaType::Void,

       0        // eod
};

void InstaDam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InstaDam *_t = static_cast<InstaDam *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { Project _r = _t->on_actionNew_triggered();
            if (_a[0]) *reinterpret_cast< Project*>(_a[0]) = std::move(_r); }  break;
        case 1: { Project _r = _t->on_actionOpen_triggered();
            if (_a[0]) *reinterpret_cast< Project*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->on_actionSave_triggered(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InstaDam::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_InstaDam.data,
    qt_meta_data_InstaDam,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
