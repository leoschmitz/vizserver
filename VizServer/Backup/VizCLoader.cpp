//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCLoader.h"
#include "VizCSecurityPeriodsManipulator.h"
#include <QTextStream>
#include <QFile>
#include <QtGui>
#include <QTreeView>
#include <QHeaderView>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QApplication>
//-----------------------------------------------------------------------------
namespace viz
{
namespace io
{
//-----------------------------------------------------------------------------
VizCLoader* VizCLoader::Instance = 0;
//-----------------------------------------------------------------------------
VizCLoader::VizCLoader(QWidget *parent) : SecurityCache(), QDialog(parent), LastLoad(""), LastLoadOk(false)
{
	PartialSecurityInfo = new VizCSearchModel(this);

	setModal(true);
	setWindowTitle(tr("Abrir Ativo"));

	loadSecurityNames();

	QStringList labels;
	labels <<  tr("Símbolo") << tr("Nome") << tr("Local");
	PartialSecurityInfo->setHorizontalHeaderLabels(labels);

	LineEdit = new VizCSearchLineEdit;

	SearchSecurities = new VizCSearchEngine();		
	SearchSecurities->sortSecurities(PartialSecurityInfo);	

	LineEdit->setMinimumSize(350, 10);	
	LineEdit->setCompleter(SearchSecurities);

	SearchSecurities->completerView()->rearrange();

	SearchSecurities->browserView()->setModel(SearchSecurities->completionModel());

	QObject::connect(SearchSecurities->browserView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(insertBrowserCompletion(QModelIndex)));	
	QObject::connect(SearchSecurities->browserView(), SIGNAL(clicked(QModelIndex)), LineEdit, SLOT(insertCompletion(QModelIndex)));
	QObject::connect(LineEdit, SIGNAL(textChanged(const QString &)), SearchSecurities, SLOT(checkActiveColumn(const QString &)));
	QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose(int)));	

	QHBoxLayout *layout = new QHBoxLayout;
	QVBoxLayout *vlayout = new QVBoxLayout;
	OpenButton = new QPushButton(tr("Abrir"));
	CancelButton = new QPushButton(tr("Cancelar"));

