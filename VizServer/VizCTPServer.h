//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTPSERVERH__
#define __VIZCTPSERVERH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCompany.h"
#include "VizCMegabolsaServer.h"
#include "VizCDatabaseUtils.h"
#include "VizCLog.h"
#include "VizCServerThread.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslSocket>

#include <QMutexLocker>
#include <QString>
#include <QThread>
#include <QMap>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCEncryptedTcpServer : public QTcpServer
{	
	Q_OBJECT
public:
	VizCEncryptedTcpServer(QObject* parent = 0);
	~VizCEncryptedTcpServer();	

protected:
	void incomingConnection(int socketDescriptor);

signals:
	void startThread(int socketDescriptor);
};
//-----------------------------------------------------------------------------
class VizCUnencryptedTcpServer : public QTcpServer
{	
	Q_OBJECT
public:
	VizCUnencryptedTcpServer(QObject* parent = 0);
	~VizCUnencryptedTcpServer();	
private:
	QMap<QString, QString> SessionMapIp;	
	
private slots:
	void addSessionId(QString sessionId, QString ip);
	void removeSessionId(QString sessionId);

protected:
	void incomingConnection(int socketDescriptor);

signals:
	void startUnencryptedConnection(int socketDescriptor, QString ip, QString sessionId);	
};
//-----------------------------------------------------------------------------
class VizCTPServer : public QObject
{
	Q_OBJECT
private:
	VizCMegabolsaServer* MegabolsaServer;
	VizCUnencryptedTcpServer* UnencryptedTcpServer;
	VizCEncryptedTcpServer* EncryptedTcpServer;

	QString generateSessionId();
private slots:	
	void addClient(const QString& user, const QString& sessionId, const QString& ip);
	void removeClient(const QString& sessionId);
	void threadStopped();
	void startThread(int socketDescriptor);
	
signals:
	void setThreadUnencryptedConnection(int socketDescriptor);

public:
	VizCTPServer(QObject* parent = 0);	 
	~VizCTPServer();	 
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif