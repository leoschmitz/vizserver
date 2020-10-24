//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "VizCClientInfoItem.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCClientInfoItem::VizCClientInfoItem(const VizETPClientPacket& clientPacket, const QString& record) 
	: TimeStart(QDateTime::currentDateTime()), Direction("IN"), PacketType(static_cast<int>(clientPacket)), Record(record)
{
	//switch (PacketType)

}
//-----------------------------------------------------------------------------
VizCClientInfoItem::VizCClientInfoItem(const VizETPServerPacket& serverPacket, const QString& record) : Direction("OUT"), 
	PacketType(static_cast<int>(serverPacket)), Record(record)
{

}
//-----------------------------------------------------------------------------
VizCClientInfoItem::~VizCClientInfoItem()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
