//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QDebug>

#include "VizCRealTimePacketDump.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCRealTimePacketDump* VizCRealTimePacketDump::Instance = 0;
//-----------------------------------------------------------------------------
VizCRealTimePacketDump* VizCRealTimePacketDump::instance()
{
	if(!Instance)
	{
		Instance = new VizCRealTimePacketDump;
	} 
	return Instance;
}
//-----------------------------------------------------------------------------
VizCRealTimePacketDump::VizCRealTimePacketDump()
{

}
//-----------------------------------------------------------------------------
VizCRealTimePacketDump::~VizCRealTimePacketDump()
{

}
//-----------------------------------------------------------------------------
void VizCRealTimePacketDump::addPacket(const QString& msgId, const core::VizCRealTimePeriod* realTimePeriod, int timesRequired)
{
	QSharedPointer<const core::VizCRealTimePeriod> dummyPointer(realTimePeriod);

	QPair<int, QSharedPointer<const core::VizCRealTimePeriod> > dummyPair(timesRequired, dummyPointer);

	IdToRealTimePeriod.insert(msgId, dummyPair);
}
//-----------------------------------------------------------------------------
const core::VizCRealTimePeriod* VizCRealTimePacketDump::packet(const QString& msgId)
{
	QMutexLocker lock(&IdToRealTimePeriodMutex);

	bool zero(false);
	//IdToRealTimePeriodMutex.lock();
	zero = ((IdToRealTimePeriod[msgId].first--) > 0) ? false : true;
	//IdToRealTimePeriodMutex.unlock();

	if (zero)
	{
		return IdToRealTimePeriod.take(msgId).second.data();
	}

	return IdToRealTimePeriod[msgId].second.data();
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
