//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCServerThread.h"

#include <QMessageBox>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
const QString VizTPProtocolVersion = "0.0.3";
//-----------------------------------------------------------------------------
VizCServerThread::VizCServerThread(int socketDescriptor, QString sessionId, QObject* parent)
	: QThread(parent), SslSocketDescriptor(socketDescriptor), TcpBlockSize16(0), 
	SslBlockSize16(0), SessionId(sessionId), TcpAuthenticationPassed(false),
	UserLogin(""), ServerState(DEFAULT_STATE)
{

}
//-----------------------------------------------------------------------------
bool VizCServerThread::initializeThread()
{
	SslSocketBuffer.setDevice(SslSocket);
	SslSocketBuffer.setVersion(QDataStream::Qt_4_0);

	if (!SslSocket->setSocketDescriptor(SslSocketDescriptor)) 
	{
		emit error(SslSocket->error());
		return false;
	}

	setState(CERTIFIED_STATE);

	SslSocket->setLocalCertificate("viztrader_v0.2.pem");
	SslSocket->setPrivateKey("nullkey.pem");

	connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReceiveData()));
	connect(TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), 
		SLOT(socketError(QAbstractSocket::SocketError)), Qt::DirectConnection);

	connect(SslSocket, SIGNAL(disconnected()), this, SLOT(sslConnectionClosed()));
	connect(SslSocket, SIGNAL(readyRead()), this, SLOT(sslReceiveData()));	
	connect(SslSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)), Qt::DirectConnection);

	SslSocket->startServerEncryption();

	return true;
}
//-----------------------------------------------------------------------------
void VizCServerThread::endThread(int condition)
{
	if (state() == CLOSING_STATE)
	{
		return;
	}
	else
	{
		setState(CLOSING_STATE);

		foreach (const QString& symbol, TradeRequests)
		{
			emit realTimeRequest(symbol, false);
		}	

		TradeRequests.clear();
	}

	if (TcpSocket)
		TcpSocket->disconnectFromHost();	

	if (SslSocket)
		SslSocket->disconnectFromHost();

	emit closeSession(SessionId);	

	Log->dumpBufferToFile();

	setState(UNKNOWN_STATE);
	
	exit(condition);	
}
//-----------------------------------------------------------------------------
void VizCServerThread::sendProtocolVersion()
{
	VizCTPLargePacket packet(VETPCP_SSL_PROTOCOL_VERSION);
	
	packet << VizTPProtocolVersion;

	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
}
//-----------------------------------------------------------------------------
void VizCServerThread::authenticate(QDataStream& in)
{
	setState(AUTHENTICATING_USER_STATE);

	QString password;

	in >> UserLogin;
	in >> password;	

	VizCTPLargePacket packet;

	if (DbUtils->isAuthenticationPassed(UserLogin, password))
	{
		packet.setPacketType(VETPCP_SSL_AUTHENTICATION_OK);	
	}
	else 
	{
		packet.setPacketType(VETPCP_SSL_ERROR_AUTHENTICATION);
	}
		
	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
}//-----------------------------------------------------------------------------
void VizCServerThread::authenticateSessionId(QDataStream& in)
{
	QString receivedSessionId;
	in >> receivedSessionId;

	VizCTPLargePacket packet;

	if (receivedSessionId != SessionId)
	{
		packet.setPacketType(VETPCP_SSL_ERROR_UNENCRYPTED_SESSIONID);
	}
	else
	{
		packet.setPacketType(VETPCP_SSL_UNENCRYPTED_SESSIONID_OK);		

		TcpAuthenticationPassed = true;

		Log->createFile(UserLogin);

		emit onlineUser(UserLogin, SessionId, SslSocket->peerAddress().toString());

	}	

	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());	

	if (TcpAuthenticationPassed)
	{
		sendProtocolVersion();

		setState(CONNECTED_STATE);
	}
}
//-----------------------------------------------------------------------------
void VizCServerThread::sendSessionId()
{
	setState(STARTING_SESSION_STATE);

	emit startSession(SessionId, SslSocket->peerAddress().toString());

	VizCTPLargePacket packet(VETPCP_SSL_SESSIONID);

	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

	packet << SessionId;
	
	if (SslSocket->isEncrypted())
		SslSocket->write(packet.getPacketReady());
}
//-----------------------------------------------------------------------------
bool VizCServerThread::sendNegotiableList(QDataStream& in, bool partial)
{

	VizCTPLargePacket packet;
	QVector<QStringList> negotiableList;
	
	if (partial)
	{
		QVector<QString> removableList;
	
		QDate date;
		in >> date;

		if (!DbUtils->getNegotiableList(negotiableList, date))
			return false;

		if (!DbUtils->getRemovedNegotiableElements(removableList, date))
			return false;	


		if ((negotiableList.size() == 0) && (removableList.size() == 0))
		{
			packet.setPacketType(VETPCP_TCP_NEGOTIABLE_UP_TO_DATE);						
		}
		else
		{
			packet.setPacketType(VETPCP_TCP_PARTIAL_NEGOTIABLE_LIST);
			packet << negotiableList;
			packet << removableList;
		}		
	}
	else
	{
		packet.setPacketType(VETPCP_TCP_FULL_NEGOTIABLE_LIST);
		
		if (!DbUtils->getNegotiableList(negotiableList))
			return false;		
		
		packet << negotiableList;
	}

	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

	
	TcpSocket->write(packet.getPacketReady());

	return true;
}
//-----------------------------------------------------------------------------
void VizCServerThread::tcpReceiveData()
{	
	QDataStream in(TcpSocket);
	in.setVersion(QDataStream::Qt_4_0);

	if (TcpBlockSize16 == 0) 
	{
		if (TcpSocket->bytesAvailable() < (int)sizeof(quint16))
			return;

		in >> TcpBlockSize16;
	}

	if (TcpSocket->bytesAvailable() < TcpBlockSize16)
		return;

	VizETPServerPacket packetType;
	in >> packetType;	

	Log << VizSClientRecord(VECLD_IN, packetType, TcpBlockSize16);

	if (packetType == VETPSP_UNENCRYPTED_SESSIONID)
		authenticateSessionId(in);		

	TcpBlockSize16 = 0;	
}
//-----------------------------------------------------------------------------
void VizCServerThread::sslManagePacket(QDataStream& in, bool& receivingNextPacket)
{
	receivingNextPacket = false;

	if (SslSocket->isEncrypted())
	{
		if (SslBlockSize16 == 0) 
		{
			if (SslSocket->bytesAvailable() < (int)sizeof(quint16))
				return;

			in >> SslBlockSize16;
		}

		if (SslSocket->bytesAvailable() < SslBlockSize16)
			return;


		VizETPServerPacket packetType;
		in >> packetType;

		switch (packetType)
		{
		case VETPSP_INVALID:
			qDebug() << "VETPSP_INVALID";			
			break;
		case VETPSP_AUTHENTICATION:
			authenticate(in);
			break;		
		case VETPSP_REQUEST_SESSIONID:
			sendSessionId();
			break;
		case VETPSP_REQUEST_FULL_HISTO:
			if (TcpAuthenticationPassed)
			{
				if (!sendSecurity(in))
					endThread(-1);
			}
			break;
		case VETPSP_REQUEST_PARTIAL_HISTO:
			if (TcpAuthenticationPassed)
			{
				if (!sendSecurity(in, true))
					endThread(-1);
			}
			break;
		case VETPSP_REQUEST_FULL_REALTIME:
			if (TcpAuthenticationPassed)
			{
				if (!sendRealTimeSecurity(in, false))
					endThread(-1);
			}
			break;
		case VETPSP_REQUEST_PARTIAL_REALTIME:
			break;
		case VETPSP_REQUEST_FULL_NEGOTIABLE_LIST:
			if (TcpAuthenticationPassed)
			{
				if (!sendNegotiableList(in))
					endThread(-1);
			}
			break;
		case VETPSP_REQUEST_PARTIAL_NEGOTIABLE_LIST:
			if (TcpAuthenticationPassed)
			{
				if (!sendNegotiableList(in, true))
					endThread(-1);
			}
			break;
		}		

		Log << VizSClientRecord(VECLD_IN, packetType, SslBlockSize16);

		if (SslSocket->bytesAvailable() > 0)
		{	
			receivingNextPacket = true;										
		}

		SslBlockSize16 = 0;
	}

	return;
}
//-----------------------------------------------------------------------------
void VizCServerThread::sslReceiveData()
{	
	s32 floodCount(0);
	bool receivingNextPacket = true;
	
	QDataStream& in = SslSocketBuffer;	

	while (receivingNextPacket)
	{
		sslManagePacket(in, receivingNextPacket);
		if ((floodCount++) > 10)
		{
			floodCount = 0;
			sleep(20);		
		}
	}	
}
//-----------------------------------------------------------------------------
void VizCServerThread::sslConnectionClosed()
{
	endThread(0);	
}
//-----------------------------------------------------------------------------
void VizCServerThread::tcpStartSocket(int socketDescriptor, QString ip, QString sessionId)
{
	if (QThread::currentThread() == TcpSocket->thread())
	{	
		if (SslSocket->isOpen() && !TcpSocket->isOpen())
		{
			if ((SslSocket->peerAddress().toString() == ip) && (sessionId == SessionId))
			{
				if (!TcpSocket->setSocketDescriptor(socketDescriptor))
				{
					emit error(TcpSocket->error());

					qDebug() << TcpSocket->error();
				}
				else
				{
					VizCTPLargePacket packet(VETPCP_SSL_READY_FOR_SESSIONID);
					
					Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());

					if (SslSocket->isEncrypted())
						SslSocket->write(packet.getPacketReady());					
				}
			}
		}
	}	
}
//-----------------------------------------------------------------------------
void VizCServerThread::tcpConnectionClosed()
{
	endThread(0);
}
//-----------------------------------------------------------------------------
void VizCServerThread::receiveTick(QString symbol, QString msgId)
{
	const core::VizCRealTimePeriod* rt = VizCRealTimePacketDump::instance()->packet(msgId);

	sendRealTimeTick(symbol, *rt);
}
//-----------------------------------------------------------------------------
void VizCServerThread::sendRealTimeTick(const QString& symbol, const core::VizCRealTimePeriod& rt)
{
	VizCTPLargePacket packet(VETPCP_TCP_REAL_TIME_TICK);

	packet << symbol;
	packet << rt;
	
	TcpSocket->write(packet.getPacketReady()); 	 
}
//-----------------------------------------------------------------------------
bool VizCServerThread::sendRealTimeSecurity(QDataStream& in, bool partial)
{
 	QString symbol;
 	core::VizCCompany company;
 
 	in >> symbol;

 	VizCTPLargePacket packet;

	//////////////////////////////////////////////////////////////////////////
	// Temporary
	//////////////////////////////////////////////////////////////////////////
	
	packet.setPacketType(VETPCP_TCP_FULL_REALTIME);
	core::VizCSecurity sec;
	sec.Symbol = QString("PETR3");

	core::VizCCompany Petrobras3;
	Petrobras3.Securities.append(sec);
	
	packet << Petrobras3;
	packet << Petrobras3.Securities.at(0);
	packet << Petrobras3.Securities.at(0).RealTimePeriods;

// 
// 	if (partial)
// 	{
// 		QDate lastDate(1900, 01, 01);	
// 
// 		in >> lastDate;
// 
// 		if (!DbUtils->getSecurity(symbol, company, lastDate))
// 			return false;
// 	}
// 	else
// 	{
// 		if (!DbUtils->getSecurity(symbol, company))
// 			return false;
// 	}	
// 

// 
// 	if (company.Securities.size() < 1)
// 	{
// 		packet.setPacketType(VETPCP_TCP_CURRENTLY_UNAVAILABLE_HISTO);
// 		packet << symbol;
// 	}
// 	else
// 	{
// 		if (company.Securities.at(0).SecurityPeriods.size() == 0)
// 		{
// 			if (partial)
// 			{
// 				packet.setPacketType(VETPCP_TCP_HISTO_UP_TO_DATE);				
// 			}
// 			else 
// 			{
// 				packet.setPacketType(VETPCP_TCP_HISTO_EMPTY);
// 				packet << company;
// 				packet << company.Securities.at(0);
// 				packet << company.Securities.at(0).SecurityPeriods;
// 			}
// 
// 		}		
// 		else
// 		{
// 			if (partial)
// 			{
// 				packet.setPacketType(VETPCP_TCP_PARTIAL_HISTO);				
// 			}
// 			else
// 			{
// 				packet.setPacketType(VETPCP_TCP_FULL_HISTO);				
// 			}
// 
// 			packet << company;
// 			packet << company.Securities.at(0);
// 			packet << company.Securities.at(0).SecurityPeriods;
// 		}
// 	}
// 
 	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());
 
 	TcpSocket->write(packet.getPacketReady()); 	 

	if (!TradeRequests.contains(symbol))
	{
		TradeRequests.insert(symbol);

		emit realTimeRequest(symbol);
	}	

 	return true;
}
//-----------------------------------------------------------------------------
bool VizCServerThread::sendSecurity(QDataStream& in, bool partial)
{
	QString symbol;
	core::VizCCompany company;

	in >> symbol;

	if (partial)
	{
		QDate lastDate(1900, 01, 01);	

		in >> lastDate;

		if (!DbUtils->getSecurity(symbol, company, lastDate))
			return false;
	}
	else
	{
		if (!DbUtils->getSecurity(symbol, company))
			return false;
	}	

	VizCTPLargePacket packet;

	if (company.Securities.size() < 1)
	{
		packet.setPacketType(VETPCP_TCP_CURRENTLY_UNAVAILABLE_HISTO);
		packet << symbol;
	}
	else
	{
		if (company.Securities.at(0).SecurityPeriods.size() == 0)
		{
			if (partial)
			{
				packet.setPacketType(VETPCP_TCP_HISTO_UP_TO_DATE);				
			}
			else 
			{
				packet.setPacketType(VETPCP_TCP_HISTO_EMPTY);
				packet << company;
				packet << company.Securities.at(0);
				packet << company.Securities.at(0).SecurityPeriods;
			}
			
		}		
		else
		{
			if (partial)
			{
				packet.setPacketType(VETPCP_TCP_PARTIAL_HISTO);				
			}
			else
			{
				packet.setPacketType(VETPCP_TCP_FULL_HISTO);				
			}

			packet << company;
			packet << company.Securities.at(0);
			packet << company.Securities.at(0).SecurityPeriods;
		}
	}
	
	Log << VizSClientRecord(VECLD_OUT, packet.packetType(), packet.packetSize());
	
	TcpSocket->write(packet.getPacketReady()); 	

	return true;
}
//-----------------------------------------------------------------------------
void VizCServerThread::setState(VizTPServerState state)
{
	ServerState = state; 	
}
//-----------------------------------------------------------------------------
VizCServerThread::VizTPServerState VizCServerThread::state()
{
	return ServerState;
}
//-----------------------------------------------------------------------------
void VizCServerThread::run()
{
	DbUtils   = VizCDatabaseUtils::instance();
	TcpSocket = new QTcpSocket;	 
	SslSocket = new QSslSocket;
	Log		  = new VizCThreadLog(this);

	if (!initializeThread())
		endThread(-1);	

	exec();	
}
//-----------------------------------------------------------------------------
void VizCServerThread::socketError(QAbstractSocket::SocketError error)
{
	int socketerror(-1);

// 	switch (error)
// 	{
// 	case QAbstractSocket::ConnectionRefusedError:
// 		vizServerLog() << "Time-out.";
// 		break;
// 	case QAbstractSocket::RemoteHostClosedError:
// 		//vizServerLog() << "Cliente desconectou.";
// 		socketerror = 0;
// 		break;
// 	default:
// 		//vizServerLog() << QString("Erro: %1!").arg(SslSocket->errorString());
// 		break;
// 	}	

	endThread(socketerror);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
