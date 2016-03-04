/****************************************************************************
** Meta object code from reading C++ file 'simindata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qpfhmi/simindata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simindata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QPF__SimInData_t {
    QByteArrayData data[25];
    char stringdata[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QPF__SimInData_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QPF__SimInData_t qt_meta_stringdata_QPF__SimInData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "QPF::SimInData"
QT_MOC_LITERAL(1, 15, 13), // "simInDataSent"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "endOfInData"
QT_MOC_LITERAL(4, 42, 10), // "setHmiNode"
QT_MOC_LITERAL(5, 53, 9), // "HMIProxy*"
QT_MOC_LITERAL(6, 63, 6), // "hmiObj"
QT_MOC_LITERAL(7, 70, 10), // "sendInData"
QT_MOC_LITERAL(8, 81, 5), // "start"
QT_MOC_LITERAL(9, 87, 3), // "end"
QT_MOC_LITERAL(10, 91, 6), // "prodId"
QT_MOC_LITERAL(11, 98, 8), // "prodType"
QT_MOC_LITERAL(12, 107, 7), // "prodVer"
QT_MOC_LITERAL(13, 115, 6), // "status"
QT_MOC_LITERAL(14, 122, 3), // "url"
QT_MOC_LITERAL(15, 126, 14), // "sendMultInData"
QT_MOC_LITERAL(16, 141, 9), // "prodTypes"
QT_MOC_LITERAL(17, 151, 8), // "prodStat"
QT_MOC_LITERAL(18, 160, 4), // "step"
QT_MOC_LITERAL(19, 165, 4), // "freq"
QT_MOC_LITERAL(20, 170, 18), // "sendInDataFromFile"
QT_MOC_LITERAL(21, 189, 16), // "fileInDataParams"
QT_MOC_LITERAL(22, 206, 20), // "actualSendMultInData"
QT_MOC_LITERAL(23, 227, 24), // "actualSendInDataFromFile"
QT_MOC_LITERAL(24, 252, 17) // "stopSendingInData"

    },
    "QPF::SimInData\0simInDataSent\0\0endOfInData\0"
    "setHmiNode\0HMIProxy*\0hmiObj\0sendInData\0"
    "start\0end\0prodId\0prodType\0prodVer\0"
    "status\0url\0sendMultInData\0prodTypes\0"
    "prodStat\0step\0freq\0sendInDataFromFile\0"
    "fileInDataParams\0actualSendMultInData\0"
    "actualSendInDataFromFile\0stopSendingInData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPF__SimInData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       3,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   78,    2, 0x0a /* Public */,
       4,    0,   81,    2, 0x2a /* Public | MethodCloned */,
       7,    7,   82,    2, 0x0a /* Public */,
      15,    6,   97,    2, 0x0a /* Public */,
      15,    5,  110,    2, 0x2a /* Public | MethodCloned */,
      15,    4,  121,    2, 0x2a /* Public | MethodCloned */,
      20,    1,  130,    2, 0x0a /* Public */,
      22,    0,  133,    2, 0x0a /* Public */,
      23,    0,  134,    2, 0x0a /* Public */,
      24,    0,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Bool, QMetaType::QDateTime, QMetaType::QDateTime, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Int, QMetaType::Int,    8,    9,   16,   17,   18,   19,
    QMetaType::Bool, QMetaType::QDateTime, QMetaType::QDateTime, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Int,    8,    9,   16,   17,   18,
    QMetaType::Bool, QMetaType::QDateTime, QMetaType::QDateTime, QMetaType::QStringList, QMetaType::QStringList,    8,    9,   16,   17,
    QMetaType::Bool, QMetaType::QString,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QPF::SimInData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SimInData *_t = static_cast<SimInData *>(_o);
        switch (_id) {
        case 0: _t->simInDataSent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->endOfInData(); break;
        case 2: _t->setHmiNode((*reinterpret_cast< HMIProxy*(*)>(_a[1]))); break;
        case 3: _t->setHmiNode(); break;
        case 4: _t->sendInData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 5: { bool _r = _t->sendMultInData((*reinterpret_cast< QDateTime(*)>(_a[1])),(*reinterpret_cast< QDateTime(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< QStringList(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->sendMultInData((*reinterpret_cast< QDateTime(*)>(_a[1])),(*reinterpret_cast< QDateTime(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< QStringList(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->sendMultInData((*reinterpret_cast< QDateTime(*)>(_a[1])),(*reinterpret_cast< QDateTime(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< QStringList(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->sendInDataFromFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: _t->actualSendMultInData(); break;
        case 10: _t->actualSendInDataFromFile(); break;
        case 11: _t->stopSendingInData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SimInData::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SimInData::simInDataSent)) {
                *result = 0;
            }
        }
        {
            typedef void (SimInData::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SimInData::endOfInData)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject QPF::SimInData::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QPF__SimInData.data,
      qt_meta_data_QPF__SimInData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QPF::SimInData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPF::SimInData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QPF__SimInData.stringdata))
        return static_cast<void*>(const_cast< SimInData*>(this));
    return QObject::qt_metacast(_clname);
}

int QPF::SimInData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QPF::SimInData::simInDataSent(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QPF::SimInData::endOfInData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
