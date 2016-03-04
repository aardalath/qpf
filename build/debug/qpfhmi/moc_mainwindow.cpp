/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qpfhmi/mainwindow.h"
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
struct qt_meta_stringdata_QPF__MainWindow_t {
    QByteArrayData data[31];
    char stringdata[431];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QPF__MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QPF__MainWindow_t qt_meta_stringdata_QPF__MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QPF::MainWindow"
QT_MOC_LITERAL(1, 16, 15), // "goToOperational"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "commandSystem"
QT_MOC_LITERAL(4, 47, 20), // "transitToOperational"
QT_MOC_LITERAL(5, 68, 10), // "sendInData"
QT_MOC_LITERAL(6, 79, 21), // "prepareSendMultInData"
QT_MOC_LITERAL(7, 101, 22), // "selectInDataParamsFile"
QT_MOC_LITERAL(8, 124, 25), // "prepareSendInDataFromFile"
QT_MOC_LITERAL(9, 150, 14), // "sendMultInData"
QT_MOC_LITERAL(10, 165, 18), // "sendInDataFromFile"
QT_MOC_LITERAL(11, 184, 21), // "stopSendingMultInData"
QT_MOC_LITERAL(12, 206, 17), // "handleFinishedHMI"
QT_MOC_LITERAL(13, 224, 15), // "checkForTaskRes"
QT_MOC_LITERAL(14, 240, 11), // "showTaskRes"
QT_MOC_LITERAL(15, 252, 24), // "showTaskMonitContextMenu"
QT_MOC_LITERAL(16, 277, 1), // "p"
QT_MOC_LITERAL(17, 279, 11), // "showWorkDir"
QT_MOC_LITERAL(18, 291, 15), // "displayTaskInfo"
QT_MOC_LITERAL(19, 307, 18), // "dumpTaskInfoToTree"
QT_MOC_LITERAL(20, 326, 8), // "taskName"
QT_MOC_LITERAL(21, 335, 12), // "Json::Value&"
QT_MOC_LITERAL(22, 348, 1), // "v"
QT_MOC_LITERAL(23, 350, 12), // "QTreeWidget*"
QT_MOC_LITERAL(24, 363, 1), // "t"
QT_MOC_LITERAL(25, 365, 10), // "dumpToTree"
QT_MOC_LITERAL(26, 376, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(27, 393, 12), // "setDebugInfo"
QT_MOC_LITERAL(28, 406, 1), // "b"
QT_MOC_LITERAL(29, 408, 20), // "sortTaskViewByColumn"
QT_MOC_LITERAL(30, 429, 1) // "c"

    },
    "QPF::MainWindow\0goToOperational\0\0"
    "commandSystem\0transitToOperational\0"
    "sendInData\0prepareSendMultInData\0"
    "selectInDataParamsFile\0prepareSendInDataFromFile\0"
    "sendMultInData\0sendInDataFromFile\0"
    "stopSendingMultInData\0handleFinishedHMI\0"
    "checkForTaskRes\0showTaskRes\0"
    "showTaskMonitContextMenu\0p\0showWorkDir\0"
    "displayTaskInfo\0dumpTaskInfoToTree\0"
    "taskName\0Json::Value&\0v\0QTreeWidget*\0"
    "t\0dumpToTree\0QTreeWidgetItem*\0"
    "setDebugInfo\0b\0sortTaskViewByColumn\0"
    "c"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPF__MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  115,    2, 0x0a /* Public */,
       4,    0,  116,    2, 0x09 /* Protected */,
       5,    0,  117,    2, 0x09 /* Protected */,
       6,    0,  118,    2, 0x09 /* Protected */,
       7,    0,  119,    2, 0x09 /* Protected */,
       8,    0,  120,    2, 0x09 /* Protected */,
       9,    0,  121,    2, 0x09 /* Protected */,
      10,    0,  122,    2, 0x09 /* Protected */,
      11,    0,  123,    2, 0x09 /* Protected */,
      12,    0,  124,    2, 0x09 /* Protected */,
      13,    0,  125,    2, 0x09 /* Protected */,
      14,    0,  126,    2, 0x09 /* Protected */,
      15,    1,  127,    2, 0x09 /* Protected */,
      17,    0,  130,    2, 0x09 /* Protected */,
      18,    0,  131,    2, 0x09 /* Protected */,
      19,    3,  132,    2, 0x09 /* Protected */,
      25,    2,  139,    2, 0x09 /* Protected */,
      27,    1,  144,    2, 0x09 /* Protected */,
      29,    1,  147,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 21, 0x80000000 | 23,   20,   22,   24,
    QMetaType::Void, 0x80000000 | 21, 0x80000000 | 26,   22,   24,
    QMetaType::Void, QMetaType::Bool,   28,
    QMetaType::Void, QMetaType::Int,   30,

       0        // eod
};

void QPF::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->goToOperational(); break;
        case 1: _t->commandSystem(); break;
        case 2: _t->transitToOperational(); break;
        case 3: _t->sendInData(); break;
        case 4: _t->prepareSendMultInData(); break;
        case 5: _t->selectInDataParamsFile(); break;
        case 6: _t->prepareSendInDataFromFile(); break;
        case 7: _t->sendMultInData(); break;
        case 8: _t->sendInDataFromFile(); break;
        case 9: _t->stopSendingMultInData(); break;
        case 10: _t->handleFinishedHMI(); break;
        case 11: _t->checkForTaskRes(); break;
        case 12: _t->showTaskRes(); break;
        case 13: _t->showTaskMonitContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 14: _t->showWorkDir(); break;
        case 15: _t->displayTaskInfo(); break;
        case 16: _t->dumpTaskInfoToTree((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Json::Value(*)>(_a[2])),(*reinterpret_cast< QTreeWidget*(*)>(_a[3]))); break;
        case 17: _t->dumpToTree((*reinterpret_cast< Json::Value(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 18: _t->setDebugInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->sortTaskViewByColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTreeWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::goToOperational)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QPF::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QPF__MainWindow.data,
      qt_meta_data_QPF__MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QPF::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPF::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QPF__MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QPF::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void QPF::MainWindow::goToOperational()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
