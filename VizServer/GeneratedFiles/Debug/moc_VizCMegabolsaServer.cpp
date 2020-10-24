/****************************************************************************
** Meta object code from reading C++ file 'VizCMegabolsaServer.h'
**
** Created: Tue 16. Mar 16:54:04 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCMegabolsaServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCMegabolsaServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCMegabolsaServer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   34,   33,   33, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   81,   33,   33, 0x0a,
     144,  137,   33,   33, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCMegabolsaServer[] = {
    "viz::server::VizCMegabolsaServer\0\0"
    "symbol,msgId\0sendTickToThread(QString,QString)\0"
    "symbol,input\0receiveThreadRealTimeRequest(QString,bool)\0"
    "id,msg\0receiveProxyDiffMessage(QString,QString)\0"
};

const QMetaObject viz::server::VizCMegabolsaServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCMegabolsaServer,
      qt_meta_data_viz__server__VizCMegabolsaServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCMegabolsaServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCMegabolsaServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *viz::server::VizCMegabolsaServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCMegabolsaServer))
        return static_cast<void*>(const_cast< VizCMegabolsaServer*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizCMegabolsaServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendTickToThread((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: receiveThreadRealTimeRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: receiveProxyDiffMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCMegabolsaServer::sendTickToThread(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
