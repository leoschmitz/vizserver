//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QtGui>

#include "VizServer.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
void VizServer::eagerInitializer()
{
	qsrand(QDateTime::currentDateTime().toTime_t());	
	
	VizCServerLog* dummyLogInitializer = VizCServerLog::instance();
	
	VizCDatabaseUtils* dummyDbInitializer  = VizCDatabaseUtils::instance();	
}
//-----------------------------------------------------------------------------
VizServer::VizServer(QObject* parent) 
	: QObject(parent)
{
	eagerInitializer();

	// Começa o servidor
	SecurityServer = new VizCTPServer;		
}
//-----------------------------------------------------------------------------
VizServer::~VizServer()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------