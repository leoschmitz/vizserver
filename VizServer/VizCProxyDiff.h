//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCPROXYDIFFH__
#define __VIZCPROXYDIFFH__
//-----------------------------------------------------------------------------
#include <QThread>

#include "VizCCOM.h"
#include "VizCMegabolsaMsgParser.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCProxyDiff : public QObject 
{	
	Q_OBJECT
public:
	VizCProxyDiff(QObject* parent = 0);
	~VizCProxyDiff();
	
	void setIpAddr(QString ip);
	void setPort(QString port);

	QString port();
	QString ipAddr();
	
	void start();
	void stop();
private slots:
	void OnAcceptStart();
	void OnConnect();
	void OnDisconnect(QString Desc);
	void OnError(int Error, QString Msg, QString Desc);
	void OnReceiveData(QString LastMsgId, QString SPF_Header, QString DataPtr, int DataSize);
	void updateReceiveMsg();
signals:
	void Connect();
	void StartRequest(QString MsgId);
	void Disconnect();

	void message(QString id, QString msg);
private:
	QString IpAddr;
	QString Port;

	VizCCOM TSINALCom;	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif