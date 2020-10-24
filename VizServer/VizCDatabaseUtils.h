//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCDATABASEUTILSH__
#define __VIZCDATABASEUTILSH__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QPair>
#include <QMutexLocker>

#include "VizCDatabase.h"
#include "VizCLog.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCDatabaseUtils : public QObject
{	
	Q_OBJECT
public:
	// eager initialized singleton
	static VizCDatabaseUtils* instance();

	// unsafe
	bool insertCompanyCCVM(const QString& cnpj, const u32& ccvm);
	bool insertCheckingFullStockRegister(core::VizCCompany& company);
	bool insertNoCheckFullStockRegister(QList<core::VizCCompany>& companies);
	bool removeLastNegotiableRegisters(const QDate& oldDate);
	bool insertCheckingSplit(QString cnpj, core::VizCSplit& split);
	
	// thread-safe
	bool isAuthenticationPassed(QString login, QString md5Password);	
	bool getNegotiableList(QVector<QStringList>& negotiableList, const QDate& clientLastDate = QDate());
	bool getRemovedNegotiableElements(QVector<QString>& removedList, const QDate& clientLastDate);
	//bool getNegotiableList(QVector<QPair<QString, QString> >& negotiableList, const QDate& clientLastDate = QDate());
	//bool getRemovedNegotiableElements(QVector<QPair<QString, QString> >& removedList, const QDate& clientLastDate);
	bool getSecurity(const QString securityId, core::VizCCompany& company, const QDate& clientLastDate = QDate());
private:	
	static VizCDatabaseUtils* Instance;

	QMutex mutexGetSecurity;
	QMutex mutexAuthentication;
	QMutex mutexGetRemovedElements;
	QMutex mutexGetNegotiableElements;

	VizCDatabaseUtils();
	~VizCDatabaseUtils();

	bool insertCheckingFullCompany(core::VizCCompany& company);
	
	bool insertCheckingFullSecurity(const s32& companyId, core::VizCSecurity& security);
	// Por enquanto não é necessário o full period
	bool insertSinglePeriod(const s32& securityId, const core::VizCSecurityPeriod& period, const bool& isTerm = false);

	VizCDatabase* Database;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif