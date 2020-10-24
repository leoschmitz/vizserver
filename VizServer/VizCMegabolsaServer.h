//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMEGABOLSASERVERH__
#define __VIZCMEGABOLSASERVERH__
//-----------------------------------------------------------------------------
#include <QObject>

#include "VizCProxyDiff.h"
#include "VizCRealTimePacketDump.h"
#include "VizCMegabolsaMsgParser.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCMegabolsaServer : public QObject 
{	
	Q_OBJECT
public:
	VizCMegabolsaServer(QObject* parent = 0);
	~VizCMegabolsaServer();

public slots:
	void receiveThreadRealTimeRequest(QString symbol, bool input);

signals:
	void sendTickToThread(QString symbol, QString msgId);

private:
	VizCProxyDiff* ProxyDiff;
	VizCMegabolsaMsgParser Parser;

	QMap<QString, int> TradeRequests;
	VizCRealTimePacketDump* PacketDump;

	void addTradeRequest(const QString& symbol);
	void removeTradeRequest(const QString& symbol);
	void parseMessage(const QString& id, const QString& msg);

private slots:
	void receiveProxyDiffMessage(QString id, QString msg);


};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif