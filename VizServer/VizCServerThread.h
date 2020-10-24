//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSERVERTHREADH__
#define __VIZCSERVERTHREADH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include "VizCCompany.h"
#include "VizCDatabaseUtils.h"
#include "VizCThreadLog.h"
#include "VizCTPLargePacket.h"
#include "VizCRealTimePacketDump.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslSocket>

#include <QMutexLocker>
#include <QString>
#include <QThread>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCServerThread : public QThread
{
	Q_OBJECT
public:
	enum VizTPServerState
	{
		UNKNOWN_STATE = -1,
		UNCONNECTED_STATE = 0,
		CLOSING_STATE = 3,
		CERTIFIED_STATE = 4,
		AUTHENTICATING_USER_STATE = 5,
		STARTING_SESSION_STATE = 6,
		AUTHENTICATING_SESSION_STATE = 7,
		CONNECTED_STATE = 8,
		WAIT_TCP_CONNECTION = 9,
		DEFAULT_STATE = UNCONNECTED_STATE	
	};	

	VizCServerThread(int socketDescriptor, QString sessionId, QObject* parent = 0);

	void run();

private slots:
	void sslReceiveData();
	void tcpReceiveData();
	void sslConnectionClosed();	
	void tcpConnectionClosed();		
	void tcpStartSocket(int socketDescriptor, QString ip, QString sessionId);
	void socketError(QAbstractSocket::SocketError);


signals:
	void error(QTcpSocket::SocketError socketError);
	void closeSession(QString sessionId);
	void startSession(QString sessionId, QString ip);
	void onlineUser(QString user, QString sessionId, QString ip);
	void realTimeRequest(QString symbol, bool input = true);

private:
	QSet<QString> TradeRequests;

	VizTPServerState ServerState;
	QDataStream SslSocketBuffer;
	QDataStream TcpSocketBuffer;
	VizCDatabaseUtils* DbUtils;
	QSslSocket* SslSocket;
	QTcpSocket* TcpSocket;
	VizCThreadLog* Log;

	quint16 SslBlockSize16;
	quint16 TcpBlockSize16;
	QString UserLogin;
	QString SessionId;	
	int SslSocketDescriptor;
	bool TcpAuthenticationPassed;	

	bool initializeThread();
	void endThread(int condition);

	void setState(VizTPServerState state);
	VizTPServerState state();
	void sslManagePacket(QDataStream& in, bool& receivingNextPacket);
	void authenticate(QDataStream& in);
	void authenticateSessionId(QDataStream& in);
	bool sendSecurity(QDataStream& in, bool partial = false); 
	
	bool sendNegotiableList(QDataStream& in, bool partial = false);
	bool sendRealTimeSecurity(QDataStream& in, bool partial = false);	
	void sendRealTimeTick(const QString& symbol, const core::VizCRealTimePeriod& rt);
	void sendSessionId();	
	void sendProtocolVersion();	

public slots:
	void receiveTick(QString symbol, QString msgId);
	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif