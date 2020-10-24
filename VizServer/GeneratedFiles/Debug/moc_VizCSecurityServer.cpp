/****************************************************************************
** Meta object code from reading C++ file 'VizCSecurityServer.h'
**
** Created: Tue 3. Nov 14:45:13 2009
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
static const uint qt_meta_data_viz__server__VizCServerThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      43,   31,   30,   30, 0x05,
      84,   74,   30,   30, 0x05,
     119,  106,   30,   30, 0x05,

 // slots: signature, parameters, type, tag, flags
     149,   30,   30,   30, 0x08,
     166,   30,   30,   30, 0x08,
     183,   30,   30,   30, 0x08,
     205,   30,   30,   30, 0x08,
     224,   30,   30,   30, 0x08,
     286,  266,   30,   30, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCServerThread[] = {
    "viz::server::VizCServerThread\0\0"
    "socketError\0error(QTcpSocket::SocketError)\0"
    "sessionId\0closeSession(QString)\0"
    "sessionId,ip\0startSession(QString,QString)\0"
    "sslReceiveData()\0tcpReceiveData()\0"
    "sslConnectionClosed()\0connectionClosed()\0"
    "socketError(QAbstractSocket::SocketError)\0"
    "socketDescriptor,ip\0"
    "startUnencryptedSocket(int,QString)\0"
};

const QMetaObject viz::server::VizCServerThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_viz__server__VizCServerThread,
      qt_meta_data_viz__server__VizCServerThread, 0 }
};

const QMetaObject *viz::server::VizCServerThread::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCServerThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCServerThread))
        return static_cast<void*>(const_cast< VizCServerThread*>(this));
    return QThread::qt_metacast(_clname);
}

int viz::server::VizCServerThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error((*reinterpret_cast< QTcpSocket::SocketError(*)>(_a[1]))); break;
        case 1: closeSession((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: startSession((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: sslReceiveData(); break;
        case 4: tcpReceiveData(); break;
        case 5: sslConnectionClosed(); break;
        case 6: connectionClosed(); break;
        case 7: socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: startUnencryptedSocket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCServerThread::error(QTcpSocket::SocketError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void viz::server::VizCServerThread::closeSession(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void viz::server::VizCServerThread::startSession(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_viz__server__VizCEncryptedTcpServer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      54,   37,   36,   36, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCEncryptedTcpServer[] = {
    "viz::server::VizCEncryptedTcpServer\0"
    "\0socketDescriptor\0startThread(int)\0"
};

const QMetaObject viz::server::VizCEncryptedTcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_viz__server__VizCEncryptedTcpServer,
      qt_meta_data_viz__server__VizCEncryptedTcpServer, 0 }
};

const QMetaObject *viz::server::VizCEncryptedTcpServer::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCEncryptedTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCEncryptedTcpServer))
        return static_cast<void*>(const_cast< VizCEncryptedTcpServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int viz::server::VizCEncryptedTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startThread((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCEncryptedTcpServer::startThread(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_viz__server__VizCUnencryptedTcpServer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      59,   39,   38,   38, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   38,   38,   38, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCUnencryptedTcpServer[] = {
    "viz::server::VizCUnencryptedTcpServer\0"
    "\0socketDescriptor,ip\0"
    "startUnencryptedConnection(int,QString)\0"
    "manageConnection()\0"
};

const QMetaObject viz::server::VizCUnencryptedTcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_viz__server__VizCUnencryptedTcpServer,
      qt_meta_data_viz__server__VizCUnencryptedTcpServer, 0 }
};

const QMetaObject *viz::server::VizCUnencryptedTcpServer::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCUnencryptedTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCUnencryptedTcpServer))
        return static_cast<void*>(const_cast< VizCUnencryptedTcpServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int viz::server::VizCUnencryptedTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startUnencryptedConnection((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: manageConnection(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCUnencryptedTcpServer::startUnencryptedConnection(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_viz__server__VizCTPServer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      44,   27,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   27,   26,   26, 0x08,
     110,   97,   26,   26, 0x08,
     150,  140,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCTPServer[] = {
    "viz::server::VizCTPServer\0\0socketDescriptor\0"
    "setThreadUnencryptedConnection(int)\0"
    "startThread(int)\0sessionId,ip\0"
    "addSessionId(QString,QString)\0sessionId\0"
    "removeSessionId(QString)\0"
};

const QMetaObject viz::server::VizCTPServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCTPServer,
      qt_meta_data_viz__server__VizCTPServer, 0 }
};

const QMetaObject *viz::server::VizCTPServer::metaObject() const
{
    return &staticMetaObject;
}

void *viz::server::VizCTPServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCTPServer))
        return static_cast<void*>(const_cast< VizCTPServer*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizCTPServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setThreadUnencryptedConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: startThread((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: addSessionId((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: removeSessionId((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCTPServer::setThreadUnencryptedConnection(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
