//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCCCVMToDatabase.h"

#include <QFile>
#include <QTextStream>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCCCVMToDatabase::VizCCCVMToDatabase(QObject* parent) : QObject(parent)
{
	DbUtils = VizCDatabaseUtils::instance();

	openFile();
}
//-----------------------------------------------------------------------------
VizCCCVMToDatabase::~VizCCCVMToDatabase()
{
}
//-----------------------------------------------------------------------------
bool VizCCCVMToDatabase::insertCCVM(const QString& cnpj, const u32& ccvm)
{
	return DbUtils->insertCompanyCCVM(cnpj, ccvm);
}
//-----------------------------------------------------------------------------
bool VizCCCVMToDatabase::openFile(QString fileName)
{
	QFile finput(fileName);

	if (!finput.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QByteArray inputFromBovespa = finput.readAll();

	finput.close();

	QTextStream in(&inputFromBovespa, QIODevice::ReadOnly);

	u32 ccvm(0);
	QString cnpj;
	bool ok(false);
	
	while (!in.atEnd()) 
	{
		QString line = in.readLine();
		cnpj = line.section(' ', 0, 0);
		ccvm = line.section(' ', 1, 1).toInt(&ok); 

		if (!ok || (!insertCCVM(cnpj, ccvm)))
			return false;
	}

	return true;	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
