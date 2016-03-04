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
    QByteArrayData data[44];
    char stringdata[594];
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
QT_MOC_LITERAL(3, 33, 19), // "stopSendingMessages"
QT_MOC_LITERAL(4, 53, 13), // "commandSystem"
QT_MOC_LITERAL(5, 67, 20), // "processPendingEvents"
QT_MOC_LITERAL(6, 88, 20), // "transitToOperational"
QT_MOC_LITERAL(7, 109, 10), // "sendInData"
QT_MOC_LITERAL(8, 120, 21), // "prepareSendMultInData"
QT_MOC_LITERAL(9, 142, 25), // "prepareSendInDataFromFile"
QT_MOC_LITERAL(10, 168, 22), // "selectInDataParamsFile"
QT_MOC_LITERAL(11, 191, 21), // "stopSendingMultInData"
QT_MOC_LITERAL(12, 213, 10), // "sentInData"
QT_MOC_LITERAL(13, 224, 8), // "msgsLeft"
QT_MOC_LITERAL(14, 233, 15), // "endOfInDataMsgs"
QT_MOC_LITERAL(15, 249, 17), // "handleFinishedHMI"
QT_MOC_LITERAL(16, 267, 15), // "checkForTaskRes"
QT_MOC_LITERAL(17, 283, 11), // "showTaskRes"
QT_MOC_LITERAL(18, 295, 20), // "updateTasksMonitTree"
QT_MOC_LITERAL(19, 316, 5), // "nCols"
QT_MOC_LITERAL(20, 322, 18), // "initTasksMonitTree"
QT_MOC_LITERAL(21, 341, 22), // "updateAgentsMonitPanel"
QT_MOC_LITERAL(22, 364, 24), // "showTaskMonitContextMenu"
QT_MOC_LITERAL(23, 389, 1), // "p"
QT_MOC_LITERAL(24, 391, 11), // "showWorkDir"
QT_MOC_LITERAL(25, 403, 15), // "displayTaskInfo"
QT_MOC_LITERAL(26, 419, 9), // "pauseTask"
QT_MOC_LITERAL(27, 429, 10), // "resumeTask"
QT_MOC_LITERAL(28, 440, 8), // "stopTask"
QT_MOC_LITERAL(29, 449, 12), // "runDockerCmd"
QT_MOC_LITERAL(30, 462, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(31, 479, 4), // "item"
QT_MOC_LITERAL(32, 484, 3), // "cmd"
QT_MOC_LITERAL(33, 488, 18), // "dumpTaskInfoToTree"
QT_MOC_LITERAL(34, 507, 8), // "taskName"
QT_MOC_LITERAL(35, 516, 11), // "Json::Value"
QT_MOC_LITERAL(36, 528, 1), // "v"
QT_MOC_LITERAL(37, 530, 12), // "QTreeWidget*"
QT_MOC_LITERAL(38, 543, 1), // "t"
QT_MOC_LITERAL(39, 545, 10), // "dumpToTree"
QT_MOC_LITERAL(40, 556, 12), // "setDebugInfo"
QT_MOC_LITERAL(41, 569, 1), // "b"
QT_MOC_LITERAL(42, 571, 20), // "sortTaskViewByColumn"
QT_MOC_LITERAL(43, 592, 1) // "c"

    },
    "QPF::MainWindow\0goToOperational\0\0"
    "stopSendingMessages\0commandSystem\0"
    "processPendingEvents\0transitToOperational\0"
    "sendInData\0prepareSendMultInData\0"
    "prepareSendInDataFromFile\0"
    "selectInDataParamsFile\0stopSendingMultInData\0"
    "sentInData\0msgsLeft\0endOfInDataMsgs\0"
    "handleFinishedHMI\0checkForTaskRes\0"
    "showTaskRes\0updateTasksMonitTree\0nCols\0"
    "initTasksMonitTree\0updateAgentsMonitPanel\0"
    "showTaskMonitContextMenu\0p\0showWorkDir\0"
    "displayTaskInfo\0pauseTask\0resumeTask\0"
    "stopTask\0runDockerCmd\0QTreeWidgetItem*\0"
    "item\0cmd\0dumpTaskInfoToTree\0taskName\0"
    "Json::Value\0v\0QTreeWidget*\0t\0dumpToTree\0"
    "setDebugInfo\0b\0sortTaskViewByColumn\0"
    "c"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPF__MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  159,    2, 0x06 /* Public */,
       3,    0,  160,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  161,    2, 0x0a /* Public */,
       5,    0,  162,    2, 0x0a /* Public */,
       6,    0,  163,    2, 0x09 /* Protected */,
       7,    0,  164,    2, 0x09 /* Protected */,
       8,    0,  165,    2, 0x09 /* Protected */,
       9,    0,  166,    2, 0x09 /* Protected */,
      10,    0,  167,    2, 0x09 /* Protected */,
      11,    0,  168,    2, 0x09 /* Protected */,
      12,    1,  169,    2, 0x09 /* Protected */,
      14,    0,  172,    2, 0x09 /* Protected */,
      15,    0,  173,    2, 0x09 /* Protected */,
      16,    0,  174,    2, 0x09 /* Protected */,
      17,    0,  175,    2, 0x09 /* Protected */,
      18,    1,  176,    2, 0x09 /* Protected */,
      20,    1,  179,    2, 0x09 /* Protected */,
      21,    0,  182,    2, 0x09 /* Protected */,
      22,    1,  183,    2, 0x09 /* Protected */,
      24,    0,  186,    2, 0x09 /* Protected */,
      25,    0,  187,    2, 0x09 /* Protected */,
      26,    0,  188,    2, 0x09 /* Protected */,
      27,    0,  189,    2, 0x09 /* Protected */,
      28,    0,  190,    2, 0x09 /* Protected */,
      29,    2,  191,    2, 0x09 /* Protected */,
      33,    3,  196,    2, 0x09 /* Protected */,
      39,    2,  203,    2, 0x09 /* Protected */,
      40,    1,  208,    2, 0x09 /* Protected */,
      42,    1,  211,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
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
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 30, QMetaType::QString,   31,   32,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 35, 0x80000000 | 37,   34,   36,   38,
    QMetaType::Void, 0x80000000 | 35, 0x80000000 | 30,   36,   38,
    QMetaType::Void, QMetaType::Bool,   41,
    QMetaType::Void, QMetaType::Int,   43,

       0        // eod
};

void QPF::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->goToOperational(); break;
        case 1: _t->stopSendingMessages(); break;
        case 2: _t->commandSystem(); break;
        case 3: _t->processPendingEvents(); break;
        case 4: _t->transitToOperational(); break;
        case 5: _t->sendInData(); break;
        case 6: _t->prepareSendMultInData(); break;
        case 7: _t->prepareSendInDataFromFile(); break;
        case 8: _t->selectInDataParamsFile(); break;
        case 9: _t->stopSendingMultInData(); break;
        case 10: _t->sentInData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->endOfInDataMsgs(); break;
        case 12: _t->handleFinishedHMI(); break;
        case 13: _t->checkForTaskRes(); break;
        case 14: _t->showTaskRes(); break;
        case 15: _t->updateTasksMonitTree((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->initTasksMonitTree((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->updateAgentsMonitPanel(); break;
        case 18: _t->showTaskMonitContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 19: _t->showWorkDir(); break;
        case 20: _t->displayTaskInfo(); break;
        case 21: _t->pauseTask(); break;
        case 22: _t->resumeTask(); break;
        case 23: _t->stopTask(); break;
        case 24: { bool _r = _t->runDockerCmd((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: _t->dumpTaskInfoToTree((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const Json::Value(*)>(_a[2])),(*reinterpret_cast< QTreeWidget*(*)>(_a[3]))); break;
        case 26: _t->dumpToTree((*reinterpret_cast< const Json::Value(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 27: _t->setDebugInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->sortTaskViewByColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 25:
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
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::stopSendingMessages)) {
                *result = 1;
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
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void QPF::MainWindow::goToOperational()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QPF::MainWindow::stopSendingMessages()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
