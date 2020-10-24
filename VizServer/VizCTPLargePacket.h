//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTPLARGEPACKET_H__
#define __VIZCTPLARGEPACKET_H__
//-----------------------------------------------------------------------------
#include <QDataStream>

#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCTPLargePacket : private QByteArray, public QDataStream
{	
private:	
	VizETPClientPacket PacketType;

	void initialize();
	
public:
	VizCTPLargePacket();
	VizCTPLargePacket(const VizETPClientPacket& packetType);	
	~VizCTPLargePacket();

	void setPacketType(const VizETPClientPacket& packetType);
	VizETPClientPacket packetType();
	
	const int packetSize();
	QDataStream& data();
	QByteArray& getPacketReady();	
};
//-----------------------------------------------------------------------------
template <typename T>
static inline VizCTPLargePacket& operator<<(VizCTPLargePacket& out, const T& t)
{
	out.data() << t;	
	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
