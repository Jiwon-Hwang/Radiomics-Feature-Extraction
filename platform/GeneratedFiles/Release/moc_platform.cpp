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
    QByteArrayData data[25];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CPlatform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CPlatform_t qt_meta_stringdata_CPlatform = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CPlatform"
QT_MOC_LITERAL(1, 10, 18), // "slotDataScanFinish"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "slotDataProgress"
QT_MOC_LITERAL(4, 47, 11), // "nCurrentIdx"
QT_MOC_LITERAL(5, 59, 11), // "nMaximumIdx"
QT_MOC_LITERAL(6, 71, 20), // "addFileDirectoryItem"
QT_MOC_LITERAL(7, 92, 9), // "seriesIdx"
QT_MOC_LITERAL(8, 102, 9), // "showImage"
QT_MOC_LITERAL(9, 112, 9), // "nSliceIdx"
QT_MOC_LITERAL(10, 122, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(11, 139, 4), // "item"
QT_MOC_LITERAL(12, 144, 6), // "column"
QT_MOC_LITERAL(13, 151, 17), // "scrollChangeImage"
QT_MOC_LITERAL(14, 169, 6), // "nValue"
QT_MOC_LITERAL(15, 176, 13), // "setFilterMode"
QT_MOC_LITERAL(16, 190, 21), // "setCheckedFamilyState"
QT_MOC_LITERAL(17, 212, 15), // "checkReadyToRun"
QT_MOC_LITERAL(18, 228, 9), // "selectAll"
QT_MOC_LITERAL(19, 238, 7), // "checked"
QT_MOC_LITERAL(20, 246, 20), // "checkFeatureBoxState"
QT_MOC_LITERAL(21, 267, 14), // "checkBinOption"
QT_MOC_LITERAL(22, 282, 9), // "showPopUp"
QT_MOC_LITERAL(23, 292, 6), // "sender"
QT_MOC_LITERAL(24, 299, 3) // "run"

    },
    "CPlatform\0slotDataScanFinish\0\0"
    "slotDataProgress\0nCurrentIdx\0nMaximumIdx\0"
    "addFileDirectoryItem\0seriesIdx\0showImage\0"
    "nSliceIdx\0QTreeWidgetItem*\0item\0column\0"
    "scrollChangeImage\0nValue\0setFilterMode\0"
    "setCheckedFamilyState\0checkReadyToRun\0"
    "selectAll\0checked\0checkFeatureBoxState\0"
    "checkBinOption\0showPopUp\0sender\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPlatform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    2,   85,    2, 0x0a /* Public */,
       6,    1,   90,    2, 0x0a /* Public */,
       8,    1,   93,    2, 0x0a /* Public */,
       8,    2,   96,    2, 0x0a /* Public */,
      13,    1,  101,    2, 0x0a /* Public */,
      15,    0,  104,    2, 0x0a /* Public */,
      16,    0,  105,    2, 0x0a /* Public */,
      17,    0,  106,    2, 0x0a /* Public */,
      18,    1,  107,    2, 0x0a /* Public */,
      20,    0,  110,    2, 0x0a /* Public */,
      21,    0,  111,    2, 0x0a /* Public */,
      22,    1,  112,    2, 0x0a /* Public */,
      24,    0,  115,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,   23,
    QMetaType::Void,

       0        // eod
};

void CPlatform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPlatform *_t = static_cast<CPlatform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotDataScanFinish(); break;
        case 1: _t->slotDataProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->addFileDirectoryItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->showImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->showImage((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->scrollChangeImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setFilterMode(); break;
        case 7: _t->setCheckedFamilyState(); break;
        case 8: { bool _r = _t->checkReadyToRun();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->selectAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->checkFeatureBoxState(); break;
        case 11: _t->checkBinOption(); break;
        case 12: _t->showPopUp((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 13: _t->run(); break;
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
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
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
