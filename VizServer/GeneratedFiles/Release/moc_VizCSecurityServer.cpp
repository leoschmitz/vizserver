/****************************************************************************
** Meta object code from reading C++ file 'VizCSecurityServer.h'
**
** Created: Tue 20. Oct 11:58:56 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCSecurityServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCSecurityServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCSendSecurityThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      49,   37,   36,   36, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   36,   36,   36, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCSendSecurityThread[] = {
    "viz::server::VizCSendSecurityThread\0"
    "\0socketError\0error(QTcpSocket::SocketError)\0"
    "receiveData()\0"
};

const QMetaObject viz::server::VizCSendSecurityThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_viz__server__VizCSendSecurityThread,
      qt_meta_data_viz__server__VizCSendSecurityThread, 0 }
};

const QMetaObject *viz::server::VizCSendSecurityThread::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCSendSecurityThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCSendSecurityThread))
        return static_cast<void*>(const_cast< VizCSendSecurityThread*>(this));
    return QThread::qt_metacast(_clname);
}

int viz::server::VizCSendSecurityThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error((*reinterpret_cast< QTcpSocket::SocketError(*)>(_a[1]))); break;
        case 1: receiveData(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCSendSecurityThread::error(QTcpSocket::SocketError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_viz__server__VizCSecurityServer[] = {

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

static const char qt_meta_stringdata_viz__server__VizCSecurityServer[] = {
    "viz::server::VizCSecurityServer\0"
};

const QMetaObject viz::server::VizCSecurityServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_viz__server__VizCSecurityServer,
      qt_meta_data_viz__server__VizCSecurityServer, 0 }
};

const QMetaObject *viz::server::VizCSecurityServer::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCSecurityServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCSecurityServer))
        return static_cast<void*>(const_cast< VizCSecurityServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int viz::server::VizCSecurityServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
