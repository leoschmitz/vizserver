//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QDebug>
#include <QTimer>

#include "VizCProxyDiff.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCProxyDiff::VizCProxyDiff(QObject* parent) : IpAddr("127.0.0.1"), Port("0"), QObject(parent) 
{	 
	if (TSINALCom.DllLoaded)
	{
		connect(this, SIGNAL(Connect()), TSINALCom.qObject(), SLOT(Connect()));
		connect(this, SIGNAL(Disconnect()), TSINALCom.qObject(), SLOT(Disconnect()));
		connect(this, SIGNAL(StartRequest(QString)), TSINALCom.qObject(), SLOT(StartRequest(QString)));
		
		connect(TSINALCom.qObject(), SIGNAL(OnAcceptStart()), this, SLOT(OnAcceptStart()));
		connect(TSINALCom.qObject(), SIGNAL(OnConnect()), this, SLOT(OnConnect()));
		connect(TSINALCom.qObject(), SIGNAL(OnDisconnect(QString)), this, SLOT(OnDisconnect(QString)));
		connect(TSINALCom.qObject(), SIGNAL(OnReceiveData(QString, QString, QString, int)), this, SLOT(OnReceiveData(QString, QString, QString, int)));
		connect(TSINALCom.qObject(), SIGNAL(OnError(int, QString, QString)), this, SLOT(OnError(int, QString, QString)));

		setIpAddr("localhost");
		setPort("15000");
	}	
}
//-----------------------------------------------------------------------------
VizCProxyDiff::~VizCProxyDiff()
{
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::OnAcceptStart()
{	
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::start()
{
	emit Connect();
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::stop()
{
	emit Disconnect();
}
//-----------------------------------------------------------------------------
// Ocorre quando o componente se conecta no Receptor ProxyDiff
void VizCProxyDiff::OnConnect()
{	
	emit StartRequest("");
}
//-----------------------------------------------------------------------------
// O processo Receptor ProxyDiff desconectou o cliente.
void VizCProxyDiff::OnDisconnect(QString Desc)
{
}
//-----------------------------------------------------------------------------
// É gerado por qualquer anomalia que possa ocorrer durante todo o processo de 
// conexão ou durante a troca de mensagens.
void VizCProxyDiff::OnError(int Error, QString Msg, QString Desc)
{
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::updateReceiveMsg()
{
	TSINALCom.setProperty("Ack", true);
}
//-----------------------------------------------------------------------------
// Ocorre quando o MEGA BOLSA (Sistema Eletrônico de Negociação da  BOVESPA) envia 
// uma mensagem de difusão. Obs: SPF_Header para uso futuro
void VizCProxyDiff::OnReceiveData(QString LastMsgId, QString SPF_Header, QString DataPtr, int DataSize)
{
	static int temporaryFloodControl(0);

 	if (!(temporaryFloodControl % 3000))
	{
		//qDebug() << temporaryFloodControl;

		QTimer *timer = new QTimer(this);
		
		connect(timer, SIGNAL(timeout()), this, SLOT(updateReceiveMsg()));
		
		timer->start(1000);	
 		
	}
	else
	{
		TSINALCom.setProperty("Ack", true);
	}	

	// Parser.parseMessage(LastMsgId, DataPtr);

	emit message(LastMsgId, DataPtr);

	temporaryFloodControl++;
}
//-----------------------------------------------------------------------------
QString VizCProxyDiff::port()
{
	return Port;
}
//-----------------------------------------------------------------------------
QString VizCProxyDiff::ipAddr()
{
	return IpAddr;
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::setIpAddr(QString ip)
{
	IpAddr = ip;
	TSINALCom.setProperty("IpAddr", IpAddr);
}
//-----------------------------------------------------------------------------
void VizCProxyDiff::setPort(QString port)
{
	Port = port;	
	TSINALCom.setProperty("Port", Port);
}
//-----------------------------------------------------------------------------
// void VizCProxyDiff::receiveThreadRealTimeRequest(const QString& symbol)
// {
// 	qDebug() << symbol;
// 	Parser.addTradeRequest(symbol);
// }
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------