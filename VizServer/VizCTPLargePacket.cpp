//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTPLargePacket.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCTPLargePacket::VizCTPLargePacket() : QByteArray(), QDataStream(&(*this), QIODevice::WriteOnly), PacketType(VETPCP_INVALID)
{
	initialize();
}
//-----------------------------------------------------------------------------
VizCTPLargePacket::VizCTPLargePacket(const VizETPClientPacket& packetType) : QByteArray(), PacketType(packetType),
	QDataStream(&(*this), QIODevice::WriteOnly)
{
	initialize();

	data() << PacketType;
}
//-----------------------------------------------------------------------------
VizCTPLargePacket::~VizCTPLargePacket()
{

}
//-----------------------------------------------------------------------------
void VizCTPLargePacket::initialize()
{
	setVersion(QDataStream::Qt_4_0);
	data() << (quint32) 0;	
}
//-----------------------------------------------------------------------------
void VizCTPLargePacket::setPacketType(const VizETPClientPacket& packetType)
{
	PacketType = packetType;

	data() << PacketType;
}
//-----------------------------------------------------------------------------
VizETPClientPacket VizCTPLargePacket::packetType()
{
	return PacketType;
}
//-----------------------------------------------------------------------------
const int VizCTPLargePacket::packetSize()
{
	return (*this).size();
}
//-----------------------------------------------------------------------------
QDataStream& VizCTPLargePacket::data()
{
	return *this;
}
//-----------------------------------------------------------------------------
QByteArray& VizCTPLargePacket::getPacketReady()
{
	device()->seek(0);
	data() << (quint32)(packetSize() - sizeof(quint32));

	return *this;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------