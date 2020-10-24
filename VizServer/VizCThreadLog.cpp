//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCThreadLog.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCThreadLog::VizCThreadLog(QObject* parent) : QObject(parent), Buffer(&LoggingBuffer)
{	
}
//-----------------------------------------------------------------------------
VizCThreadLog::~VizCThreadLog()
{

}
//-----------------------------------------------------------------------------
void VizCThreadLog::dumpBufferToFile()
{
	if (!ClientTraces.exists())
		return;

	if (!ClientTraces.open(QIODevice::Append))
		qDebug() << "Problema na abertura do arquivo.";

	ClientTraces.close();
}
//-----------------------------------------------------------------------------
void VizCThreadLog::createFile(const QString& clientName)
{
	const char* logDirectory = "Log\\ClientLog\\";

	QDir dir(logDirectory);

	if (!dir.exists())
	{
		QDir createDirectory;
		createDirectory.mkdir(logDirectory);
	}

	QString fileName = QString("%1%2.csv").arg(logDirectory).arg(clientName);

	ClientTraces.setFileName(fileName);

	QString* stringbuffer = Buffer.string();

	if (ClientTraces.exists())
	{
		if (!ClientTraces.open(QIODevice::Append))
 			qDebug() << "Problema na abertura do arquivo.";		

		Buffer.setDevice(&ClientTraces);

		ClientTraces.close();		
	}
	else
	{
		if (!ClientTraces.open(QIODevice::WriteOnly))
			qDebug() << "Problema na cria��o do arquivo";
		
		Buffer.setDevice(&ClientTraces);

		ClientTraces.close();		
	}

	Buffer << *stringbuffer;

	dumpBufferToFile();
}
//-----------------------------------------------------------------------------
QString VizCThreadLog::translateDirection(const VizEClientLogDirection& direction) const
{
	switch (direction) 
	{
	case VECLD_OUT:
		return tr("Saiu");
		break;
	case VECLD_IN:
		return tr("Entrou");
		break;
	case VECLD_LOCAL:
		return tr("Local");
		break;
	case VECLD_UNKNOWN:
		return tr("Desconhecido");
		break;

	}
	return QString();
}
//-----------------------------------------------------------------------------
QString VizCThreadLog::translatePacketOut(const s32& packetType) const
{
	VizETPClientPacket packet = static_cast<VizETPClientPacket>(packetType);

	switch (packet)
	{
	case VETPCP_INVALID:
		return tr("Inv�lido");
		break;
	// autentica��o
	case VETPCP_SSL_AUTHENTICATION_OK:
		return tr("Autentica��o ok");
		break;
	case VETPCP_SSL_ERROR_AUTHENTICATION:
		return tr("Erro na autentica��o");
		break;
	case VETPCP_SSL_SESSIONID:
		return tr("Sess�o");
		break;
	case VETPCP_SSL_UNENCRYPTED_SESSIONID_OK:
		return tr("Sess�o autenticada");
		break;
	case VETPCP_SSL_ERROR_UNENCRYPTED_SESSIONID:
		return tr("Erro na autentica��o da sess�o");
		break;
	case VETPCP_SSL_READY_FOR_SESSIONID:
		return tr("Aguardando sess�o");
		break;
	case VETPCP_SSL_PROTOCOL_VERSION:
		return tr("Vers�o do protocolo");
	// dados histricos
	case VETPCP_TCP_FULL_HISTO:
		return tr("Ativo completo");
		break;
	case VETPCP_TCP_CURRENTLY_UNAVAILABLE_HISTO:
		return tr("Ativo n�o dispon�vel");
		break;
	case VETPCP_TCP_PARTIAL_HISTO:
		return tr("Atualiza��o de ativo");
		break;
	case VETPCP_TCP_HISTO_UP_TO_DATE:
		return tr("Ativo do cliente em dia");
		break;
	case VETPCP_TCP_UPDATE_COMPANY_INFO:
		return tr("Atualiza��o da informa��o da companhia");
		break;
	case VETPCP_TCP_UPDATE_SECURITY_INFO:
		return tr("Atualiza��o da informa��o do ativo");
		break;
	case VETPCP_TCP_REMOVE_COMPANY_HISTO:
		return tr("Remova companhia");
		break;
	case VETPCP_TCP_REMOVE_SECURITY_HISTO:
		return tr("Remova ativo");
		break;
			// tempo-real
	case VETPCP_TCP_FULL_REALTIME:
		return tr("Ativo tempo-real");
		break;
	case VETPCP_TCP_CURRENTLY_UNAVAILABLE_REALTIME:
		return tr("Ativo n�o dispon�vel");
		break;
	case VETPCP_TCP_PARTIAL_REALTIME:
		return tr("Atualiza��o de ativo");
		break;
			// lista de ativos
	case VETPCP_TCP_FULL_NEGOTIABLE_LIST:
		return tr("Lista de ativos");
		break;
	case VETPCP_TCP_PARTIAL_NEGOTIABLE_LIST:
		return tr("Atualiza��o da lista de ativos");
		break;
	case VETPCP_TCP_NEGOTIABLE_UP_TO_DATE:
		return tr("Lista de ativos em dia");
		break;
	}
	return QString();
}
//-----------------------------------------------------------------------------
QString VizCThreadLog::translatePacketIn(const s32& packetType) const
{
	VizETPServerPacket packet = static_cast<VizETPServerPacket>(packetType);

	switch (packet)
	{
	case VETPSP_INVALID:
		return tr("Invalido");
		break;
		// SSL
	case VETPSP_AUTHENTICATION:
		return tr("Autenticando");
		break;
	case VETPSP_REQUEST_SESSIONID:
		return tr("Requisi��o de sess�o");
		break;
	case VETPSP_REQUEST_FULL_HISTO:
		return tr("Requisi��o de ativo");
		break;
	case VETPSP_REQUEST_PARTIAL_HISTO:
		return tr("Atualiza��o de ativo");
		break;
	case VETPSP_REQUEST_FULL_REALTIME:
		return tr("Requisi��o de ativo tempo-real");
		break;
	case VETPSP_REQUEST_PARTIAL_REALTIME:
		return tr("Atualiza��o de ativo tempo-real");
		break;
	case VETPSP_REQUEST_FULL_NEGOTIABLE_LIST:
		return tr("Requisi��o de lista de ativos");
		break;
	case VETPSP_REQUEST_PARTIAL_NEGOTIABLE_LIST:
		return tr("Atualiza��o de lista de ativos");
		break;
		// TCP
	case VETPSP_UNENCRYPTED_SESSIONID:
		return tr("Id da Sess�o");
		break;		
	}

	return QString();	
}
//-----------------------------------------------------------------------------
void VizCThreadLog::addRecord(const VizSClientRecord& record)
{
	QString preDefinedPacketType = (record.Direction == VECLD_OUT) ? translatePacketOut(record.PacketType) 
		: ((record.Direction == VECLD_IN) ? translatePacketIn(record.PacketType) : QString());

	QString in = QString("%1; %2; %3; %4; %5\n"
		).arg(QDateTime::currentDateTime().toString(Qt::ISODate)
		).arg(translateDirection(record.Direction)
		).arg(QString::number(record.PacketType)
		).arg((record.Direction == VECLD_LOCAL) ? (record.Register) : preDefinedPacketType
		).arg(QString::number(record.PacketSize));

	Buffer << in;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------