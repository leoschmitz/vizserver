//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCCVMTODATABASE__
#define __VIZCCCVMTODATABASE__
//-----------------------------------------------------------------------------
#include <VizTypes.h>
#include <VizCDatabaseUtils.h>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCCCVMToDatabase: public QObject
{	
	Q_OBJECT

public:
	bool openFile(QString fileName = QString("BovespaData\\CnpjCCVM.txt"));

	VizCCCVMToDatabase(QObject * parent = 0);
	~VizCCCVMToDatabase();

private:
	VizCDatabaseUtils* DbUtils;
	bool insertCCVM(const QString& cnpj, const u32& ccvm);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif