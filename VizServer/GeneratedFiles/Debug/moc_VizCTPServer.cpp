/****************************************************************************
** Meta object code from reading C++ file 'VizCTPServer.h'
**
** Created: Tue 16. Mar 12:47:50 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCTPServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCTPServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCEncryptedTcpServer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

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

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCEncryptedTcpServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCEncryptedTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
      69,   39,   38,   38, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,  117,   38,   38, 0x08,
     170,  160,   38,   38, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCUnencryptedTcpServer[] = {
    "viz::server::VizCUnencryptedTcpServer\0"
    "\0socketDescriptor,ip,sessionId\0"
    "startUnencryptedConnection(int,QString,QString)\0"
    "sessionId,ip\0addSessionId(QString,QString)\0"
    "sessionId\0removeSessionId(QString)\0"
};

const QMetaObject viz::server::VizCUnencryptedTcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_viz__server__VizCUnencryptedTcpServer,
      qt_meta_data_viz__server__VizCUnencryptedTcpServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCUnencryptedTcpServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCUnencryptedTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
        case 0: startUnencryptedConnection((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: addSessionId((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: removeSessionId((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCUnencryptedTcpServer::startUnencryptedConnection(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_viz__server__VizCTPServer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   27,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   80,   26,   26, 0x08,
     143,  133,   26,   26, 0x08,
     165,   26,   26,   26, 0x08,
     181,   27,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCTPServer[] = {
    "viz::server::VizCTPServer\0\0socketDescriptor\0"
    "setThreadUnencryptedConnection(int)\0"
    "user,sessionId,ip\0addClient(QString,QString,QString)\0"
    "sessionId\0removeClient(QString)\0"
    "threadStopped()\0startThread(int)\0"
};

const QMetaObject viz::server::VizCTPServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCTPServer,
      qt_meta_data_viz__server__VizCTPServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCTPServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCTPServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
        case 1: addClient((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: removeClient((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: threadStopped(); break;
        case 4: startThread((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
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
