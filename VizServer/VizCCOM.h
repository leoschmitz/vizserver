//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCOMH__
#define __VIZCCOMH__
//-----------------------------------------------------------------------------
#include <ActiveQt/QAxObject.h>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCCOM : public QAxObject
{	
public:
	bool DllLoaded;

	VizCCOM();
	~VizCCOM();

private:	
	bool LoadDll();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif