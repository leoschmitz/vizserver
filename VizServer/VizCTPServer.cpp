//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTPServer.h"

#include <QMessageBox>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
// Quaisquer mudanças nessas portas devem ser reescritas também no cliente
const int SslListeningPort = 12000;
const int TcpListeningPort = 12001;
//-----------------------------------------------------------------------------
VizCEncryptedTcpServer::VizCEncryptedTcpServer(QObject* parent) : QTcpServer(parent)
{	
	if (!listen(QHostAddress::Any, SslListeningPort)) 
	{
		vizServerLog() << tr("Servidor SSL. O seguinte erro aconteceu: %1.").arg(errorString());
		close();
		return;
	}		
}
//-----------------------------------------------------------------------------
VizCEncryptedTcpServer::~VizCEncryptedTcpServer() 
{

}
//-----------------------------------------------------------------------------
void VizCEncryptedTcpServer::incomingConnection(int socketDescriptor)
{	
	emit startThread(socketDescriptor);		
}
//-----------------------------------------------------------------------------
VizCUnencryptedTcpServer::VizCUnencryptedTcpServer(QObject* parent) : QTcpServer(parent)
{	
	if (!listen(QHostAddress::Any, TcpListeningPort)) 
	{
		vizServerLog() << tr("Servidor sem criptografia. O seguinte erro aconteceu: %1.").arg(errorString());
		close();
		return;
	}		
}
//-----------------------------------------------------------------------------
VizCUnencryptedTcpServer::~VizCUnencryptedTcpServer() 
{

}
//-----------------------------------------------------------------------------
void VizCUnencryptedTcpServer::removeSessionId(QString sessionId)
{
	if (SessionMapIp.contains(sessionId))
		SessionMapIp.remove(sessionId);
}
//-----------------------------------------------------------------------------
void VizCUnencryptedTcpServer::addSessionId(QString sessionId, QString ip)
{
	SessionMapIp.insert(sessionId, ip);
}
//-----------------------------------------------------------------------------
void VizCUnencryptedTcpServer::incomingConnection(int socketDescriptor)
{	
	QTcpServer::incomingConnection(socketDescriptor);

	QString ip;
	QString uniqueSessionId;

	if (hasPendingConnections())
		ip = nextPendingConnection()->peerAddress().toString();

	QList<QString> sessions = SessionMapIp.keys(ip);

	if (sessions.size() > 0)
	{
		uniqueSessionId = sessions.at(0);
		SessionMapIp.remove(uniqueSessionId);
	}

	emit startUnencryptedConnection(socketDescriptor, ip, uniqueSessionId);		
}
//-----------------------------------------------------------------------------
QString VizCTPServer::generateSessionId()
{
	QString sessionId('0', 32);

	for (int i = 0; i < sessionId.size(); i++)
	{
		s32 randomNumber = (rand() % (10 + 26 + 26));

		sessionId[i] = (randomNumber < 10) ? (randomNumber + 48) : 
			((randomNumber < 36) ? (randomNumber + 65 - 10) : (randomNumber + 97 - 36));
	}

	return sessionId;
}
//-----------------------------------------------------------------------------
void VizCTPServer::addClient(const QString& user, const QString& sessionId, const QString& ip)
{
	vizServerLog() << QString("Usuário %1 com sessão %2 e ip %3 conectou.").arg(user).arg(sessionId).arg(ip);
}
//-----------------------------------------------------------------------------
void VizCTPServer::removeClient(const QString& sessionId)
{
	vizServerLog() << QString("Cliente com sessão %1 desconectou.").arg(sessionId);	
}
//-----------------------------------------------------------------------------
void VizCTPServer::threadStopped()
{
	vizServerLog() << "Thread Finalizada.";
}
//-----------------------------------------------------------------------------
void VizCTPServer::startThread(int socketDescriptor)
{
	vizServerLog() << "Thread disparada...";
	VizCServerThread* thread = new VizCServerThread(socketDescriptor, generateSessionId());	
	// Para a thread ficar como seu proprio parent
	thread->moveToThread(thread);

 	connect(thread, SIGNAL(onlineUser(QString, QString, QString)), this, SLOT(addClient(QString, QString, QString)));
	connect(thread, SIGNAL(startSession(QString, QString)), UnencryptedTcpServer, SLOT(addSessionId(QString, QString)));
 	connect(thread, SIGNAL(closeSession(QString)), this, SLOT(removeClient(QString)));
	connect(thread, SIGNAL(closeSession(QString)), UnencryptedTcpServer, SLOT(removeSessionId(QString)));
	connect(thread, SIGNAL(finished()), SLOT(deleteLater()));		
	connect(thread, SIGNAL(finished()), this, SLOT(threadStopped()));
	connect(UnencryptedTcpServer, SIGNAL(startUnencryptedConnection(int, QString, QString)),
		thread, SLOT(tcpStartSocket(int, QString, QString)), Qt::QueuedConnection);	
	connect(thread, SIGNAL(realTimeRequest(QString, bool)), MegabolsaServer, SLOT(receiveThreadRealTimeRequest(QString, bool)));
	connect(MegabolsaServer, SIGNAL(sendTickToThread(QString, QString)), thread, SLOT(receiveTick(QString, QString)));

	
	thread->start();
}
//-----------------------------------------------------------------------------
VizCTPServer::VizCTPServer(QObject* parent) : QObject(parent)
{
	MegabolsaServer = new VizCMegabolsaServer();
	EncryptedTcpServer   = new VizCEncryptedTcpServer();
	UnencryptedTcpServer = new VizCUnencryptedTcpServer();	

	connect(EncryptedTcpServer, SIGNAL(startThread(int)), this, SLOT(startThread(int)));
}
//-----------------------------------------------------------------------------
VizCTPServer::~VizCTPServer()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
