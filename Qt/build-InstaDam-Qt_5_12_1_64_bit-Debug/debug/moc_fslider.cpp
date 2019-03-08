/****************************************************************************
** Meta object code from reading C++ file 'fslider.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../InstaDam/fslider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_fSlider_t {
    QByteArrayData data[11];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fSlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fSlider_t qt_meta_stringdata_fSlider = {
    {
QT_MOC_LITERAL(0, 0, 7), // "fSlider"
QT_MOC_LITERAL(1, 8, 18), // "filterValueChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "maskTypes"
QT_MOC_LITERAL(4, 38, 12), // "selectedMask"
QT_MOC_LITERAL(5, 51, 7), // "propNum"
QT_MOC_LITERAL(6, 59, 5), // "value"
QT_MOC_LITERAL(7, 65, 19), // "threshold_or_filter"
QT_MOC_LITERAL(8, 85, 15), // "fSliderReleased"
QT_MOC_LITERAL(9, 101, 18), // "reemitValueChanged"
QT_MOC_LITERAL(10, 120, 20) // "reemitSliderReleased"

    },
    "fSlider\0filterValueChanged\0\0maskTypes\0"
    "selectedMask\0propNum\0value\0"
    "threshold_or_filter\0fSliderReleased\0"
    "reemitValueChanged\0reemitSliderReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fSlider[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   46,    2, 0x08 /* Private */,
      10,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, 0x80000000 | 7,    4,    5,    6,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void fSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fSlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filterValueChanged((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< threshold_or_filter(*)>(_a[4]))); break;
        case 1: _t->fSliderReleased((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->reemitValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->reemitSliderReleased(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (fSlider::*)(maskTypes , int , int , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fSlider::filterValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (fSlider::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fSlider::fSliderReleased)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fSlider::staticMetaObject = { {
    &QSlider::staticMetaObject,
    qt_meta_stringdata_fSlider.data,
    qt_meta_data_fSlider,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fSlider.stringdata0))
        return static_cast<void*>(this);
    return QSlider::qt_metacast(_clname);
}

int fSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void fSlider::filterValueChanged(maskTypes _t1, int _t2, int _t3, threshold_or_filter _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void fSlider::fSliderReleased(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_fSpinBox_t {
    QByteArrayData data[10];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fSpinBox_t qt_meta_stringdata_fSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 8), // "fSpinBox"
QT_MOC_LITERAL(1, 9, 18), // "filterValueChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "maskTypes"
QT_MOC_LITERAL(4, 39, 12), // "selectedMask"
QT_MOC_LITERAL(5, 52, 7), // "propNum"
QT_MOC_LITERAL(6, 60, 5), // "value"
QT_MOC_LITERAL(7, 66, 19), // "threshold_or_filter"
QT_MOC_LITERAL(8, 86, 12), // "fSlotChanged"
QT_MOC_LITERAL(9, 99, 18) // "reemitValueChanged"

    },
    "fSpinBox\0filterValueChanged\0\0maskTypes\0"
    "selectedMask\0propNum\0value\0"
    "threshold_or_filter\0fSlotChanged\0"
    "reemitValueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fSpinBox[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x06 /* Public */,
       8,    2,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, 0x80000000 | 7,    4,    5,    6,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 7,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void fSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filterValueChanged((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< threshold_or_filter(*)>(_a[4]))); break;
        case 1: _t->fSlotChanged((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< threshold_or_filter(*)>(_a[2]))); break;
        case 2: _t->reemitValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (fSpinBox::*)(maskTypes , int , int , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fSpinBox::filterValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (fSpinBox::*)(maskTypes , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fSpinBox::fSlotChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fSpinBox::staticMetaObject = { {
    &QSpinBox::staticMetaObject,
    qt_meta_stringdata_fSpinBox.data,
    qt_meta_data_fSpinBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fSpinBox.stringdata0))
        return static_cast<void*>(this);
    return QSpinBox::qt_metacast(_clname);
}

int fSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSpinBox::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void fSpinBox::filterValueChanged(maskTypes _t1, int _t2, int _t3, threshold_or_filter _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void fSpinBox::fSlotChanged(maskTypes _t1, threshold_or_filter _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_fCheckBox_t {
    QByteArrayData data[10];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fCheckBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fCheckBox_t qt_meta_stringdata_fCheckBox = {
    {
QT_MOC_LITERAL(0, 0, 9), // "fCheckBox"
QT_MOC_LITERAL(1, 10, 18), // "filterValueChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "maskTypes"
QT_MOC_LITERAL(4, 40, 12), // "selectedMask"
QT_MOC_LITERAL(5, 53, 7), // "propNum"
QT_MOC_LITERAL(6, 61, 5), // "value"
QT_MOC_LITERAL(7, 67, 19), // "threshold_or_filter"
QT_MOC_LITERAL(8, 87, 13), // "fStateChanged"
QT_MOC_LITERAL(9, 101, 18) // "reemitStateChanged"

    },
    "fCheckBox\0filterValueChanged\0\0maskTypes\0"
    "selectedMask\0propNum\0value\0"
    "threshold_or_filter\0fStateChanged\0"
    "reemitStateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fCheckBox[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x06 /* Public */,
       8,    2,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, 0x80000000 | 7,    4,    5,    6,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 7,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void fCheckBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fCheckBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filterValueChanged((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< threshold_or_filter(*)>(_a[4]))); break;
        case 1: _t->fStateChanged((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< threshold_or_filter(*)>(_a[2]))); break;
        case 2: _t->reemitStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (fCheckBox::*)(maskTypes , int , int , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fCheckBox::filterValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (fCheckBox::*)(maskTypes , threshold_or_filter );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fCheckBox::fStateChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fCheckBox::staticMetaObject = { {
    &QCheckBox::staticMetaObject,
    qt_meta_stringdata_fCheckBox.data,
    qt_meta_data_fCheckBox,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fCheckBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fCheckBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fCheckBox.stringdata0))
        return static_cast<void*>(this);
    return QCheckBox::qt_metacast(_clname);
}

int fCheckBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCheckBox::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void fCheckBox::filterValueChanged(maskTypes _t1, int _t2, int _t3, threshold_or_filter _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void fCheckBox::fStateChanged(maskTypes _t1, threshold_or_filter _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
