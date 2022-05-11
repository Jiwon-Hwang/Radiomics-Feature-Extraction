/****************************************************************************
** Meta object code from reading C++ file 'data.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../data.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'data.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CData_t {
    QByteArrayData data[13];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CData_t qt_meta_stringdata_CData = {
    {
QT_MOC_LITERAL(0, 0, 5), // "CData"
QT_MOC_LITERAL(1, 6, 15), // "signalReadImage"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 24), // "std::vector<std::string>"
QT_MOC_LITERAL(4, 48, 6), // "sPaths"
QT_MOC_LITERAL(5, 55, 14), // "bReadRecursive"
QT_MOC_LITERAL(6, 70, 9), // "bScanOnly"
QT_MOC_LITERAL(7, 80, 12), // "nClearOption"
QT_MOC_LITERAL(8, 93, 29), // "std::function<void(int,int)>*"
QT_MOC_LITERAL(9, 123, 9), // "pCallback"
QT_MOC_LITERAL(10, 133, 20), // "signalDataScanFinish"
QT_MOC_LITERAL(11, 154, 18), // "signalDataProgress"
QT_MOC_LITERAL(12, 173, 13) // "slotReadImage"

    },
    "CData\0signalReadImage\0\0std::vector<std::string>\0"
    "sPaths\0bReadRecursive\0bScanOnly\0"
    "nClearOption\0std::function<void(int,int)>*\0"
    "pCallback\0signalDataScanFinish\0"
    "signalDataProgress\0slotReadImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   39,    2, 0x06 /* Public */,
      10,    0,   50,    2, 0x06 /* Public */,
      11,    2,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   56,    2, 0x0a /* Public */,
      12,    5,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, 0x80000000 | 8,    4,    5,    6,    7,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, 0x80000000 | 8,    4,    5,    6,    7,    9,

       0        // eod
};

void CData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CData *_t = static_cast<CData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalReadImage((*reinterpret_cast< std::vector<std::string>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< std::function<void(int,int)>*(*)>(_a[5]))); break;
        case 1: _t->signalDataScanFinish(); break;
        case 2: _t->signalDataProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->slotReadImage(); break;
        case 4: _t->slotReadImage((*reinterpret_cast< std::vector<std::string>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< std::function<void(int,int)>*(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CData::*_t)(std::vector<std::string> , bool , bool , int , std::function<void(int,int)> * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CData::signalReadImage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CData::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CData::signalDataScanFinish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CData::*_t)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CData::signalDataProgress)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject CData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CData.data,
      qt_meta_data_CData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CData::signalReadImage(std::vector<std::string> _t1, bool _t2, bool _t3, int _t4, std::function<void(int,int)> * _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CData::signalDataScanFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CData::signalDataProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
