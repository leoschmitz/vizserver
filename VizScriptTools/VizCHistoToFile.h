//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCHISTOTOFILEH__
#define __VIZCHISTOTOFILEH__
//-----------------------------------------------------------------------------
#include <VizTypes.h>
#include <VizCCompany.h>
#include <VizCDatabaseUtils.h>

#include <QObject>
#include <QDir>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
// Por enquanto essa classe atualiza o arquivo copiando todo seu conteudo para
// memoria e sobrescrevendo o arquivo antigo.
//-----------------------------------------------------------------------------
class VizCHistoToFile: public QObject
{	
	Q_OBJECT

public:
	bool openMegaBolsaFile(QString fileName);

	VizCHistoToFile();
	~VizCHistoToFile();
private:	
	VizCDatabaseUtils* DbUtils;
	QMap<QString, core::VizCSecurity> HistoricSecurities;
	QMap<QString, core::VizCCompany> HistoricCompanies;

	bool readHeader(QString head);
	bool readRegister(QString reg);
	bool readTrailer(QString trailer);	

	static int makeISINcheckBit(QString isin);
	static QString makeISIN(QString possibleCODINT);
	static VizESecurityBDI fromHistoBDIToVizBDI(int bdiCode);

	void mountCompanies();
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif