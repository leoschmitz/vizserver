//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "VizCLog.h"

//#include <QMutexLocker>
#include <QTextStream>
#include <QDir>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
const char* GlobalLogDirectory = "Log\\";
VizCServerLog* VizCServerLog::Instance = 0;
//-----------------------------------------------------------------------------
VizCServerLog* VizCServerLog::instance()
{	
	if(!Instance)
	{
		Instance = new VizCServerLog;
	} 
	return Instance;
}
//-----------------------------------------------------------------------------
VizCServerLog::VizCServerLog(QObject* parent) : QObject(parent)
{
	QDir dir(GlobalLogDirectory);
	
	if (!dir.exists())
	{
		QDir createDirectory;
		createDirectory.mkdir(GlobalLogDirectory);
	}

	QString fileName = QString("%1server_log_%2").arg(GlobalLogDirectory).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
	fileName.remove(':');

	Traces.setFileName(fileName);			
}
//-----------------------------------------------------------------------------
VizCServerLog::~VizCServerLog()
{
	
}
//-----------------------------------------------------------------------------
void VizCServerLog::appendToDoc(const QString& s)
{
	//QMutexLocker lock (&TimerProtectMutex);

	this->blockSignals(true);

	QString in = QString("%1: %2\n").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(s);

	if (!Traces.open(QIODevice::Append))
		qDebug() << "Problema na escrita do arquivo.";

	QTextStream fileStream(&Traces); 

	fileStream << in;

	Traces.close();

	this->blockSignals(false);
}
//-----------------------------------------------------------------------------
void VizCServerLog::append(const char* c)
{
	appendToDoc(c);
}
//-----------------------------------------------------------------------------
void VizCServerLog::append(const QString& s)
{	
	appendToDoc(s);
}
//-----------------------------------------------------------------------------
void VizCServerLog::append(const s32& i)
{
	appendToDoc(QString::number(i));
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
