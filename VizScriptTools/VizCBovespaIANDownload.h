//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCBOVESPAIANDOWNLOADH__
#define __VIZCBOVESPAIANDOWNLOADH__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QObject>
#include <QList>
#include <QWebPage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>
#include <QDate>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
struct IANLink
{
	IANLink() : Downloading(false)
	{
	}
	QString Link;
	bool Downloading;
};
struct IAN
{
	QString CCVM;
	QString Cnpj;
	QMap<QString, IANLink> DateAndLinkToDownload;
};
//-----------------------------------------------------------------------------
class VizCBovespaIANDownload: public QObject
{	
	Q_OBJECT

public:
	VizCBovespaIANDownload();
	~VizCBovespaIANDownload();

	void getIANFiles();

private:
	void getCNPJAndCCMV();
	void getIANFilesInAllDate();
	void getIANFileInDate(const QDate& date);

	bool loadCNPJAndCCMVFromFile();
	bool saveCNPJAndCCMVToFile();
	QString cleanCNPJ(const QString& cnpj);

	void parseXML(QXmlStreamReader& xml, QString& date, QMap<QString, QString>& ccmv_and_links);
	void sincronizeIANMap(const QString& date, QMap<QString, QString>& ccmv_and_links);
	void downloadIANMap();

	QString ianFileKeyName(const QString& ccmv, const QString& datestr);

	bool Finished;
	QMap<QString, IAN> IANMap;
	QWebPage* WebPageCNPJAndCCMV;
	QNetworkAccessManager* NetworkAccessManagerDownloadXML;
	QNetworkAccessManager* NetworkAccessManagerDownloadWAN;
	

private slots:
	void slotGetCNPJAndCCMVFinished(bool ok);
	void slotGetCNPJAndCCMVProgress(int progress);
	void slotReplyFinishedDownloadXML(QNetworkReply* reply);
	void slotReplyFinishedDownloadWAN(QNetworkReply* reply);
	
	
};
//-----------------------------------------------------------------------------
struct IANBONDEVALUE
{
	QDate Date;						//data da Aprovação
	qreal VLANTESAG;				//valor nominal por ações antes da aprovação - Reais 
	qreal VLDEPOISAG;				//valor nominal por ações depois da aprovação - Reais 
	unsigned long long QTANTESAG;	//quantidade de ações antes da aprovação - escala
	unsigned long long QTDEPOISAG;	//quantidade de ações depois da aprovação - escala
};
struct IANBONDE
{
	QString CNPJ;
	QString CCVM;
	bool OK;

	QMap<QString, IANBONDEVALUE> Values;

};
//-----------------------------------------------------------------------------
class VizCBovespaIANSplitGenerator
{

public:
	VizCBovespaIANSplitGenerator();
	~VizCBovespaIANSplitGenerator();

	bool generateIANBonde();

	/*
	agora eh ler os cnpj e ccvm e ler os zip e retirar os bonde.001 
	*/

	//qUncompress()


private:

	QMap<QString, IANBONDE> IANBondeMap;

	bool loadCNPJAndCCMVFromFile();
	bool saveIANBonde();
	QString bondeKey(const QDate& date);
	
	QStringList getIANBondeFileFromZipFiles(const QString& ccvm);
	void parseIANBondeFile(const QStringList& bondes, IANBONDE& data);

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif