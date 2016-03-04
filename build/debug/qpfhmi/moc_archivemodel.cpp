/****************************************************************************
** Meta object code from reading C++ file 'archivemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qpfhmi/archivemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'archivemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QPF__ArchiveModel_t {
    QByteArrayData data[3];
    char stringdata[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QPF__ArchiveModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QPF__ArchiveModel_t qt_meta_stringdata_QPF__ArchiveModel = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QPF::ArchiveModel"
QT_MOC_LITERAL(1, 18, 10), // "updateView"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "QPF::ArchiveModel\0updateView\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPF__ArchiveModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QPF::ArchiveModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ArchiveModel *_t = static_cast<ArchiveModel *>(_o);
        switch (_id) {
        case 0: _t->updateView(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QPF::ArchiveModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QPF__ArchiveModel.data,
      qt_meta_data_QPF__ArchiveModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QPF::ArchiveModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPF::ArchiveModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QPF__ArchiveModel.stringdata))
        return static_cast<void*>(const_cast< ArchiveModel*>(this));
    return QObject::qt_metacast(_clname);
}

int QPF::ArchiveModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
