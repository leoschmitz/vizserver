//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCBOVESPALISTTOFILEH__
#define __VIZCBOVESPALISTTOFILEH__
//-----------------------------------------------------------------------------
#include <VizCDatabaseUtils.h>
#include <VizTypes.h>

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

class VizCBovespaListToFile: public QObject
{	
	Q_OBJECT

public:
	bool openMegaBolsaEmissorFile(QString fileName);
	bool openMegaBolsaNegotiableFile(QString fileName);

	VizCBovespaListToFile();
	~VizCBovespaListToFile();

	VizCDatabaseUtils* DbUtils;
private:	
	bool readEmissorLine(QString line);

	bool readNegotiableHeader(QString head);
	bool readNegotiableCompanyRegister(QString reg, core::VizCCompany& company);
	bool readNegotiableSecurityRegister(QString reg, core::VizCCompany& company);
	bool readNegotiableTrailer(QString trailere);	
};

// Nem todas substituicoes foram feitas
// Deve ser investigada a utilidade de algumas expansoes
static QString expandSecurityNameAbreviations(QString name)
{
	QString newString(name);
	newString.replace("BCO ", "BANCO ", Qt::CaseInsensitive); 
	newString.replace("AGE ", "ASSEMBLÉIA GERAL EXTRAORDINARIA ", Qt::CaseInsensitive); 
	newString.replace("AGROINDL ", "AGRO-INDUSTRIAL ", Qt::CaseInsensitive); 
	newString.replace("ARREND ", "ARRENDAMENTO ", Qt::CaseInsensitive); 
	newString.replace("FDO ", "FUNDO ", Qt::CaseInsensitive); 
	newString.replace("COM ", "COMÉRCIO ", Qt::CaseInsensitive); 
	newString.replace("COML ", "COMERCIAL ", Qt::CaseInsensitive);
	return newString;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif