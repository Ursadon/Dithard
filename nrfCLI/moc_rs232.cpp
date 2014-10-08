/****************************************************************************
** Meta object code from reading C++ file 'rs232.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rs232.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rs232.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RS232_t {
    QByteArrayData data[6];
    char stringdata[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_RS232_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_RS232_t qt_meta_stringdata_RS232 = {
    {
QT_MOC_LITERAL(0, 0, 5),
QT_MOC_LITERAL(1, 6, 13),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 6),
QT_MOC_LITERAL(4, 28, 11),
QT_MOC_LITERAL(5, 40, 11)
    },
    "RS232\0wakepacket_sn\0\0packet\0readRequest\0"
    "packet_rcvd\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RS232[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x08,
       5,    1,   33,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void RS232::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RS232 *_t = static_cast<RS232 *>(_o);
        switch (_id) {
        case 0: _t->wakepacket_sn((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->readRequest(); break;
        case 2: _t->packet_rcvd((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RS232::*_t)(const QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RS232::wakepacket_sn)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject RS232::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RS232.data,
      qt_meta_data_RS232,  qt_static_metacall, 0, 0}
};


const QMetaObject *RS232::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RS232::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RS232.stringdata))
        return static_cast<void*>(const_cast< RS232*>(this));
    return QThread::qt_metacast(_clname);
}

int RS232::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void RS232::wakepacket_sn(const QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
