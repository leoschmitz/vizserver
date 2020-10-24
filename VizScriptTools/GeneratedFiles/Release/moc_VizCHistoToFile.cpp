/****************************************************************************
** Meta object code from reading C++ file 'VizCHistoToFile.h'
**
** Created: Thu 11. Feb 18:00:58 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCHistoToFile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCHistoToFile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCHistoToFile[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCHistoToFile[] = {
    "viz::server::VizCHistoToFile\0"
};

const QMetaObject viz::server::VizCHistoToFile::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCHistoToFile,
      qt_meta_data_viz__server__VizCHistoToFile, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCHistoToFile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCHistoToFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *viz::server::VizCHistoToFile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCHistoToFile))
        return static_cast<void*>(const_cast< VizCHistoToFile*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizCHistoToFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
