/****************************************************************************
** Meta object code from reading C++ file 'VizCMegabolsaMsgParser.h'
**
** Created: Tue 16. Mar 15:35:05 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCMegabolsaMsgParser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCMegabolsaMsgParser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCMegabolsaMsgParser[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   36,   36,   36, 0x05,
      57,   36,   36,   36, 0x05,
      72,   36,   36,   36, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCMegabolsaMsgParser[] = {
    "viz::server::VizCMegabolsaMsgParser\0"
    "\0startOfTradingDay()\0closingOfDay()\0"
    "systemOnTheAirWarning()\0"
};

const QMetaObject viz::server::VizCMegabolsaMsgParser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCMegabolsaMsgParser,
      qt_meta_data_viz__server__VizCMegabolsaMsgParser, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCMegabolsaMsgParser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCMegabolsaMsgParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *viz::server::VizCMegabolsaMsgParser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCMegabolsaMsgParser))
        return static_cast<void*>(const_cast< VizCMegabolsaMsgParser*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizCMegabolsaMsgParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startOfTradingDay(); break;
        case 1: closingOfDay(); break;
        case 2: systemOnTheAirWarning(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCMegabolsaMsgParser::startOfTradingDay()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void viz::server::VizCMegabolsaMsgParser::closingOfDay()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void viz::server::VizCMegabolsaMsgParser::systemOnTheAirWarning()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
