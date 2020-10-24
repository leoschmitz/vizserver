/****************************************************************************
** Meta object code from reading C++ file 'VizCProxyDiff.h'
**
** Created: Tue 16. Mar 14:15:54 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VizCProxyDiff.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VizCProxyDiff.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_viz__server__VizCProxyDiff[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x05,
      44,   38,   27,   27, 0x05,
      66,   27,   27,   27, 0x05,
      86,   79,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
     111,   27,   27,   27, 0x08,
     127,   27,   27,   27, 0x08,
     144,  139,   27,   27, 0x08,
     181,  166,   27,   27, 0x08,
     248,  210,   27,   27, 0x08,
     291,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_viz__server__VizCProxyDiff[] = {
    "viz::server::VizCProxyDiff\0\0Connect()\0"
    "MsgId\0StartRequest(QString)\0Disconnect()\0"
    "id,msg\0message(QString,QString)\0"
    "OnAcceptStart()\0OnConnect()\0Desc\0"
    "OnDisconnect(QString)\0Error,Msg,Desc\0"
    "OnError(int,QString,QString)\0"
    "LastMsgId,SPF_Header,DataPtr,DataSize\0"
    "OnReceiveData(QString,QString,QString,int)\0"
    "updateReceiveMsg()\0"
};

const QMetaObject viz::server::VizCProxyDiff::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_viz__server__VizCProxyDiff,
      qt_meta_data_viz__server__VizCProxyDiff, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &viz::server::VizCProxyDiff::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *viz::server::VizCProxyDiff::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *viz::server::VizCProxyDiff::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_viz__server__VizCProxyDiff))
        return static_cast<void*>(const_cast< VizCProxyDiff*>(this));
    return QObject::qt_metacast(_clname);
}

int viz::server::VizCProxyDiff::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Connect(); break;
        case 1: StartRequest((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: Disconnect(); break;
        case 3: message((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: OnAcceptStart(); break;
        case 5: OnConnect(); break;
        case 6: OnDisconnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: OnError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: OnReceiveData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: updateReceiveMsg(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void viz::server::VizCProxyDiff::Connect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void viz::server::VizCProxyDiff::StartRequest(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void viz::server::VizCProxyDiff::Disconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void viz::server::VizCProxyDiff::message(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
