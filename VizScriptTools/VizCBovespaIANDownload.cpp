//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCBovespaIANDownload.h"
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#include <QByteArray>
#include <QDate>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <windows.h>
#include "zip.h"
#include "unzip.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server 
{
//-----------------------------------------------------------------------------
static const QString WorkDir = "IANFiles";
static const QString CnpjCCVMFileName = "CnpjCCVM.txt";
static const QString SplitsFileName = "splits.txt";
//-----------------------------------------------------------------------------
VizCBovespaIANDownload::VizCBovespaIANDownload() 
{

	WebPageCNPJAndCCMV = new QWebPage(this);
	connect(WebPageCNPJAndCCMV, SIGNAL(loadProgress(int)), this, SLOT(slotGetCNPJAndCCMVProgress(int)));
	connect(WebPageCNPJAndCCMV, SIGNAL(loadFinished(bool)), this, SLOT(slotGetCNPJAndCCMVFinished(bool)));

	NetworkAccessManagerDownloadXML = new QNetworkAccessManager(this);
	connect(NetworkAccessManagerDownloadXML, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinishedDownloadXML(QNetworkReply*)));

	NetworkAccessManagerDownloadWAN = new QNetworkAccessManager(this);
	connect(NetworkAccessManagerDownloadWAN, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinishedDownloadWAN(QNetworkReply*)));
	
}
//-----------------------------------------------------------------------------
VizCBovespaIANDownload::~VizCBovespaIANDownload()
{
	
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::getIANFiles()
{

	QDir checkForDataDir(WorkDir);
	if(!checkForDataDir.exists())
	{
		QDir createDirectory;
		createDirectory.mkdir(WorkDir);
	}


	getCNPJAndCCMV();

}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::getCNPJAndCCMV()
{

	if(!loadCNPJAndCCMVFromFile())
	{
		QUrl url("http://cvmweb.cvm.gov.br/SWB/Sistemas/SCW/CPublica/CiaAb/ResultBuscaParticCiaAb.aspx?CNPJNome=&TipoConsult=C");
		WebPageCNPJAndCCMV->mainFrame()->load(url);
	}
	else
	{
		getIANFilesInAllDate();
	}
	

}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::slotGetCNPJAndCCMVFinished(bool ok)
{
	if(ok)
	{
		QWebFrame* frame = WebPageCNPJAndCCMV->mainFrame();
		QWebElement document = frame->documentElement();

		QWebElementCollection trs = document.findAll("tr[bgcolor=\"Cornsilk\"]");

		foreach(QWebElement tr, trs)
		{
			QWebElement cnpj = tr.findFirst("td[align=\"Center\"][width=\"20%\"]");
			QWebElement ccmv = tr.findFirst("td[align=\"Left\"][width=\"10%\"]");

			QWebElement spam = cnpj.firstChild();
			QString cnpj_str = spam.toPlainText();
			QWebElement a = ccmv.firstChild();
			QString ccmv_str = a.toPlainText();

			IAN ian;
			ian.CCVM = ccmv_str;
			ian.Cnpj = cleanCNPJ(cnpj_str);

			IANMap[ccmv_str] = ian;
		}

		QWebElementCollection trs1 = document.findAll("tr[bgcolor=\"#FAEFCA\"]");
		foreach(QWebElement tr, trs1)
		{
			QWebElement cnpj = tr.findFirst("td[align=\"Center\"][width=\"20%\"]");
			QWebElement ccmv = tr.findFirst("td[align=\"Left\"][width=\"10%\"]");

			QWebElement spam = cnpj.firstChild();
			QString cnpj_str = spam.toPlainText();
			QWebElement a = ccmv.firstChild();
			QString ccmv_str = a.toPlainText();

			IAN ian;
			ian.CCVM = ccmv_str;
			ian.Cnpj = cleanCNPJ(cnpj_str);

			IANMap[ccmv_str] = ian;
		}

		saveCNPJAndCCMVToFile();

		getIANFilesInAllDate();
	}
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::slotGetCNPJAndCCMVProgress(int progress)
{
	qDebug() << "Downloading CNPJ And CCMV:" << progress << "%";
}
//-----------------------------------------------------------------------------
bool VizCBovespaIANDownload::loadCNPJAndCCMVFromFile()
{
	QFile file(WorkDir + "/" + CnpjCCVMFileName);

	
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text) || file.size() == 0)
	{
		return false;
	}


	IANMap.clear();

	QTextStream fileStream(&file);

	while(!fileStream.atEnd())
	{
		QString ccmv_str;
		QString cnpj_str;

		fileStream >> cnpj_str >> ccmv_str;

		IAN ian;
		ian.CCVM = ccmv_str;
		ian.Cnpj = cnpj_str;

		IANMap[ccmv_str] = ian;

	}

	file.close();

	qDebug() << "Loaded CNPJ And CCMV";

	return true;

}
//-----------------------------------------------------------------------------
bool VizCBovespaIANDownload::saveCNPJAndCCMVToFile()
{
	QFile file(WorkDir + "/" + CnpjCCVMFileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	
	QTextStream fileStream(&file);

	foreach(QString key, IANMap.keys())
	{
		const IAN& ian = IANMap[key];

		fileStream << ian.Cnpj << ' ' << ian.CCVM << '\n';
		
	}

	file.close();

	qDebug() << "Saved CNPJ And CCMV: " << WorkDir + "/" + CnpjCCVMFileName;

	return true;

}
//-----------------------------------------------------------------------------
QString VizCBovespaIANDownload::cleanCNPJ(const QString& cnpj)
{
	//01.547.749/0001-16
	return cnpj.simplified().remove(QChar('.')).remove(QChar('/')).remove(QChar('-'));

}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::getIANFilesInAllDate()
{
	QDate current = QDate::currentDate();
	s32 current_year = current.year();
	
	Finished = false;

	while(current.year() != 1995)
	{
		getIANFileInDate(current);
		current = current.addDays(-1);
	}
	
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::getIANFileInDate(const QDate& date)
{
	QString user = "397DWLLINUSMO";
	QString pass = "20visual1tic409";
	//QString datestring = QString("%1/%2/%3").arg(date.day(), 2).arg(date.month(), 2).arg(date.year(), 4);

	QString datestring = date.toString("dd/MM/yyyy");

	QString timestring = "00:00";
	QString documentstring = "IAN";

	qDebug() << datestring;


	//txtLogin 397DWLLINUSMO
	//txtSenha nova2009 -> 20visual1tic409
	//txtData
	//txtHora
	//txtDocumento IAN

	QByteArray dataToSend;
	QString data = QString("txtLogin=%1&txtSenha=%2&txtData=%3&txtHora=%4&txtDocumento=%5").arg(user).arg(pass).arg(datestring).arg(timestring).arg(documentstring);
	dataToSend.insert(0,data);
	QNetworkRequest request;
	request.setUrl(QUrl("https://WWW.RAD.CVM.GOV.BR/DOWNLOAD/SolicitaDownload.asp"));
	NetworkAccessManagerDownloadXML->post(request, dataToSend);

}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::parseXML(QXmlStreamReader& xml, QString& date, QMap<QString, QString>& ccmv_and_links)
{

	ccmv_and_links.clear();

	while(!xml.atEnd())
	{
		QXmlStreamReader::TokenType type = xml.readNext();

		if(xml.isCDATA())
		{
			continue;
		}

		if(xml.isStartElement())
		{
			if(xml.name() == "Link")
			{
				if
				(
					xml.attributes().value("Documento").toString() == "IAN" &&
					xml.attributes().value("Situacao").toString() == "Liberado"
				)
				{
					QString url = xml.attributes().value("url").toString();
					QString ccvm = xml.attributes().value("ccvm").toString();
					QString dateStr = xml.attributes().value("DataRef").toString();
					QString dateFormat = xml.attributes().value("FrmDtRef").toString();
					dateFormat.replace("a", "y");
					dateFormat.replace("m", "M");
					QDate d = QDate::fromString(dateStr, dateFormat);
					date = d.toString("dd/MM/yyyy");

					//FrmDtRef
					//dd/mm/aaaa

					if(!ccmv_and_links.contains(ccvm))
					{
						//qDebug() << ccvm << "URL: ADDED OK";
						ccmv_and_links[ccvm] = url;
					}
					/*else
					{
						qDebug() << ccvm << "URL: ALREADY CONTAINS";
					}*/

				}
			}
			else if(xml.name() == "ERROS")
			{
				//qDebug() << "ERRORS";
				QXmlStreamReader::TokenType type = xml.readNext();
				if(xml.name() == "NUMERO_DO_ERRO")
				{
					//qDebug() << "NUMERO_DO_ERRO";
					QXmlStreamReader::TokenType type = xml.readNext();
					if(xml.isCharacters())
					{
						if(xml.text() == "22016")
						{
							qDebug() << "NUMERO_DO_ERRO" << xml.text() << "Nenhum registro localizado";
						}
						else
						{
							qDebug() << "NUMERO_DO_ERRO" << xml.text();
						}
					}
				}
				return;
			}
		}
	}
	if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
	{
		qDebug() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
	}
	//<Link url="http://siteempresas.bovespa.com.br/DXW/FormDetalheDownload.asp?ccvm=3891&amp;site=C&amp;data=31/12/2008&amp;moeda=L&amp;tipo=3" Documento="IAN" ccvm="3891" DataRef="31/12/2008" FrmDtRef="dd/mm/aaaa" Situacao="Liberado"  />


}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::sincronizeIANMap(const QString& date, QMap<QString, QString>& ccmv_and_links)
{
	Finished = true;

	foreach(QString key, ccmv_and_links.keys())
	{
		if(IANMap.contains(key))
		{
			//if(IANMap[key].LinkToDownload.isEmpty())
			{
				Finished = false;
				IANMap[key].DateAndLinkToDownload[date].Link = ccmv_and_links[key];
			}
		}
	}
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::downloadIANMap()
{

	foreach(QString key, IANMap.keys())
	{
		IAN& ian = IANMap[key];
		
		foreach(QString date, ian.DateAndLinkToDownload.keys())
		{
			IANLink& l = ian.DateAndLinkToDownload[date];
			if(!l.Downloading)
			{
				l.Downloading = true;
				QUrl url(l.Link);
				QString key = ianFileKeyName(ian.CCVM, date);
				QString wd = WorkDir+'/'+ian.CCVM;
				QString fileName = QString("%1/%2.zip").arg(wd).arg(key);
				if(!QFile::exists(fileName))
				{
					QNetworkRequest request;
					request.setUrl(url);
					NetworkAccessManagerDownloadWAN->get(request);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::slotReplyFinishedDownloadXML(QNetworkReply* reply)
{

	qDebug() << "chegou um xml";

	QByteArray data = reply->readAll();
	QXmlStreamReader XML;
	XML.addData(data);

	QMap<QString, QString> ccmv_and_links;
	QString date;
	parseXML(XML, date, ccmv_and_links);
	sincronizeIANMap(date, ccmv_and_links);

	downloadIANMap();

}
//-----------------------------------------------------------------------------
QString VizCBovespaIANDownload::ianFileKeyName(const QString& ccmv, const QString& datestr)
{
	QDate d = QDate::fromString(datestr, "dd/MM/yyyy");
	QString date = d.toString("dd-MM-yyyy");
	return QString("%1_%2").arg(ccmv).arg(date);
}
//-----------------------------------------------------------------------------
void VizCBovespaIANDownload::slotReplyFinishedDownloadWAN(QNetworkReply* reply)
{

	QUrl url(reply->request().url());
	QString ccmv = url.queryItemValue("ccvm");
	QString datestr = url.queryItemValue("data");

	qDebug() << "chegou um wan" << url;

	QString key = ianFileKeyName(ccmv, datestr);

	QString wd = WorkDir+'/'+ccmv;
	QDir checkForDataDir(wd);
	if(!checkForDataDir.exists())
	{
		QDir createDirectory;
		createDirectory.mkdir(wd);
	}

	QString fileName = QString("%1/%2.zip").arg(wd).arg(key);
	QByteArray data = reply->readAll();
	QFile file(fileName);
	bool ok = file.open(QIODevice::WriteOnly);
	QDataStream dataStream(&file);
	dataStream.writeRawData(data.constData(), data.size());

	file.close();
	


}
//-----------------------------------------------------------------------------
VizCBovespaIANSplitGenerator::VizCBovespaIANSplitGenerator()
{
}
//-----------------------------------------------------------------------------
VizCBovespaIANSplitGenerator::~VizCBovespaIANSplitGenerator()
{
}
//-----------------------------------------------------------------------------
bool VizCBovespaIANSplitGenerator::generateIANBonde()
{
	if(!loadCNPJAndCCMVFromFile())
	{
		return false;
	}

	foreach(QString key, IANBondeMap.keys())
	{
		IANBONDE& ian = IANBondeMap[key];

		qDebug() << "get IAN FILE from zip: " << ian.CCVM;

		QStringList filesbonde = getIANBondeFileFromZipFiles(ian.CCVM);
		parseIANBondeFile(filesbonde, ian);
	}

	bool b = saveIANBonde();

	return b;

}
//-----------------------------------------------------------------------------
bool VizCBovespaIANSplitGenerator::loadCNPJAndCCMVFromFile()
{
	QFile file(WorkDir + "/" + CnpjCCVMFileName);


	if(!file.open(QIODevice::ReadOnly | QIODevice::Text) || file.size() == 0)
	{
		return false;
	}

	IANBondeMap.clear();

	QTextStream fileStream(&file);

	while(!fileStream.atEnd())
	{
		QString ccmv_str;
		QString cnpj_str;

		fileStream >> cnpj_str >> ccmv_str;

		IANBONDE ian;
		ian.CCVM = ccmv_str;
		ian.CNPJ = cnpj_str;

		IANBondeMap[ccmv_str] = ian;

	}

	file.close();


	qDebug() << "Loaded CNPJ And CCMV";

	return true;

}
//-----------------------------------------------------------------------------
QStringList VizCBovespaIANSplitGenerator::getIANBondeFileFromZipFiles(const QString& ccvm)
{

	QStringList out;


	QDir dir(WorkDir + "/" + ccvm);
	if(!dir.exists())
	{
		qDebug() << "nao existe diretorio: " + ccvm;
		return out;
	}

	
	QFileInfoList list = dir.entryInfoList(QStringList() << "*.zip", QDir::Files | QDir::NoSymLinks, QDir::NoSort);
	for(int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		QString fileName = WorkDir + "/" + ccvm + "/" + fileInfo.fileName();

		HZIP hz = OpenZip((wchar_t*)fileName.utf16(), 0);
		ZIPENTRY ze;
		GetZipItem(hz, -1, &ze);
		int numitems = ze.index;
		// -1 gives overall information about the zipfile
		for(int zi = 0; zi < numitems; zi++)
		{
			ZIPENTRY ze;
			GetZipItem(hz,zi,&ze); // fetch individual details

			QString zipname = QString::fromUtf16(ze.name);

			//qDebug() << zipname;

			if(zipname.contains("IANBonde.001", Qt::CaseInsensitive))
			{
				qDebug() << fileName + "/IANBonde.001";

				if(ze.unc_size > 0)
				{
					char* ibuf = new char[ze.unc_size];
					for(int j = 0; j < ze.unc_size; j++)
					{
						ibuf[j] = 0;
					}
					UnzipItem(hz, zi, ibuf, ze.unc_size);

					QString fileText = QString::fromAscii(ibuf);

					out << fileText;

					delete[] ibuf;
				}

			}

		}
		CloseZip(hz);


	}


	return out;



}
//-----------------------------------------------------------------------------
void VizCBovespaIANSplitGenerator::parseIANBondeFile(const QStringList& bondes, IANBONDE& data)
{

	foreach(QString bonde, bondes)
	{
		//int genYear = head.mid(23, 4).toInt(&ok);
		/*
		ITEM	2	04.03.01	número do item
		CODCVM	6	01.01.01	código da empresa na CVM
		DATAIAN	8	01.05.02	data do IAN (formato AAAAMMDD)
		DTAG	8	04.03.02	data da Aprovação (formato AAAAMMDD)
		VLANTESAG	16	04.03.03	valor nominal por ações antes da aprovação - Reais - formato (99999.9999999999)
		VLDEPOISAG	16	04.03.04	valor nominal por ações depois da aprovação - Reais 	formato (99999.9999999999)
		QTANTESAG	12	04.03.05	quantidade de ações antes da aprovação - escala
		QTDEPOISAG	12	04.03.06	quantidade de ações depois da aprovação - escala
		*/

		QTextStream stream(&bonde); // Set the stream to read from myFile
		QString line;
		
		while(true)
		{
			line = stream.readLine();

			line = line.trimmed();

			if(line.isEmpty() || line.isNull())
				break;
			

			QString ITEM = line.mid(0, 2);
			QString CODCVM = line.mid(2, 6);
			QString DATAIAN = line.mid(8, 8);
			QString DTAG = line.mid(16, 8);
			QString VLANTESAG = line.mid(24, 16);
			QString VLDEPOISAG = line.mid(40, 16);
			QString QTANTESAG = line.mid(56, 12);
			QString QTDEPOISAG = line.mid(68, 12);

			IANBONDEVALUE bondevalue;

			bondevalue.Date = QDate::fromString(DTAG, "yyyyMMdd");

			if(!bondevalue.Date.isValid())
			{
				break;
			}



			bondevalue.VLANTESAG = VLANTESAG.toDouble();
			bondevalue.VLDEPOISAG = VLDEPOISAG.toDouble();
			bondevalue.QTANTESAG = QTANTESAG.toULongLong();
			bondevalue.QTDEPOISAG = QTDEPOISAG.toULongLong();

			QString key = bondeKey(bondevalue.Date);

			if(!data.Values.contains(key))
			{
				data.Values[key] = bondevalue;
			}
			else
			{
				qDebug() << "ja tem";
			}
		}
	}

}
//-----------------------------------------------------------------------------
bool VizCBovespaIANSplitGenerator::saveIANBonde()
{
	QFile file(WorkDir + "/" + SplitsFileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;


	QTextStream fileStream(&file);

	foreach(QString key1, IANBondeMap.keys())
	{
		const IANBONDE& ianbonde = IANBondeMap[key1];

		foreach(QString key2, ianbonde.Values.keys())
		{

			const IANBONDEVALUE& ianbondevalue = ianbonde.Values[key2];

			fileStream
				<< ianbonde.CNPJ << ' '
				<< ianbonde.CCVM << ' '
				<< ianbondevalue.Date.toString("dd/MM/yyyy") << ' '
				<< ianbondevalue.VLANTESAG << ' '
				<< ianbondevalue.VLDEPOISAG << ' '
				<< ianbondevalue.QTANTESAG << ' '
				<< ianbondevalue.QTDEPOISAG << '\n';
		}

	}

	file.close();

	qDebug() << "Saved IAN BONDE: " << WorkDir + "/" + SplitsFileName;

	return true;

}
//-----------------------------------------------------------------------------
QString VizCBovespaIANSplitGenerator::bondeKey(const QDate& date)
{
	return date.toString("dd/MM/yyyy");
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
