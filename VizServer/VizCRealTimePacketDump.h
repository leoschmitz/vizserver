//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCREALTIMEPACKETDUMPH__
#define __VIZCREALTIMEPACKETDUMPH__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QMutex>
#include <QPair>
#include <QString>

#include "VizCRealTimePeriod.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCRealTimePacketDump : public QObject
{	
private:
	QMutex IdToRealTimePeriodMutex;
	QMap<QString, QPair<int, QSharedPointer<const core::VizCRealTimePeriod> > > IdToRealTimePeriod;

	VizCRealTimePacketDump();
	~VizCRealTimePacketDump();

	static VizCRealTimePacketDump* Instance;

public:
	static VizCRealTimePacketDump* instance();

	void addPacket(const QString& msgId, const core::VizCRealTimePeriod* realTimePeriod, int timesRequired = 1);
	const core::VizCRealTimePeriod* packet(const QString& msgId);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif