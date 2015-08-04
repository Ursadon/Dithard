/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
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
QT_MOC_LITERAL(6, 84, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(7, 106, 17), // "crc_error_timeout"
QT_MOC_LITERAL(8, 124, 10), // "send_queue"
QT_MOC_LITERAL(9, 135, 8), // "get_adc1"
QT_MOC_LITERAL(10, 144, 9), // "get_range"
QT_MOC_LITERAL(11, 154, 10), // "T1_timeout"
QT_MOC_LITERAL(12, 165, 10), // "T2_timeout"
QT_MOC_LITERAL(13, 176, 24), // "on_checkBox_stateChanged"
QT_MOC_LITERAL(14, 201, 4), // "arg1"
QT_MOC_LITERAL(15, 206, 23), // "on_cb_acks_stateChanged"
QT_MOC_LITERAL(16, 230, 26), // "on_checkBox_2_stateChanged"
QT_MOC_LITERAL(17, 257, 19), // "on_rbSerial_clicked"
QT_MOC_LITERAL(18, 277, 7), // "checked"
QT_MOC_LITERAL(19, 285, 15) // "on_rbIP_clicked"

    },
    "MainWindow\0on_pbComPortOpen_clicked\0"
    "\0readRequest\0on_battery_main_valueChanged\0"
    "value\0on_pushButton_clicked\0"
    "crc_error_timeout\0send_queue\0get_adc1\0"
    "get_range\0T1_timeout\0T2_timeout\0"
    "on_checkBox_stateChanged\0arg1\0"
    "on_cb_acks_stateChanged\0"
    "on_checkBox_2_stateChanged\0"
    "on_rbSerial_clicked\0checked\0on_rbIP_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    1,   91,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    1,  101,    2, 0x08 /* Private */,
      15,    1,  104,    2, 0x08 /* Private */,
      16,    1,  107,    2, 0x08 /* Private */,
      17,    1,  110,    2, 0x08 /* Private */,
      19,    1,  113,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Bool,   18,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pbComPortOpen_clicked(); break;
        case 1: _t->readRequest(); break;
        case 2: _t->on_battery_main_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->crc_error_timeout(); break;
        case 5: _t->send_queue(); break;
        case 6: _t->get_adc1(); break;
        case 7: _t->get_range(); break;
        case 8: _t->T1_timeout(); break;
        case 9: _t->T2_timeout(); break;
        case 10: _t->on_checkBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_cb_acks_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_rbSerial_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->on_rbIP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
