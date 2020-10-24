//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCLIENTINFOITEMH__
#define __VIZCCLIENTINFOITEMH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"

#include <QDate>
#include <QString>
#include <QObject>
#include <QStringList>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCClientInfoItem 
{	

public:
	QDateTime TimeStart;
	const char * Direction;
	int  PacketType;
	QString Record;
	
	VizCClientInfoItem(const VizETPClientPacket& clientPacket, const QString& record);
	VizCClientInfoItem(const VizETPServerPacket& serverPacket, const QString& record);	
	
	~VizCClientInfoItem();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif