/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata[332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 24), // "on_pbComPortOpen_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 11), // "readRequest"
QT_MOC_LITERAL(4, 49, 28), // "on_battery_main_valueChanged"
QT_MOC_LITERAL(5, 78, 5), // "value"
QT_MOC_LITERAL(6, 84, 17), // "crc_error_timeout"
QT_MOC_LITERAL(7, 102, 10), // "send_queue"
QT_MOC_LITERAL(8, 113, 8), // "get_adc1"
QT_MOC_LITERAL(9, 122, 9), // "get_range"
QT_MOC_LITERAL(10, 132, 10), // "T1_timeout"
QT_MOC_LITERAL(11, 143, 10), // "T2_timeout"
QT_MOC_LITERAL(12, 154, 23), // "on_cbRange_stateChanged"
QT_MOC_LITERAL(13, 178, 4), // "arg1"
QT_MOC_LITERAL(14, 183, 23), // "on_cb_acks_stateChanged"
QT_MOC_LITERAL(15, 207, 25), // "on_cbNoQueue_stateChanged"
QT_MOC_LITERAL(16, 233, 19), // "on_rbSerial_clicked"
QT_MOC_LITERAL(17, 253, 15), // "on_rbIP_clicked"
QT_MOC_LITERAL(18, 269, 19), // "on_pbIpOpen_clicked"
QT_MOC_LITERAL(19, 289, 11), // "packet_rcvd"
QT_MOC_LITERAL(20, 301, 6), // "packet"
QT_MOC_LITERAL(21, 308, 23) // "on_pbCustomSend_clicked"

    },
    "MainWindow\0on_pbComPortOpen_clicked\0"
    "\0readRequest\0on_battery_main_valueChanged\0"
    "value\0crc_error_timeout\0send_queue\0"
    "get_adc1\0get_range\0T1_timeout\0T2_timeout\0"
    "on_cbRange_stateChanged\0arg1\0"
    "on_cb_acks_stateChanged\0"
    "on_cbNoQueue_stateChanged\0on_rbSerial_clicked\0"
    "on_rbIP_clicked\0on_pbIpOpen_clicked\0"
    "packet_rcvd\0packet\0on_pbCustomSend_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    0,  100,    2, 0x08 /* Private */,
       4,    1,  101,    2, 0x08 /* Private */,
       6,    0,  104,    2, 0x08 /* Private */,
       7,    0,  105,    2, 0x08 /* Private */,
       8,    0,  106,    2, 0x08 /* Private */,
       9,    0,  107,    2, 0x08 /* Private */,
      10,    0,  108,    2, 0x08 /* Private */,
      11,    0,  109,    2, 0x08 /* Private */,
      12,    1,  110,    2, 0x08 /* Private */,
      14,    1,  113,    2, 0x08 /* Private */,
      15,    1,  116,    2, 0x08 /* Private */,
      16,    0,  119,    2, 0x08 /* Private */,
      17,    0,  120,    2, 0x08 /* Private */,
      18,    0,  121,    2, 0x08 /* Private */,
      19,    1,  122,    2, 0x08 /* Private */,
      21,    0,  125,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   20,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_pbComPortOpen_clicked(); break;
        case 1: _t->readRequest(); break;
        case 2: _t->on_battery_main_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->crc_error_timeout(); break;
        case 4: _t->send_queue(); break;
        case 5: _t->get_adc1(); break;
        case 6: _t->get_range(); break;
        case 7: _t->T1_timeout(); break;
        case 8: _t->T2_timeout(); break;
        case 9: _t->on_cbRange_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_cb_acks_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_cbNoQueue_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_rbSerial_clicked(); break;
        case 13: _t->on_rbIP_clicked(); break;
        case 14: _t->on_pbIpOpen_clicked(); break;
        case 15: _t->packet_rcvd((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 16: _t->on_pbCustomSend_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
