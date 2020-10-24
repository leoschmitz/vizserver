//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSPLITTODATABASE_H__
#define __VIZCSPLITTODATABASE_H__
//-----------------------------------------------------------------------------
#include <VizTypes.h>
#include <VizCCompany.h>
#include <VizCDatabaseUtils.h>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCSplitToDatabase : public QObject
{	
	Q_OBJECT

public:
	bool openMegaBolsaFile(QString fileName);

	VizCSplitToDatabase(QObject* parent = 0);
	~VizCSplitToDatabase();
private:	
	VizCDatabaseUtils* DbUtils;

	bool readRegister(QString reg);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif