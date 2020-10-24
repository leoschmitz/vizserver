//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZSERVICE_H__
#define __VIZSERVICE_H__
//-----------------------------------------------------------------------------
#include <QtService/QtServiceBase>

#include "VizServer.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizService : public QtService<QCoreApplication>
{	
public:
	
	VizService(int argc, char** argv);
	~VizService();

private:
	VizServer* Server;
	
protected:
	void start();
	void pause();
	void resume();
	void stop();

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif // __VIZSERVICE_H__
//-----------------------------------------------------------------------------