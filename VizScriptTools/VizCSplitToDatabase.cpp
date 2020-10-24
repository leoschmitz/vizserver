//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCSplitToDatabase.h"

#include <VizTypes.h>
#include <VizCCompany.h>
#include <VizCDatabaseUtils.h>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
// As datas tem que estar ordenadas ou não funcionará corretamente
bool VizCSplitToDatabase::openMegaBolsaFile(QString fileName)
{
	QFile finput(fileName);

	if (!finput.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QByteArray inputFromDenison = finput.readAll();

	finput.close();

	QTextStream in(&inputFromDenison, QIODevice::ReadOnly);

	u32 ccvm(0);
	QString cnpj;
	bool ok(false);

	while (!in.atEnd()) 
	{
		QString line = in.readLine();
		
		if (!readRegister(line))
			return false;
		
	}

	return true;
}
//-----------------------------------------------------------------------------
VizCSplitToDatabase::VizCSplitToDatabase(QObject* parent) : QObject(parent)
{
	DbUtils = VizCDatabaseUtils::instance();

	openMegaBolsaFile("BovespaData\\splitsOrdenado.txt");
}
//-----------------------------------------------------------------------------
VizCSplitToDatabase::~VizCSplitToDatabase()
{

}
//-----------------------------------------------------------------------------
bool VizCSplitToDatabase::readRegister(QString reg)
{
	bool ok(false);

	QString cnpj = reg.section(' ', 0, 0);
	s32 ccvm = reg.section(' ', 1, 1).toInt(&ok); 

	if (!ok)
		return false;

	QString stringDate = reg.section(' ', 2, 2);
	s32 day = stringDate.section('/', 0, 0).toInt(&ok);
	if (!ok)
		return false;
	s32 month = stringDate.section('/', 1, 1).toInt(&ok);
	if (!ok)
		return false;
	s32 year = stringDate.section('/', 2, 2).toInt(&ok);
	if (!ok)
		return false;
	
	QDate date(year, month, day);

	f64 after = reg.section(' ', 3, 3).toDouble(&ok); 

	if (!ok)
		return false;

	f64 before = reg.section(' ', 4, 4).toDouble(&ok); 

	if (!ok)
		return false;

	u64 tradesAfter = reg.section(' ', 5, 5).toLongLong(&ok); 

	if (!ok)
		return false; 

	u64 tradesBefore = reg.section(' ', 6, 6).toLongLong(&ok); 

	if (!ok)
		return false;

	core::VizCSplit split(tradesBefore, tradesAfter, after, before, date);

	return (DbUtils->insertCheckingSplit(cnpj, split));	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------