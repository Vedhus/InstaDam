/****************************************************************************
** Meta object code from reading C++ file 'filtercontrols.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../InstaDam/filtercontrols.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filtercontrols.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_filterControls_t {
    QByteArrayData data[10];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filterControls_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filterControls_t qt_meta_stringdata_filterControls = {
    {
QT_MOC_LITERAL(0, 0, 14), // "filterControls"
QT_MOC_LITERAL(1, 15, 11), // "valAssigned"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "maskTypes"
QT_MOC_LITERAL(4, 38, 8), // "maskType"
QT_MOC_LITERAL(5, 47, 19), // "threshold_or_filter"
QT_MOC_LITERAL(6, 67, 4), // "thof"
QT_MOC_LITERAL(7, 72, 9), // "assignVal"
QT_MOC_LITERAL(8, 82, 7), // "propNum"
QT_MOC_LITERAL(9, 90, 5) // "value"

    },
    "filterControls\0valAssigned\0\0maskTypes\0"
    "maskType\0threshold_or_filter\0thof\0"
    "assignVal\0propNum\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filterControls[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    4,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, 0x80000000 | 5,    4,    8,    9,    6,

       0        // eod
};

void filterControls::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        filterControls *_t = static_cast<filterControls *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valAssigned((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< threshold_or_filter(*)>(_a[2]))); break;
        case 1: _t->assignVal((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< threshold_or_filter(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (filterControls::*)(maskTypes , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&filterControls::valAssigned)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject filterControls::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_filterControls.data,
      qt_meta_data_filterControls,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *filterControls::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filterControls::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filterControls.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int filterControls::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void filterControls::valAssigned(maskTypes _t1, threshold_or_filter _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
