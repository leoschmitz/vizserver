/****************************************************************************
** Meta object code from reading C++ file 'VizIDatabase.h'
**
** Created: Tue 29. Sep 20:59:41 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizIDatabase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizIDatabase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizIDatabase[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizIDatabase[] = {
    "viz::server::VizIDatabase\0"
};

const QMetaObject viz::server::VizIDatabase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizIDatabase,
      qt_meta_data_viz__server__VizIDatabase, 0 }
};

const QMetaObject *viz::server::VizIDatabase::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizIDatabase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizIDatabase))
        return static_cast<void*>(const_cast< VizIDatabase*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizIDatabase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
