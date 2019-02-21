/****************************************************************************
** Meta object code from reading C++ file 'photoviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../InstaDam/photoviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'photoviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PhotoViewer_t {
    QByteArrayData data[11];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PhotoViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PhotoViewer_t qt_meta_stringdata_PhotoViewer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PhotoViewer"
QT_MOC_LITERAL(1, 12, 12), // "photoClicked"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "zoomed"
QT_MOC_LITERAL(4, 33, 11), // "changedMask"
QT_MOC_LITERAL(5, 45, 9), // "maskTypes"
QT_MOC_LITERAL(6, 55, 11), // "loadedPhoto"
QT_MOC_LITERAL(7, 67, 22), // "zoomedInADifferentView"
QT_MOC_LITERAL(8, 90, 9), // "setImMask"
QT_MOC_LITERAL(9, 100, 19), // "threshold_or_filter"
QT_MOC_LITERAL(10, 120, 4) // "thof"

    },
    "PhotoViewer\0photoClicked\0\0zoomed\0"
    "changedMask\0maskTypes\0loadedPhoto\0"
    "zoomedInADifferentView\0setImMask\0"
    "threshold_or_filter\0thof"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PhotoViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    3,   52,    2, 0x06 /* Public */,
       4,    1,   59,    2, 0x06 /* Public */,
       6,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    3,   63,    2, 0x0a /* Public */,
       8,    2,   70,    2, 0x0a /* Public */,
       8,    1,   75,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::QPointF,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::QPointF,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 9,    2,   10,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void PhotoViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PhotoViewer *_t = static_cast<PhotoViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->photoClicked((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->zoomed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3]))); break;
        case 2: _t->changedMask((*reinterpret_cast< maskTypes(*)>(_a[1]))); break;
        case 3: _t->loadedPhoto(); break;
        case 4: _t->zoomedInADifferentView((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3]))); break;
        case 5: _t->setImMask((*reinterpret_cast< maskTypes(*)>(_a[1])),(*reinterpret_cast< threshold_or_filter(*)>(_a[2]))); break;
        case 6: _t->setImMask((*reinterpret_cast< maskTypes(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PhotoViewer::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PhotoViewer::photoClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PhotoViewer::*)(int , float , QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PhotoViewer::zoomed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PhotoViewer::*)(maskTypes );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PhotoViewer::changedMask)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PhotoViewer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PhotoViewer::loadedPhoto)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PhotoViewer::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_PhotoViewer.data,
      qt_meta_data_PhotoViewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PhotoViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PhotoViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PhotoViewer.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int PhotoViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PhotoViewer::photoClicked(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PhotoViewer::zoomed(int _t1, float _t2, QPointF _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PhotoViewer::changedMask(maskTypes _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PhotoViewer::loadedPhoto()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
