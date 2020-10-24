//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCDatabaseUtils.h"

#include <QTime>
#include <iostream>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCDatabaseUtils* VizCDatabaseUtils::Instance = 0;
//-----------------------------------------------------------------------------
VizCDatabaseUtils* VizCDatabaseUtils::instance()
{	
	if(!Instance)
	{
		Instance = new VizCDatabaseUtils;
	} 
	return Instance;
}
//-----------------------------------------------------------------------------
VizCDatabaseUtils::VizCDatabaseUtils() 
{
	Database = new VizCDatabase;
	Database->open();
}
//-----------------------------------------------------------------------------
VizCDatabaseUtils::~VizCDatabaseUtils()
{
	Database->close();
} 
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::getNegotiableList(QVector<QStringList>& negotiableList, const QDate& clientLastDate)
{
	QMutexLocker locker(&mutexGetNegotiableElements);
	// FULL negotiablelist
	vizServerLog() << "Requisição de lista de ativos...";
	vizStartLocalTimer();
	if (clientLastDate.isNull())
	{
		if (!Database->getSecurityNegotiableList(negotiableList))
			return false;
	}
	else
	{
		if (!Database->getSecurityNegotiableList(negotiableList, clientLastDate))
			return false;
	}
	vizLogLocalTimerElapsed();
	vizServerLog() << "Finalizada.";

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::getRemovedNegotiableElements(QVector<QString>& removedList, const QDate& clientLastDate)
{
	QMutexLocker locker(&mutexGetRemovedElements);
	// Elementos removidos
 	if (clientLastDate.isNull())
		return false;

	if (!Database->getSecurityRemovedList(removedList, clientLastDate))
		return false;
	

	return true;
}
// bool VizCDatabaseUtils::getNegotiableList(QVector<QPair<QString, QString> >& negotiableList, const QDate& clientLastDate)
// {
// 	QMutexLocker locker(&mutexGetNegotiableElements);
// 	// FULL negotiablelist
// 	if (clientLastDate.isNull())
// 	{
// 		if (!Database->getSecurityNegotiableList(negotiableList))
// 			return false;
// 	}
// 	else
// 	{
// 		if (!Database->getSecurityNegotiableList(negotiableList, clientLastDate))
// 			return false;
// 	}
// 
// 	return true;
// }
// 
// bool VizCDatabaseUtils::getRemovedNegotiableElements(QVector<QPair<QString, QString> >& removedList, const QDate& clientLastDate)
// {
// 	QMutexLocker locker(&mutexGetRemovedElements);
// 	// Elementos removidos
//  	if (clientLastDate.isNull())
// 		return false;
// 
// 	if (!Database->getSecurityRemovedList(removedList, clientLastDate))
// 		return false;
// 	
// 
// 	return true;
// }
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::getSecurity(const QString securityId, core::VizCCompany& company, const QDate& clientLastDate)
{
	QMutexLocker locker(&mutexGetSecurity);

	QVector<s32> dbSecurityIds;				
	s32 id(-1);

	if (!Database->getSecurityIdsBySymbol(securityId, dbSecurityIds))
		return false;

	if (dbSecurityIds.size() == 0)
	{
		return true;
	} 
	else if (dbSecurityIds.size() == 1)
	{
		core::VizCSecurity dbSecurity;
		s32 dbCompanyId;

		// FULL security
		if (clientLastDate.isNull())
		{
			if (!Database->getSecurity(dbSecurityIds.at(0), dbSecurity,  dbCompanyId, true, viz::VESO_ASCENDING_ORDER))
				return false;
		} 
		else
		{
			if (!Database->getSecurity(dbSecurityIds.at(0), dbSecurity,  dbCompanyId))
				return false;		

			if (!Database->getSecurityPeriodsDailyBySecurityIdFromDate(dbSecurityIds.at(0), dbSecurity.SecurityPeriods, clientLastDate))
				return false;
		}


		if (!Database->getCompany(dbCompanyId, company))
			return false;

		company.Securities.push_back(dbSecurity);	
	} 
	else 
		return false;			

	return true;

}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::removeLastNegotiableRegisters(const QDate& oldDate)
{
	return (Database->updateSecurityNegotiableStatus(oldDate));
}
//-----------------------------------------------------------------------------
// bool VizCDatabaseUtils::getFullSecurity(const QString securityId, core::VizCCompany& company)
// {
// 	QMutexLocker locker(&mutexGetSecurity);
// 
// 	QVector<s32> dbSecurityIds;				
// 	s32 id(-1);
// 
// 	if (!Database->getSecurityIdsBySymbol(securityId, dbSecurityIds))
// 		return false;
// 
// 	if (dbSecurityIds.size() == 0)
// 	{
// 		return true;
// 	} 
// 	else if (dbSecurityIds.size() == 1)
// 	{
// 		core::VizCSecurity dbSecurity;
// 		s32 dbCompanyId;
// 		if (!Database->getSecurity(dbSecurityIds.at(0), dbSecurity,  dbCompanyId, true, viz::VESO_ASCENDING_ORDER))
// 			return false;
// 
// 		if (!Database->getCompany(dbCompanyId, company))
// 			return false;
// 
// 		company.Securities.push_back(dbSecurity);	
// 	} 
// 	else 
// 		return false;			
// 
// 	return true;
// }
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::isAuthenticationPassed(QString login, QString md5Password)
{
	QMutexLocker locker(&mutexAuthentication);

	core::VizSUser user;

	if (login.isEmpty() || md5Password.isEmpty())
	{
		return false;
	}
	
	if(!Database->getUser(login, user))
	{
		return false;
	}

	if(user.UserPassword != md5Password)
	{
		return false;
	}

	if(!user.AccountTypeActived)
	{
		return false;
	}

	//colocar codigo para testar contas do tipo TRIAL

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertCompanyCCVM(const QString& cnpj, const u32& ccvm)
{
	QVector<s32> companyIds;

	Database->getCompanyIdsByCNPJ(cnpj, companyIds);

	foreach (const s32& companyId, companyIds)
	{
		core::VizCCompany company;
		
		if (!Database->getCompany(companyId, company))
			return false;

		company.CCVM = ccvm;

		if (!Database->updateCompany(companyId, company))
			return false;	
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertSinglePeriod(const s32& securityId, const core::VizCSecurityPeriod& period, const bool& isTerm)
{
	QVector<core::VizCSecurityPeriod> periodsInDatabase;	
	 
	if (!Database->getSecurityPeriodsDailyBySecurityId(securityId, periodsInDatabase))
		return false;
	
	s32 id;
	if (periodsInDatabase.size() == 0)
	{		
		if (!Database->insertSecurityPeriodDaily(securityId, period, id))
			return false;
	}
	else 
	{
		bool isAvailable = true;
		int countPeriods = 0;		
		while ((periodsInDatabase.size() > countPeriods))
		{
			if (period.ExchangeDate.daysTo(periodsInDatabase.at(countPeriods).ExchangeDate) == 0)
			{
				isAvailable = false;				
			}
			countPeriods++;
		}
		

		if (isAvailable)
		{
			if (!Database->insertSecurityPeriodDaily(securityId, period, id))
				return false;
		}
		else if (isTerm)
		{
			bool insertionNeeded = false;
			for (int i = 0; i < periodsInDatabase.size(); i++)
			{
				if (period.ExchangeDate.daysTo(periodsInDatabase.at(i).ExchangeDate) == 0)
				{
					while (i < periodsInDatabase.size())
					{
						if ((period.ExchangeDate.daysTo(periodsInDatabase.at(i).ExchangeDate) == 0)
							&& (period.ForwardMarketTermDays != (periodsInDatabase[i].ForwardMarketTermDays)))
							insertionNeeded = true;
						i++;

					}					
				}

			}

			if (insertionNeeded)
			{
				if (!Database->insertSecurityPeriodDaily(securityId, period, id))
					return false;
			}			
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertCheckingFullSecurity(const s32& companyId, core::VizCSecurity& security)
{
	QVector<s32> dbSecurityIds;
	s32 id(-1);

	if (!Database->getSecurityIdsBySymbol(security.Symbol, dbSecurityIds))
		return false;

	if (dbSecurityIds.size() == 0)
	{
		if (!Database->insertSecurity(companyId, security, id, true))
			return false;
	} 
	else if (dbSecurityIds.size() == 1)
	{
		core::VizCSecurity securityInDatabase;

		if (!Database->getSecurity(dbSecurityIds.at(0), securityInDatabase, id))
			return false;

		security.SecurityId = securityInDatabase.SecurityId;

		if (securityInDatabase.crc() != security.crc())
		{
			// Fazer o merge pro update
			security.mergeAttributes(securityInDatabase);
			if (!Database->updateSecurity(security.SecurityId, companyId, security))
				return false;
		}

		// Para o conversor do histo apenas
		if (security.SecurityPeriods.size() == 1)
		{		
			// Mercado a termo
			bool ok = false;
			if (security.SecurityPeriods.at(0).ForwardMarketTermDays.trimmed().toInt(&ok) > 0)
			{
				if (ok)
					insertSinglePeriod(security.SecurityId, security.SecurityPeriods.at(0), true);
			}
			else insertSinglePeriod(security.SecurityId, security.SecurityPeriods.at(0));			
		}
	} 
	else 
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertCheckingSplit(QString cnpj, core::VizCSplit& split)
{
	QVector<core::VizCSecurityPeriod> periods;

	QVector<s32> companyIds;

	if (!Database->getCompanyIdsByCNPJ(cnpj, companyIds))
		return false;

	QDate correctDate;

	if (!Database->getNearestForwardDateSplit(companyIds.at(0), split.Date, correctDate))
	{
		return false;
	}	

	QVector<core::VizCSplit> splits;

	if (!Database->getSplits(companyIds.at(0),	splits))
	{
		return false;		
	}

	if (split.Date.daysTo(correctDate) > 200)
	{
		//qDebug() << "Aprovacao " << split.Date << " correcao " << correctDate << " cnpj " << cnpj;
		std::cout << '!';
	}
	else
	{
		s32 splitId(-1);

		if (correctDate.isValid())
		{
			QVector<core::VizCSplit> splitsInDb;

			if (!Database->getSplits(companyIds.at(0), splitsInDb))
				return false;

			// Tem que estar ordenado já que o approval date não aparece na base
			// e não há como comparar
			bool contains(false);
			core::VizCSplit dbSplit;

			foreach (const core::VizCSplit s, splitsInDb)
			{
				if (s.Date == correctDate)
				{					
					contains = true;
					dbSplit = s;
					break;
				}
			}

			split.Date = correctDate;

			if (!contains)
			{			
				if (!Database->insertSplit(companyIds.at(0), splitId, split))
				{
					return false;
				}
				std::cout << '.';
			}
			else
			{
				if (!Database->updateSplit(dbSplit.SplitId, split))
				{
					return false;
				}

				std::cout << '.';
			}

			
		}
		else 
		{
			std::cout << '!';			
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertCheckingFullCompany(core::VizCCompany& company)
{
	QVector<s32> dbCompanyIds;
	s32 id(-1);

	if (!Database->getCompanyIdsByPartialSecuritySymbol(company.PartialSecuritySymbol, dbCompanyIds))
		return false;

	if (dbCompanyIds.size() == 0)
	{		
		if (!Database->insertCompany(company, id, true))
			return false;
	}
	else if (dbCompanyIds.size() == 1)
	{
		core::VizCCompany companyInDatabase;

		if (!Database->getCompany(dbCompanyIds.at(0), companyInDatabase))
			return false;

		company.CompanyId = companyInDatabase.CompanyId;

		if (companyInDatabase.crc() != company.crc())
		{
			company.mergeAttributes(companyInDatabase);
			if (!Database->updateCompany(company.CompanyId, company))
				return false;
		}

		for (int i = 0; i < company.Securities.size(); i++)
		{
			insertCheckingFullSecurity(company.CompanyId, company.Securities[i]);			
		}		
	} else 
		return false;
	
	return true;

}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertCheckingFullStockRegister(core::VizCCompany& company)
{
	return insertCheckingFullCompany(company);	
}
//-----------------------------------------------------------------------------
bool VizCDatabaseUtils::insertNoCheckFullStockRegister(QList<core::VizCCompany>& companies)
{
	for (int companyCount = 0; companyCount < companies.size(); companyCount++)
	{
		QTime timePerCompany;
		timePerCompany.start();
		QVector<s32> dbCompanyIds;
		s32 id(-1);

		if (!Database->getCompanyIdsByPartialSecuritySymbol(companies.at(companyCount).PartialSecuritySymbol, dbCompanyIds))
			return false;

		if (dbCompanyIds.size() == 0)
		{		
			if (!Database->insertCompany(companies.at(companyCount), id, true))
				return false;
		}
		else if (dbCompanyIds.size() == 1)
		{
			for (int i = 0; i < companies.at(companyCount).Securities.size(); i++)
			{
				QTime timePerSecurity;
				timePerSecurity.start();
				QVector<s32> dbSecurityIds;				
				s32 id(-1);

				if (!Database->getSecurityIdsBySymbol(companies.at(companyCount).Securities.at(i).Symbol, dbSecurityIds))
					return false;

				if (dbSecurityIds.size() == 0)
				{
					if (!Database->insertSecurity(dbCompanyIds.at(0), companies.at(companyCount).Securities.at(i), id, true))
						return false;
				} 
				else if (dbSecurityIds.size() == 1)
				{
					QVector<s32> dbPeriodIds;
					if (!Database->insertSecurityPeriodsDaily(dbSecurityIds.at(0), 
						companies.at(companyCount).Securities.at(i).SecurityPeriods, dbPeriodIds))
						return false;
				} 
				else 
					return false;		
				qDebug() << "Tempo por securityperiod: " << timePerSecurity.elapsed() / companies.at(companyCount).Securities.at(i).SecurityPeriods.size();
			}		
		} else 
			return false;	
		qDebug() << "Tempo por companhia: " << timePerCompany.elapsed();
	}

	return true;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
