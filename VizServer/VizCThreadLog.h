//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTHREADLOGH__
#define __VIZCTHREADLOGH__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QTextStream>
#include <QByteArray>
#include <QDir>

#include "VizCLog.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
enum VizEClientLogDirection
{
	VECLD_UNKNOWN = -1,
	VECLD_OUT	  = 0,
	VECLD_IN	  = 1,
	VECLD_LOCAL	  = 2,
	VECLD_DEFAULT = VECLD_UNKNOWN
};
//-----------------------------------------------------------------------------
typedef struct vizSClientRecord
{
	VizEClientLogDirection Direction; // Direção 
	s32 PacketType; // Tipo do pacote
	s32 PacketSize; // tamanho do pacote
	QString Register;	// Registro
	
	vizSClientRecord(const VizEClientLogDirection& direction, const s32& packetType, const s32& packetSize, const QString& reg = "")
		: Direction(direction), PacketType(packetType), PacketSize(packetSize), Register(reg)
	{
	}
} VizSClientRecord;
//-----------------------------------------------------------------------------
class VizCThreadLog : public QObject
{	
	Q_OBJECT
public:
	VizCThreadLog(QObject* parent = 0);    	
	~VizCThreadLog();
	
	void addRecord(const VizSClientRecord& record);
	void createFile(const QString& clientName);
	void dumpBufferToFile();

private:
	QFile ClientTraces;	
	QTextStream Buffer;
	QString LoggingBuffer;

	// O Translation vai pro view depois
	QString translatePacketIn(const s32& packetType) const;
	QString translatePacketOut(const s32& packetType) const;
	QString translateDirection(const VizEClientLogDirection& direction) const;	
};	
//-----------------------------------------------------------------------------
static inline VizCThreadLog* operator<<(VizCThreadLog* in, const VizSClientRecord& record)
{
	in->addRecord(record);
	return in;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
