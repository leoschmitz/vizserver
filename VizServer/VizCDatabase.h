//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCDATABASE_H__
#define __VIZCDATABASE_H__
//-----------------------------------------------------------------------------
#include <QString>
#include <QVector>
#include <QSqlError>
#include "VizTypes.h"
#include "VizCCompany.h"
#include "VizCSecurity.h"
#include "VizCSecurityPeriod.h"
#include "VizUser.h"
//-----------------------------------------------------------------------------
namespace viz
{
//-----------------------------------------------------------------------------
namespace server
{
//-----------------------------------------------------------------------------
class VizCDatabase
{
public:
	VizCDatabase();
	~VizCDatabase();

	bool open();
	void close();

	bool insertCompany(const core::VizCCompany& company, s32& insertedCompanyId, bool fill = false);
	bool removeCompany(s32 companyId);
	bool updateCompany(s32 companyId, const core::VizCCompany& company);
	bool getAllCompanyIds(QVector<s32>& companyIds);
	bool getCompanyIdsByPartialSecuritySymbol(const QString& partialSecuritySymbol, QVector<s32>& companyIds);
	bool getCompanyIdsByCNPJ(const QString& cnpj, QVector<s32>& companyIds);
	bool getCompany(s32 companyId, core::VizCCompany& company, bool filled = false, VizESortOrder securityPeriodsOrder = viz::VESO_UNSORTED);	

	bool insertSecurity(s32 companyId, const core::VizCSecurity& security, s32& insertedSecurityId, bool fill = false);
	bool removeSecurity(s32 securityId);
	bool updateSecurity(s32 securityId, s32 companyId, const core::VizCSecurity& security);
	bool getAllSecurityIds(QVector<s32>& securityIds);
	bool getSecurityIdsByCompanyId(s32 companyId, QVector<s32>& securityIds);
	bool getSecurityIdsBySymbol(const QString& symbol, QVector<s32>& securityIds);
	bool getSecurity(s32 securityId, core::VizCSecurity& security, s32& companyId, bool filled = false, VizESortOrder securityPeriodsOrder = viz::VESO_UNSORTED);
	
 	bool updateSecurityNegotiableStatus(const QDate& negotiableEnd);
	bool getSecurityNegotiableList(QVector<QStringList>& negotiableList, const QDate& clientLastDate = QDate());
	bool getSecurityRemovedList(QVector<QString>& removedList, const QDate& clientLastDate);

	bool insertSecurityPeriodDaily(s32 securityId, const core::VizCSecurityPeriod& securityPeriod, s32& insertedSecurityPeriodDailyId);
	bool insertSecurityPeriodsDaily(s32 securityId, const QVector<core::VizCSecurityPeriod>& securityPeriods, QVector<s32>& insertedSecurityPeriodDailyIds);
	bool removeSecurityPeriodDaily(s32 securityPeriodDailyId);
	bool updateSecurityPeriodsDaily(s32 securityPeriodDailyId, s32 securityId, const core::VizCSecurityPeriod& securityPeriod);
	bool getSecurityPeriodsDailyBySecurityIdFromDate(s32 securityId, QVector<core::VizCSecurityPeriod>& securityPeriods, const QDate& date, VizESortOrder = viz::VESO_UNSORTED);
	bool getSecurityPeriodsDailyBySecurityId(s32 securityId, QVector<core::VizCSecurityPeriod>& securityPeriods, VizESortOrder = viz::VESO_UNSORTED);
	bool getAllSecurityPeriodDailyIds(QVector<s32>& securityPeriodDailyIds, VizESortOrder securityPeriodsOrder = viz::VESO_UNSORTED);
	bool getSecurityPeriodsDaily(s32 securityPeriodDailyId, core::VizCSecurityPeriod& securityPeriod);

	bool getUser(const QString& username, core::VizSUser& user);

	bool getSecurityPeriodDailyDateRange(s32 securityId, QDate& startDate, QDate& endDate);

	QSqlError lastError() const;

	bool getNearestForwardDateSplit(s32 companyId, const QDate& approvalsDate, QDate& correctedDate);
	bool insertSplit(s32 companyId, s32& insertedSplitId, const core::VizCSplit& split);
	bool updateSplit(s32 splitId, const core::VizCSplit& split);
	bool removeSplit(s32 insertedSplitId);
	bool getSplits(s32 companyId, QVector<core::VizCSplit>& splits);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