	QObject::connect(OpenButton, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	layout->addWidget(OpenButton);
	layout->addWidget(CancelButton);

	vlayout->addWidget(SearchSecurities->browserView());	
	vlayout->addWidget(completerLineEdit());	
	vlayout->addLayout(layout);
	setLayout(vlayout);	
}
//-----------------------------------------------------------------------------
VizCLoader::~VizCLoader()
{

}
//-----------------------------------------------------------------------------
void VizCLoader::insertBrowserCompletion(const QModelIndex& index)
{ 
	QString secId = SearchSecurities->model()->data(SearchSecurities->model()->index(index.row(), 0)).toString();
	LineEdit->setText(secId);
	accept();
}
//-----------------------------------------------------------------------------
VizCLoader* VizCLoader::instance()
{
	if(!Instance)
	{
		Instance = new VizCLoader;
	}
	return Instance;
}

//-----------------------------------------------------------------------------
void VizCLoader::onClose(int closeCondition)
{
	if (closeCondition == 0)
	{
		LastLoadOk = false;				
		LineEdit->clear();
		return;
	}
	LastLoad = LineEdit->text().toUpper();	
	LineEdit->clear();
	SecurityCache.loadSecurity(LastLoad);		
	LastLoadOk = SecurityCache.contains(LastLoad);		
}
//-----------------------------------------------------------------------------
core::VizSecurity VizCLoader::security(const QString& securityId)
{
	return SecurityCache.security(securityId);
}
//-----------------------------------------------------------------------------
bool VizCLoader::insertSecurityHeader(QString partialSecurity, QString wholeName)
{
	if (SecurityHeaders.contains(partialSecurity))
		return false;

	SecurityHeaders.insert(partialSecurity, wholeName);

	return true;
}
//-----------------------------------------------------------------------------
QLineEdit* VizCLoader::completerLineEdit()
{
	return LineEdit;
}
//-----------------------------------------------------------------------------
bool VizCLoader::insertSecuritySymbol(QString symbol)
{
	if (!SecurityHeaders.contains(symbol.mid(0, 4)))
		return false;

	return insertSecurityInfo(symbol, SecurityHeaders.value(symbol.mid(0, 4)));
}
//-----------------------------------------------------------------------------
QList<QStandardItem*> VizCLoader::mountRegister(QString symbol, QString wholeName)
{
	QList<QStandardItem*> items;

	QStandardItem* item = new QStandardItem(QString(symbol));
	items.push_back(item);	

	item = new QStandardItem(wholeName);
	items.push_back(item);

	if (SecurityCache.isLocal(symbol))
		item = new QStandardItem(QString(tr("Sim")));
	else item = new QStandardItem(QString(tr(" ")));

	items.push_back(item);

	return items;
}
//-----------------------------------------------------------------------------
bool VizCLoader::insertSecurityInfo(QString symbol, QString wholeName, int row)
{
	if (row > PartialSecurityInfo->rowCount())
		return false;

	QList<QStandardItem*> items = mountRegister(symbol, wholeName);	

	if (row == -1)
		PartialSecurityInfo->appendRow(items);	
	else
	{
		//PartialSecurityInfo->setItem(row, 0, items.at(0));
		//PartialSecurityInfo->setItem(row, 1, items.at(1));
		//PartialSecurityInfo->setItem(row, 2, items.at(2));
		PartialSecurityInfo->insertRow(row, items);		
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCLoader::saveSecurityList()
{
	//////////////////////////////////////////////////////////////////////////
	// Formato do Arquivo
	// ==================
	// (QString) VizList
	// (QString) Version
	// (QString) SimboloCompleto
	// (QString) NomeCompleto
	// (quint16) crc
	// ...
	// 
	//////////////////////////////////////////////////////////////////////////
	io::VizISecurityFile::setDataDirectory();

	QFile file(io::NameOfTheFileWhichHasCompleteList);

	if (!file.open(QIODevice::WriteOnly))
		return false;

	QDataStream fileStream(&file);

	QString vizList = "VizList";

	fileStream << vizList;
	fileStream << io::VersionOfTheFileWhichHasCompleteList;
	for (int i = 0; i < PartialSecurityInfo->rowCount(); i++)
	{
		QVariant vsymbol = PartialSecurityInfo->item(i, 0)->data(Qt::DisplayRole);
		QString symbol = vsymbol.toString();
		QVariant vwholeName = PartialSecurityInfo->item(i, 1)->data(Qt::DisplayRole);
		QString wholeName = vwholeName.toString();

		fileStream << symbol << wholeName << io::listCheckSum(symbol, wholeName);
	}

	file.close();

	return true;	
}
//-----------------------------------------------------------------------------
bool VizCLoader::loadSecurityNames()
{
	VizISecurityFile::setDataDirectory();

	QFile file(NameOfTheFileWhichHasCompleteList);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	QByteArray fileWithNames = file.readAll();

	file.close();

	quint16 crc(0);
	QString vizList;
	QString version;
	QString symbol;
	QString wholeName;

	QDataStream fileStream(&fileWithNames, QIODevice::ReadOnly);

	fileStream >> vizList >> version;
	if ((vizList != "VizList") ||(version != VersionOfTheFileWhichHasCompleteList))
		return false;


	//versao 1
	//3264 milisec
	/*s32 inc = 0;
	while(!fileStream.atEnd())
	{
	fileStream >> symbol >> wholeName >> crc;
	if(crc != listCheckSum(symbol, wholeName))
	{
	return false;
	}

	insertSecurityInfo(symbol, wholeName, inc);
	inc++;
	}*/


	//versao 2
	//2593 milisec
	QVector< QPair<QString, QString> > container;
	while(!fileStream.atEnd())
	{
		fileStream >> symbol >> wholeName >> crc;
		if(crc != listCheckSum(symbol, wholeName))
		{
			return false;
		}
		container.push_back(QPair<QString, QString>(symbol, wholeName));
	}
	for(s32 i = 0; i < container.size(); i++)
	{
		insertSecurityInfo(container[i].first, container[i].second, i);
	}


	/*
	//versao 3
	//3235 milisec
	int rowCount = 0;
	while (!fileStream.atEnd())
	{
	fileStream >> symbol >> wholeName >> crc;
	if (crc != listCheckSum(symbol, wholeName))
	return false;	

	insertSecurityInfo(symbol, wholeName, rowCount);

	rowCount++;
	}
	*/

	return true;
}
//-----------------------------------------------------------------------------
QVector<core::VizSecurity> VizCLoader::securities()
{
	return SecurityCache.securities();
}
//-----------------------------------------------------------------------------
QVector<core::VizSecurityPeriod> VizCLoader::securityPeriods(const core::VizSecurity& security, viz::VizESecurityPeriodicity securityPeriodicity, const QDateTime& startDate, const QDateTime& endDate)
{
	return SecurityCache.securityPeriods(security, securityPeriodicity, startDate, endDate);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
