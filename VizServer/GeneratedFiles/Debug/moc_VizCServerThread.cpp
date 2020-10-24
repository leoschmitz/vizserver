/****************************************************************************
** Meta object code from reading C++ file 'VizCServerThread.h'
**
** Created: Tue 16. Mar 16:54:04 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCServerThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCServerThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCServerThread[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   31,   30,   30, 0x05,
      84,   74,   30,   30, 0x05,
     119,  106,   30,   30, 0x05,
     167,  149,   30,   30, 0x05,
     216,  203,   30,   30, 0x05,
     253,  246,   30,   30, 0x25,

 // slots: signature, parameters, type, tag, flags
     278,   30,   30,   30, 0x08,
     295,   30,   30,   30, 0x08,
     312,   30,   30,   30, 0x08,
     334,   30,   30,   30, 0x08,
     386,  356,   30,   30, 0x08,
     422,   30,   30,   30, 0x08,
     477,  464,   30,   30, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCServerThread[] = {
    "viz::server::VizCServerThread\0\0"
    "socketError\0error(QTcpSocket::SocketError)\0"
    "sessionId\0closeSession(QString)\0"
    "sessionId,ip\0startSession(QString,QString)\0"
    "user,sessionId,ip\0onlineUser(QString,QString,QString)\0"
    "symbol,input\0realTimeRequest(QString,bool)\0"
    "symbol\0realTimeRequest(QString)\0"
    "sslReceiveData()\0tcpReceiveData()\0"
    "sslConnectionClosed()\0tcpConnectionClosed()\0"
    "socketDescriptor,ip,sessionId\0"
    "tcpStartSocket(int,QString,QString)\0"
    "socketError(QAbstractSocket::SocketError)\0"
    "symbol,msgId\0receiveTick(QString,QString)\0"
};

const QMetaObject viz::server::VizCServerThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_viz__server__VizCServerThread,
      qt_meta_data_viz__server__VizCServerThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCServerThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCServerThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
        case 3: onlineUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: realTimeRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: realTimeRequest((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: sslReceiveData(); break;
        case 7: tcpReceiveData(); break;
        case 8: sslConnectionClosed(); break;
        case 9: tcpConnectionClosed(); break;
        case 10: tcpStartSocket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 12: receiveTick((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 13;
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

// SIGNAL 3
void viz::server::VizCServerThread::onlineUser(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void viz::server::VizCServerThread::realTimeRequest(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
