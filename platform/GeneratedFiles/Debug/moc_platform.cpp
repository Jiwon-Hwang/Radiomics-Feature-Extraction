/****************************************************************************
** Meta object code from reading C++ file 'platform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../platform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'platform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CPlatform_t {
    QByteArrayData data[17];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CPlatform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CPlatform_t qt_meta_stringdata_CPlatform = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CPlatform"
QT_MOC_LITERAL(1, 10, 9), // "showImage"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 9), // "nFrameIdx"
QT_MOC_LITERAL(4, 31, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(5, 48, 4), // "item"
QT_MOC_LITERAL(6, 53, 6), // "column"
QT_MOC_LITERAL(7, 60, 17), // "scrollChangeImage"
QT_MOC_LITERAL(8, 78, 6), // "nValue"
QT_MOC_LITERAL(9, 85, 22), // "calcLocalIntensityPeak"
QT_MOC_LITERAL(10, 108, 6), // "short*"
QT_MOC_LITERAL(11, 115, 8), // "pusImage"
QT_MOC_LITERAL(12, 124, 14), // "unsigned char*"
QT_MOC_LITERAL(13, 139, 7), // "pucMask"
QT_MOC_LITERAL(14, 147, 7), // "nHeight"
QT_MOC_LITERAL(15, 155, 6), // "nWidth"
QT_MOC_LITERAL(16, 162, 3) // "run"

    },
    "CPlatform\0showImage\0\0nFrameIdx\0"
    "QTreeWidgetItem*\0item\0column\0"
    "scrollChangeImage\0nValue\0"
    "calcLocalIntensityPeak\0short*\0pusImage\0"
    "unsigned char*\0pucMask\0nHeight\0nWidth\0"
    "run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPlatform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       1,    2,   42,    2, 0x0a /* Public */,
       7,    1,   47,    2, 0x0a /* Public */,
       9,    4,   50,    2, 0x0a /* Public */,
      16,    0,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Short, 0x80000000 | 10, 0x80000000 | 12, QMetaType::Int, QMetaType::Int,   11,   13,   14,   15,
    QMetaType::Void,

       0        // eod
};

void CPlatform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPlatform *_t = static_cast<CPlatform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showImage((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->scrollChangeImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { short _r = _t->calcLocalIntensityPeak((*reinterpret_cast< short*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< short*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->run(); break;
        default: ;
        }
    }
}

const QMetaObject CPlatform::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CPlatform.data,
      qt_meta_data_CPlatform,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CPlatform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPlatform::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CPlatform.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CPlatform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
struct qt_meta_stringdata_BorderlessMainWindow_t {
    QByteArrayData data[6];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BorderlessMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BorderlessMainWindow_t qt_meta_stringdata_BorderlessMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 20), // "BorderlessMainWindow"
QT_MOC_LITERAL(1, 21, 14), // "slot_minimized"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "slot_restored"
QT_MOC_LITERAL(4, 51, 14), // "slot_maximized"
QT_MOC_LITERAL(5, 66, 11) // "slot_closed"

    },
    "BorderlessMainWindow\0slot_minimized\0"
    "\0slot_restored\0slot_maximized\0slot_closed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BorderlessMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BorderlessMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BorderlessMainWindow *_t = static_cast<BorderlessMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_minimized(); break;
        case 1: _t->slot_restored(); break;
        case 2: _t->slot_maximized(); break;
        case 3: _t->slot_closed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject BorderlessMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BorderlessMainWindow.data,
      qt_meta_data_BorderlessMainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BorderlessMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BorderlessMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BorderlessMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BorderlessMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE