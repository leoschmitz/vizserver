//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCDatabase.h"
#include "VizCLog.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCDatabase::VizCDatabase()
{
}
//-----------------------------------------------------------------------------
VizCDatabase::~VizCDatabase()
{
}

//-----------------------------------------------------------------------------
bool VizCDatabase::open()
{
	vizServerLog() << "Abrindo Banco de Dados...";

	//QT_DEBUG_COMPONENT
	//qDebug() << QApplication::libraryPaths();

	//aqui tem que trocar o user quando for conectar para usuario final
	//para garantir mais 1 nivel de segurança tem o vizuser que so pode fazer SELECT na base viztrader
	//user vizadmin pode tudo na base viztrader

	vizStartLocalTimer();
	Q_ASSERT(QSqlDatabase::isDriverAvailable("QMYSQL"));
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	Q_ASSERT(db.driver()->hasFeature(QSqlDriver::LastInsertId));
	db.setHostName("localhost");
	//db.setHostName(".");
	db.setDatabaseName("viztrader");
	db.setUserName("vizadmin");
	db.setPassword("20_4dM1n_Uz4r09");
	bool ok = db.open();
	vizLogLocalTimerElapsed();

	if (ok)
		vizServerLog() << "Banco de Dados aberto!";
	else vizServerLog() << "ERRO: Banco de dados não abriu.";

	return ok;

}
//-----------------------------------------------------------------------------
void VizCDatabase::close()
{
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
}
//-----------------------------------------------------------------------------
QSqlError VizCDatabase::lastError() const
{
	QSqlQuery query;
	return query.lastError();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::insertCompany(const core::VizCCompany& company, s32& insertedCompanyId, bool fill)
{
	QSqlQuery query;
	query.prepare("INSERT INTO VizTrader.vizcompany(CompanyId, Name, WholeName, PartialSecuritySymbol, CNPJ, IssuerCreation, CurrentStatus, InfoUpdated, CCVM) VALUES (NULL, :Name, :WholeName, :PartialSecuritySymbol, :CNPJ, :IssuerCreation, :CurrentStatus, :InfoUpdated, :CCVM)");
	query.bindValue(":Name", company.Name);
	query.bindValue(":WholeName", company.WholeName);
	query.bindValue(":PartialSecuritySymbol", company.PartialSecuritySymbol);
	query.bindValue(":CNPJ", company.CNPJ);
	query.bindValue(":IssuerCreation", company.IssuerCreation);
	query.bindValue(":CurrentStatus", company.CurrentStatus);
	query.bindValue(":InfoUpdated", company.InfoUpdated);
	query.bindValue(":CCVM", company.CCVM);

	bool ok = query.exec();

	if(ok)
	{
		QVariant v = query.lastInsertId();
		Q_ASSERT(v.isValid());
		insertedCompanyId = v.toInt();

		if(fill)
		{
			foreach(core::VizCSecurity s, company.Securities)
			{
				s32 sid;
				ok = insertSecurity(insertedCompanyId, s, sid, true);
				if(!ok) break;
			}
		}
	}

	return ok;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::removeCompany(s32 companyId)
{
	QSqlQuery query;
	query.prepare("DELETE FROM VizTrader.vizcompany WHERE CompanyId = :CompanyId");
	query.bindValue(":CompanyId", companyId);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::updateCompany(s32 companyId, const core::VizCCompany& company)
{
	QSqlQuery query;
	query.prepare("UPDATE VizTrader.vizcompany SET Name = :Name, WholeName = :WholeName, PartialSecuritySymbol = :PartialSecuritySymbol, CNPJ = :CNPJ, IssuerCreation = :IssuerCreation, CurrentStatus = :CurrentStatus, InfoUpdated = :InfoUpdated, CCVM = :CCVM WHERE vizcompany.CompanyId = :CompanyId");
	query.bindValue(":CompanyId", companyId);
	query.bindValue(":Name", company.Name);
	query.bindValue(":WholeName", company.WholeName);
	query.bindValue(":PartialSecuritySymbol", company.PartialSecuritySymbol);
	query.bindValue(":CNPJ", company.CNPJ);
	query.bindValue(":IssuerCreation", company.IssuerCreation);
	query.bindValue(":CurrentStatus", company.CurrentStatus);
	query.bindValue(":InfoUpdated", company.InfoUpdated);
	query.bindValue(":CCVM", company.CCVM);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getAllCompanyIds(QVector<s32>& companyIds)
{

	QSqlQuery query;
	query.prepare("SELECT CompanyId FROM VizTrader.vizcompany");

	if(!query.exec())
	{
		return false;
	}

	companyIds.clear();

	while(query.next())
	{
		s32 CompanyId = query.value(0).toInt();
		companyIds.append(CompanyId);
	}

	return true;

}
//-----------------------------------------------------------------------------
bool VizCDatabase::getCompany(s32 companyId, core::VizCCompany& company, bool filled, VizESortOrder order)
{
	QSqlQuery query;
	query.prepare("SELECT CompanyId, Name, WholeName, PartialSecuritySymbol, CNPJ, IssuerCreation, CurrentStatus, InfoUpdated, CCVM FROM VizTrader.vizcompany WHERE CompanyId = :CompanyId LIMIT 1");
	query.bindValue(":CompanyId", companyId);

	if(!query.exec())
	{
		return false;
	}

	if(query.size() == 0)
		return false;

	while(query.next())
	{
		company.CompanyId = query.value(0).toInt();
		company.Name = query.value(1).toString();
		company.WholeName = query.value(2).toString();
		company.PartialSecuritySymbol = query.value(3).toString();
		company.CNPJ = query.value(4).toString();
		company.IssuerCreation = query.value(5).toDate();
		company.CurrentStatus = query.value(6).toString();
		company.InfoUpdated = query.value(7).toDate();
		company.CCVM = query.value(8).toUInt();
	}

	bool ok = true;

	if(filled)
	{
		QVector<s32> ids;
		company.Securities.clear();
		ok = getSecurityIdsByCompanyId(company.CompanyId, ids);
		if(!ok) return ok;
		foreach(s32 i, ids)
		{
			core::VizCSecurity security;
			ok = getSecurity(i, security, company.CompanyId, true, order);
			if(!ok) break;
			company.Securities.append(security);
		}
	}

	if (!getSplits(companyId, company.Splits))
	{
		return false;
	}

	return ok;

}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityNegotiableList(QVector<QStringList >& negotiableList, const QDate& clientLastDate)
{
	negotiableList.clear();

	QSqlQuery query;
	if (clientLastDate.isNull())
	{
		query.prepare("SELECT Symbol, WholeName, SecurityId FROM viztrader.vizcompany, VizTrader.vizsecurity WHERE vizsecurity.negotiable = true AND vizcompany.CompanyId = vizsecurity.CompanyId");
	}
	else
	{
		query.prepare("SELECT Symbol, WholeName, SecurityId FROM viztrader.vizcompany, VizTrader.vizsecurity WHERE vizsecurity.negotiable = true AND vizcompany.CompanyId = vizsecurity.CompanyId AND vizsecurity.negotiableStart > :Date");
		query.bindValue(":Date", clientLastDate);
	}	

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		QStringList listOfElements;
		QDate start;
		QDate end;

		getSecurityPeriodDailyDateRange(query.value(2).toInt(), start, end);

		listOfElements << query.value(0).toString();
		listOfElements << query.value(1).toString();
		listOfElements << QObject::tr("Indraday");  
		listOfElements << start.toString(Qt::ISODate);
		listOfElements << end.toString(Qt::ISODate);
		//listOfElements << query.value(2).toDate().toString(Qt::ISODate);
		//listOfElements << query.value(3).toDate().toString(Qt::ISODate);

		negotiableList.push_back(listOfElements);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityRemovedList(QVector<QString>& removedList, const QDate& clientLastDate)
{
	removedList.clear();

	QSqlQuery query;
	
	
	query.prepare("SELECT Symbol FROM VizTrader.vizsecurity WHERE vizsecurity.negotiableEnd > :Date AND vizsecurity.negotiableEnd >= vizsecurity.NegotiableStart");
	query.bindValue(":Date", clientLastDate);
		

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		//QPair<QString, QString> listElement;

		//listElement.second = query.value(0).toString();
		//listElement.first  = query.value(1).toString();		

		//removedList.push_back(listElement);
		removedList.append(query.value(0).toString());
	}

	return true;
}
// bool VizCDatabase::getSecurityNegotiableList(QVector<QPair<QString, QString> >& negotiableList, const QDate& clientLastDate)
// {
// 	negotiableList.clear();
// 
// 	QSqlQuery query;
// 	if (clientLastDate.isNull())
// 	{
// 		query.prepare("SELECT WholeName, Symbol FROM viztrader.vizcompany, VizTrader.vizsecurity WHERE vizsecurity.negotiable = true AND vizcompany.CompanyId = vizsecurity.CompanyId");
// 	}
// 	else
// 	{
// 		query.prepare("SELECT WholeName, Symbol FROM viztrader.vizcompany, VizTrader.vizsecurity WHERE vizsecurity.negotiable = true AND vizcompany.CompanyId = vizsecurity.CompanyId AND vizsecurity.negotiableStart > :Date");
// 		query.bindValue(":Date", clientLastDate);
// 	}	
// 
// 	if(!query.exec())
// 	{
// 		return false;
// 	}
// 
// 	while(query.next())
// 	{
// 		QPair<QString, QString> listElement;
// 
// 		listElement.second = query.value(0).toString();
// 		listElement.first  = query.value(1).toString();		
// 
// 		negotiableList.push_back(listElement);
// 	}
// 
// 	return true;
// }
// 
// bool VizCDatabase::getSecurityRemovedList(QVector<QPair<QString, QString> >& removedList, const QDate& clientLastDate)
// {
// 	removedList.clear();
// 
// 	QSqlQuery query;
// 	
// 	
// 	query.prepare("SELECT WholeName, Symbol FROM viztrader.vizcompany, VizTrader.vizsecurity WHERE vizsecurity.negotiable = true AND vizcompany.CompanyId = vizsecurity.CompanyId AND vizsecurity.negotiableEnd > :Date AND vizsecurity.negotiableEnd >= vizsecurity.NegotiableStart");
// 	query.bindValue(":Date", clientLastDate);
// 		
// 
// 	if(!query.exec())
// 	{
// 		return false;
// 	}
// 
// 	while(query.next())
// 	{
// 		QPair<QString, QString> listElement;
// 
// 		listElement.second = query.value(0).toString();
// 		listElement.first  = query.value(1).toString();		
// 
// 		removedList.push_back(listElement);
// 	}
// 
// 	return true;
// }
//-----------------------------------------------------------------------------
bool VizCDatabase::getCompanyIdsByCNPJ(const QString& cnpj, QVector<s32>& companyIds)
{
	companyIds.clear();

	QSqlQuery query;
	query.prepare("SELECT CompanyId FROM VizTrader.vizcompany WHERE CNPJ = :cnpj");
	query.bindValue(":cnpj", cnpj);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		s32 CompanyId = query.value(0).toInt();
		companyIds.append(CompanyId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getCompanyIdsByPartialSecuritySymbol(const QString& partialSecuritySymbol, QVector<s32>& companyIds)
{
	companyIds.clear();

	QSqlQuery query;
	query.prepare("SELECT CompanyId FROM VizTrader.vizcompany WHERE PartialSecuritySymbol = :PartialSecuritySymbol");
	query.bindValue(":PartialSecuritySymbol", partialSecuritySymbol);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		s32 CompanyId = query.value(0).toInt();
		companyIds.append(CompanyId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::insertSplit(s32 companyId, s32& insertedSplitId, const core::VizCSplit& split)
{
	QSqlQuery query;
	query.prepare(
		"INSERT INTO VizTrader.vizsplit(\
		VizSplitId, CompanyId, Date, ValueAfter, ValueBefore, QuantityAfter, QuantityBefore\
		)\
		\
		VALUES (\
		NULL, :CompanyId, :Date, :ValueAfter, :ValueBefore, :QuantityAfter, :QuantityBefore\
		)");

	query.bindValue(":CompanyId", companyId);
	query.bindValue(":Date", split.Date);
	query.bindValue(":ValueAfter", split.ValueAfter);
	query.bindValue(":ValueBefore", split.ValueBefore);
	query.bindValue(":QuantityAfter", split.QuantityAfter);
	query.bindValue(":QuantityBefore", split.QuantityBefore);

	bool ok = query.exec();

	if(ok)
	{
		QVariant v = query.lastInsertId();
		Q_ASSERT(v.isValid());
		insertedSplitId = v.toInt();
	}

	return ok;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::removeSplit(s32 insertedSplitId)
{
	QSqlQuery query;
	query.prepare("DELETE FROM VizTrader.vizsplit WHERE VizSplitId = :VizSplitId");
	query.bindValue(":VizSplitId", insertedSplitId);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getNearestForwardDateSplit(s32 companyId, const QDate& approvalsDate, QDate& correctedDate)
{
	correctedDate = QDate();
	QSqlQuery query;

	query.prepare("SELECT ExchangeDate from VizSecurity, VizSecurityPeriodDaily, VizCompany \
				WHERE VizSecurity.SecurityId = VizSecurityPeriodDaily.SecurityId \
				AND VizSecurity.CompanyId = VizCompany.CompanyId \
				AND (EspeciEvent REGEXP \'.*E[A-Z]{0,3}G.*\' \
				  or EspeciEvent REGEXP \'.*E[A-Z]{0,3}B.*\') \
				  AND VizCompany.CompanyId = :companyId \
				  AND ExchangeDate > :ApprovalsDate \
				  ORDER BY ExchangeDate \
				  LIMIT 2");

	query.bindValue(":companyId", companyId);
	query.bindValue(":ApprovalsDate", approvalsDate);

	if (!query.exec())
		return false;

	if (query.next())
	{
		correctedDate = query.value(0).toDate();
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::updateSplit(s32 splitId, const core::VizCSplit& split)
{
	QSqlQuery query;

	query.prepare("UPDATE Viztrader.VizSplit SET \
				  CompanyId = :CompanyId, Date = :Date, ValueAfter = :ValueAfter, ValueBefore = :ValueBefore, \
				  QuantityAfter = :QuantityAfter, QuantityBefore = :QuantityBefore\
				  WHERE VizSplitId = :splitId");

	query.bindValue(":splitId", splitId);
	query.bindValue(":CompanyId", split.SplitId);
	query.bindValue(":Date", split.Date);
	query.bindValue(":ValueAfter", split.ValueAfter);
	query.bindValue(":ValueBefore", split.ValueBefore);
	query.bindValue(":QuantityAfter", split.QuantityAfter);
	query.bindValue(":QuantityBefore", split.QuantityBefore);	

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSplits(s32 companyId, QVector<core::VizCSplit>& splits)
{

	splits.clear();

	QSqlQuery query;

	query.prepare("SELECT Date, ValueAfter, ValueBefore, QuantityAfter, QuantityBefore FROM VizTrader.vizsplit WHERE CompanyId = :CompanyId");
	query.bindValue(":CompanyId", companyId);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		core::VizCSplit split;

		split.Date = query.value(0).toDate();
		split.ValueAfter = query.value(1).toDouble();
		split.ValueBefore = query.value(2).toDouble();
		split.QuantityAfter = query.value(3).toLongLong();
		split.QuantityBefore = query.value(4).toLongLong();		

		splits.append(split);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::insertSecurity(s32 companyId, const core::VizCSecurity& security, s32& insertedSecurityId, bool fill)
{

	QSqlQuery query;
	query.prepare(
		"INSERT INTO VizTrader.vizsecurity(\
		SecurityId, CompanyId, ISIN, ISINObject, Symbol, Bdi, BdiDescription,\
		DistributionNumber, MarketType, MarketDescription, SerialNumber,\
		SecurityType, DueDate, Price, OptionStyle, CurrecyType, CurrecyTypeDescription, Protection, Negotiable, NegotiableStart, NegotiableEnd, InfoUpdated\
		)\
		\
		VALUES (\
		NULL, :CompanyId, :ISIN, :ISINObject, :Symbol, :Bdi, :BdiDescription,\
		:DistributionNumber, :MarketType, :MarketDescription, :SerialNumber,\
		:SecurityType, :DueDate, :Price, :OptionStyle, :CurrecyType, :CurrecyTypeDescription, :Protection, :Negotiable, :NegotiableStart, :NegotiableEnd, :InfoUpdated\
		)");
		
	query.bindValue(":CompanyId", companyId);
	query.bindValue(":ISIN", security.ISIN.isEmpty() ? QVariant(QVariant::String) : security.ISIN);
	query.bindValue(":ISINObject", security.ISINObject.isEmpty() ? QVariant(QVariant::String) : security.ISINObject);
	query.bindValue(":Symbol", security.Symbol);
	query.bindValue(":Bdi", static_cast<s32>(security.Bdi));
	query.bindValue(":BdiDescription", security.BdiDescription.isEmpty() ? QVariant(QVariant::String) : security.BdiDescription);
	query.bindValue(":DistributionNumber,", security.DistributionNumber);
	query.bindValue(":MarketType", static_cast<s32>(security.MarketType));
	query.bindValue(":MarketDescription", security.MarketDescription.isEmpty() ? QVariant(QVariant::String) : security.MarketDescription);
	query.bindValue(":SerialNumber", security.SerialNumber);
	query.bindValue(":SecurityType", security.SecurityType.isEmpty() ? QVariant(QVariant::String) : security.SecurityType);
	query.bindValue(":DueDate", security.DueDate);
	query.bindValue(":Price", security.Price);
	query.bindValue(":OptionStyle", security.OptionStyle.isEmpty() ? QVariant(QVariant::String) : security.OptionStyle);
	query.bindValue(":CurrecyType", security.CurrecyType);
	query.bindValue(":CurrecyTypeDescription", security.CurrecyTypeDescription.isEmpty() ? QVariant(QVariant::String) : security.CurrecyTypeDescription);
	query.bindValue(":Protection", security.CurrecyTypeDescription.isEmpty() ? QVariant(QVariant::String) : security.Protection);
	query.bindValue(":Negotiable", security.Negotiable);
	query.bindValue(":NegotiableStart", security.NegotiableStart);
	query.bindValue(":NegotiableEnd", security.NegotiableEnd);
	query.bindValue(":InfoUpdated", security.InfoUpdated);

	bool ok = query.exec();

	if(ok)
	{
		QVariant v = query.lastInsertId();
		Q_ASSERT(v.isValid());
		insertedSecurityId = v.toInt();

		if(fill)
		{
			foreach(core::VizCSecurityPeriod sp, security.SecurityPeriods)
			{
				s32 spid;
				ok = insertSecurityPeriodDaily(insertedSecurityId, sp, spid);
				if(!ok) 
					break;
			}
		}

// 		foreach(QDate key, security.Splits.keys())
// 		{
// 			const QPair<u64, u64>& value = security.Splits[key];
// 			s32 sid;
// 			ok = insertSplit(insertedSecurityId, sid, key, value);
// 			if(!ok) 
// 				break;
// 		}

	}

	return ok;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::updateSecurity(s32 securityId, s32 companyId, const core::VizCSecurity& security)
{
	QSqlQuery query;
	query.prepare(
		"UPDATE VizTrader.vizsecurity SET\
		CompanyId = :CompanyId, ISIN = :ISIN, ISINObject = :ISINObject, Symbol = :Symbol, Bdi = :Bdi, BdiDescription = :BdiDescription,\
		DistributionNumber = :DistributionNumber, MarketType = :MarketType, MarketDescription = :MarketDescription, SerialNumber = :SerialNumber,\
		SecurityType = :SecurityType, DueDate = :DueDate, Price = :Price, OptionStyle = :OptionStyle, CurrecyType = :CurrecyType, CurrecyTypeDescription = :CurrecyTypeDescription,\
		Protection = :Protection, Negotiable = :Negotiable, NegotiableStart = :NegotiableStart, NegotiableEnd = :NegotiableEnd, InfoUpdated = :InfoUpdated\
		WHERE vizsecurity.SecurityId = :SecurityId");

	
	query.bindValue(":SecurityId", securityId);
	query.bindValue(":CompanyId", companyId);
	query.bindValue(":ISIN", security.ISIN.isEmpty() ? QVariant(QVariant::String) : security.ISIN);
	query.bindValue(":ISINObject", security.ISINObject.isEmpty() ? QVariant(QVariant::String) : security.ISINObject);
	query.bindValue(":Symbol", security.Symbol);
	query.bindValue(":Bdi", static_cast<s32>(security.Bdi));
	query.bindValue(":BdiDescription", security.BdiDescription.isEmpty() ? QVariant(QVariant::String) : security.BdiDescription);
	query.bindValue(":DistributionNumber", security.DistributionNumber);
	query.bindValue(":MarketType", static_cast<s32>(security.MarketType));
	query.bindValue(":MarketDescription", security.MarketDescription.isEmpty() ? QVariant(QVariant::String) : security.MarketDescription);
	query.bindValue(":SerialNumber", security.SerialNumber);
	query.bindValue(":SecurityType", security.SecurityType.isEmpty() ? QVariant(QVariant::String) : security.SecurityType);
	query.bindValue(":DueDate", security.DueDate);
	query.bindValue(":Price", security.Price);
	query.bindValue(":OptionStyle", security.OptionStyle.isEmpty() ? QVariant(QVariant::String) : security.OptionStyle);
	query.bindValue(":CurrecyType", security.CurrecyType);
	query.bindValue(":CurrecyTypeDescription", security.CurrecyTypeDescription.isEmpty() ? QVariant(QVariant::String) : security.CurrecyTypeDescription);
	query.bindValue(":Protection", security.CurrecyTypeDescription.isEmpty() ? QVariant(QVariant::String) : security.Protection);
	query.bindValue(":Negotiable", security.Negotiable);
	query.bindValue(":NegotiableStart", security.NegotiableStart);
	query.bindValue(":NegotiableEnd", security.NegotiableEnd);
	query.bindValue(":InfoUpdated", security.InfoUpdated);

/*
	bool ok = query.exec();
	qDebug() << query.isValid();
	qDebug() << query.lastError().text() << query.lastError().type();
	return ok;
*/	

	return query.exec();

}
//-----------------------------------------------------------------------------
bool VizCDatabase::removeSecurity(s32 securityId)
{
	QSqlQuery query;
	query.prepare("DELETE FROM VizTrader.vizsecurity WHERE SecurityId = :SecurityId");
	query.bindValue(":SecurityId", securityId);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getAllSecurityIds(QVector<s32>& securityIds)
{

	QSqlQuery query;
	query.prepare("SELECT SecurityId FROM VizTrader.vizsecurity");

	if(!query.exec())
	{
		return false;
	}

	securityIds.clear();

	while(query.next())
	{
		s32 securityId = query.value(0).toInt();
		securityIds.append(securityId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityIdsBySymbol(const QString& symbol, QVector<s32>& securityIds)
{
	securityIds.clear();

	QSqlQuery query;
	query.prepare("SELECT SecurityId FROM VizTrader.vizsecurity WHERE Symbol = :Symbol");
	query.bindValue(":Symbol", symbol);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		s32 securityId = query.value(0).toInt();
		securityIds.append(securityId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityIdsByCompanyId(s32 companyId, QVector<s32>& securityIds)
{
	securityIds.clear();

	QSqlQuery query;
	query.prepare("SELECT SecurityId FROM VizTrader.vizsecurity WHERE CompanyId = :CompanyId");
	query.bindValue(":CompanyId", companyId);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		s32 securityId = query.value(0).toInt();
		securityIds.append(securityId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurity(s32 securityId, core::VizCSecurity& security, s32& companyId, bool filled, VizESortOrder order)
{
	QSqlQuery query;
	query.prepare("SELECT SecurityId, CompanyId, ISIN, ISINObject, Symbol, Bdi, BdiDescription, DistributionNumber, MarketType, MarketDescription, SerialNumber, SecurityType, DueDate, Price, OptionStyle, CurrecyType, CurrecyTypeDescription, Protection, Negotiable, NegotiableStart, NegotiableEnd, InfoUpdated FROM VizTrader.vizsecurity WHERE SecurityId = :SecurityId");
	query.bindValue(":SecurityId", securityId);

	if(!query.exec())
	{
		//qDebug() << query.lastError().text() << query.lastError().type();
		return false;
	}

	bool canConvert;

	while(query.next())
	{
		security.SecurityId = query.value(0).toInt();
		companyId = query.value(1).toInt();
		security.ISIN = query.value(2).toString();
		security.ISINObject = query.value(3).toString();
		security.Symbol = query.value(4).toString();
		security.Bdi = static_cast<VizESecurityBDI>(query.value(5).toInt(&canConvert));
		if(!canConvert) security.Bdi = viz::VESB_INVALID;
		security.BdiDescription = query.value(6).toString();
		security.DistributionNumber = query.value(7).toInt(&canConvert);
		if(!canConvert) security.DistributionNumber = -1;
		security.MarketType = static_cast<VizEMarketType>(query.value(8).toInt(&canConvert));
		if(!canConvert) security.MarketType = viz::VEMT_INVALID;
		security.MarketDescription = query.value(9).toString();
		security.SerialNumber = query.value(10).toInt(&canConvert);
		if(!canConvert) security.SerialNumber = -1;
		security.SecurityType = query.value(11).toString();
		security.DueDate = query.value(12).toDate();
		security.Price = query.value(13).toDouble();
		security.OptionStyle = query.value(14).toString();
		security.CurrecyType = query.value(15).toInt(&canConvert);
		if(!canConvert) security.CurrecyType = -1;
		security.CurrecyTypeDescription = query.value(16).toString();
		security.Protection = query.value(17).toString();
		security.Negotiable = query.value(18).toBool();
		security.NegotiableStart = query.value(19).toDate();
		security.NegotiableEnd = query.value(20).toDate();
		security.InfoUpdated = query.value(21).toDate();
	}

	bool ok = true;

	if(filled)
	{
		security.SecurityPeriods.clear();
		ok = getSecurityPeriodsDailyBySecurityId(security.SecurityId, security.SecurityPeriods, order);
	}

	//ok = getSplits(companyId, company.Splits);

	return ok;

}
//-----------------------------------------------------------------------------
bool VizCDatabase::updateSecurityNegotiableStatus(const QDate& negotiableEnd)
{
	QSqlQuery query;
	query.prepare(
		"UPDATE VizTrader.vizsecurity SET\
		NegotiableEnd = :NegotiableEnd\
		WHERE vizsecurity.Negotiable = :Negotiable");

	query.bindValue(":Negotiable", true);
	query.bindValue(":NegotiableEnd", negotiableEnd);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::insertSecurityPeriodDaily(s32 securityId, const core::VizCSecurityPeriod& securityPeriod, s32& insertedSecurityPeriodDailyId)
{
	QSqlQuery query;
	query.prepare("INSERT INTO VizTrader.vizsecurityperioddaily (SecurityPeriodDailyId, SecurityId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate) VALUES (NULL, :SecurityId, :EspeciEvent, :OpenPrice, :ClosePrice, :MaxPrice, :MinPrice, :AveragePrice, :QuantityTrades, :TotalVolume, :INDOPC, :StrikePrice, :MaturityDate, :CurrentValue, :BestSellOffer, :BestBuyOffer, :QuotationFactor, :StrikePricePoints, :ForwardMarketTermDays, :Currency, :DISMES, :ExchangeDate)");

	query.bindValue(":SecurityId", securityId);
	query.bindValue(":EspeciEvent", securityPeriod.EspeciEvent);
	query.bindValue(":OpenPrice", securityPeriod.OpenPrice);
	query.bindValue(":ClosePrice", securityPeriod.ClosePrice);
	query.bindValue(":MaxPrice",securityPeriod.MaxPrice);
	query.bindValue(":MinPrice", securityPeriod.MinPrice);
	query.bindValue(":AveragePrice", securityPeriod.AveragePrice);
	query.bindValue(":QuantityTrades", securityPeriod.QuantityTrades);
	query.bindValue(":TotalVolume", securityPeriod.TotalVolume);
	query.bindValue(":INDOPC", static_cast<s32>(securityPeriod.INDOPC));
	query.bindValue(":StrikePrice", securityPeriod.StrikePrice);
	query.bindValue(":MaturityDate", securityPeriod.MaturityDate);
	query.bindValue(":CurrentValue", securityPeriod.CurrentValue);
	query.bindValue(":BestSellOffer", securityPeriod.BestSellOffer);
	query.bindValue(":BestBuyOffer", securityPeriod.BestBuyOffer);
	query.bindValue(":QuotationFactor", securityPeriod.QuotationFactor);
	query.bindValue(":StrikePricePoints", securityPeriod.StrikePricePoints);
	query.bindValue(":ForwardMarketTermDays", securityPeriod.ForwardMarketTermDays);
	query.bindValue(":Currency", securityPeriod.Currency);
	query.bindValue(":DISMES", securityPeriod.DISMES);
	query.bindValue(":ExchangeDate", securityPeriod.ExchangeDate);

	bool ok = query.exec();

	if(ok)
	{
		QVariant v = query.lastInsertId();
		Q_ASSERT(v.isValid());
		insertedSecurityPeriodDailyId = v.toInt();
	}

	return ok;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::insertSecurityPeriodsDaily(s32 securityId, const QVector<core::VizCSecurityPeriod>& securityPeriods, QVector<s32>& insertedSecurityPeriodDailyIds)
{

	bool ok = true;
	insertedSecurityPeriodDailyIds.clear();
	
	foreach(const core::VizCSecurityPeriod& sp, securityPeriods)
	{
		s32 id = -1;
		ok = insertSecurityPeriodDaily(securityId, sp, id);
		if(!ok) break;
		insertedSecurityPeriodDailyIds.append(id);
	}

	return ok;

}
//-----------------------------------------------------------------------------
bool VizCDatabase::removeSecurityPeriodDaily(s32 securityPeriodDailyId)
{
	QSqlQuery query;
	query.prepare("DELETE FROM VizTrader.vizsecurityperioddaily WHERE SecurityPeriodDailyId = :SecurityPeriodDailyId");
	query.bindValue(":SecurityPeriodDailyId", securityPeriodDailyId);

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::updateSecurityPeriodsDaily(s32 securityPeriodDailyId, s32 securityId, const core::VizCSecurityPeriod& securityPeriod)
{

	QSqlQuery query;

	query.prepare("UPDATE viztrader.vizsecurityperioddaily SET \
	SecurityId = :SecurityId,\
	EspeciEvent = :EspeciEvent ,\
	OpenPrice = :OpenPrice ,\
	ClosePrice = :ClosePrice ,\
	MaxPrice = :MaxPrice ,\
	MinPrice = :MinPrice ,\
	AveragePrice = :AveragePrice ,\
	QuantityTrades = :QuantityTrades ,\
	TotalVolume = :TotalVolume ,\
	INDOPC = :INDOPC ,\
	StrikePrice = :StrikePrice ,\
	MaturityDate = :MaturityDate ,\
	CurrentValue = :CurrentValue ,\
	BestSellOffer = :BestSellOffer ,\
	BestBuyOffer = :BestBuyOffer ,\
	QuotationFactor = :QuotationFactor ,\
	StrikePricePoints = :StrikePricePoints ,\
	ForwardMarketTermDays = :ForwardMarketTermDays ,\
	Currency = :Currency ,\
	DISMES = :DISMES ,\
	ExchangeDate = :ExchangeDate \
	WHERE vizsecurityperioddaily.SecurityPeriodDailyId = :SecurityPeriodDailyId");

	query.bindValue(":SecurityPeriodDailyId", securityPeriodDailyId);
	query.bindValue(":SecurityId", securityId);
	query.bindValue(":EspeciEvent", securityPeriod.EspeciEvent);
	query.bindValue(":OpenPrice", securityPeriod.OpenPrice);
	query.bindValue(":ClosePrice", securityPeriod.ClosePrice);
	query.bindValue(":MaxPrice",securityPeriod.MaxPrice);
	query.bindValue(":MinPrice", securityPeriod.MinPrice);
	query.bindValue(":AveragePrice", securityPeriod.AveragePrice);
	query.bindValue(":QuantityTrades", securityPeriod.QuantityTrades);
	query.bindValue(":TotalVolume", securityPeriod.TotalVolume);
	query.bindValue(":INDOPC", static_cast<s32>(securityPeriod.INDOPC));
	query.bindValue(":StrikePrice", securityPeriod.StrikePrice);
	query.bindValue(":MaturityDate", securityPeriod.MaturityDate);
	query.bindValue(":CurrentValue", securityPeriod.CurrentValue);
	query.bindValue(":BestSellOffer", securityPeriod.BestSellOffer);
	query.bindValue(":BestBuyOffer", securityPeriod.BestBuyOffer);
	query.bindValue(":QuotationFactor", securityPeriod.QuotationFactor);
	query.bindValue(":StrikePricePoints", securityPeriod.StrikePricePoints);
	query.bindValue(":ForwardMarketTermDays", securityPeriod.ForwardMarketTermDays);
	query.bindValue(":Currency", securityPeriod.Currency);
	query.bindValue(":DISMES", securityPeriod.DISMES);
	query.bindValue(":ExchangeDate", securityPeriod.ExchangeDate);

	/*bool ok = query.exec();
	qDebug() << query.isValid();
	qDebug() << query.lastError().text() << query.lastError().type();
	return ok;
	*/

	return query.exec();
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityPeriodsDailyBySecurityIdFromDate(s32 securityId, QVector<core::VizCSecurityPeriod>& securityPeriods, const QDate& date, VizESortOrder order)
{
	QSqlQuery query;

	if(order == viz::VESO_ASCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId AND vizsecurityperioddaily.ExchangeDate > :Date ORDER BY vizsecurityperioddaily.ExchangeDate ASC");
	else if(order == viz::VESO_DESCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId AND vizsecurityperioddaily.ExchangeDate > :Date ORDER BY vizsecurityperioddaily.ExchangeDate DESC");
	else
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId AND vizsecurityperioddaily.ExchangeDate > :Date");

	query.bindValue(":SecurityId", securityId);
	query.bindValue(":Date", date);

	if(!query.exec())
	{
		/*qDebug() << query.isValid();
		qDebug() << query.lastError().text() << query.lastError().type();
		*/
		return false;
	}

	securityPeriods.clear();

	while(query.next())
	{
		core::VizCSecurityPeriod securityPeriod;

		securityPeriod.SecurityPeriodId = query.value(0).toInt();
		securityPeriod.EspeciEvent = query.value(1).toString();
		securityPeriod.OpenPrice = query.value(2).toDouble();
		securityPeriod.ClosePrice = query.value(3).toDouble();
		securityPeriod.MaxPrice = query.value(4).toDouble();
		securityPeriod.MinPrice = query.value(5).toDouble();
		securityPeriod.AveragePrice = query.value(6).toDouble();
		securityPeriod.QuantityTrades = query.value(7).toULongLong();
		securityPeriod.TotalVolume = query.value(8).toDouble();
		securityPeriod.INDOPC = static_cast<VizEContractCorrection>(query.value(9).toInt());
		securityPeriod.StrikePrice = query.value(10).toDouble();
		securityPeriod.MaturityDate = query.value(11).toDate();
		securityPeriod.CurrentValue = query.value(12).toDouble();
		securityPeriod.BestSellOffer = query.value(13).toDouble();
		securityPeriod.BestBuyOffer = query.value(14).toDouble();
		securityPeriod.QuotationFactor = query.value(15).toInt();
		securityPeriod.StrikePricePoints = query.value(16).toDouble();
		securityPeriod.ForwardMarketTermDays = query.value(17).toString();
		securityPeriod.Currency = query.value(18).toString();
		securityPeriod.DISMES = query.value(19).toInt();
		securityPeriod.ExchangeDate = query.value(20).toDate();

		securityPeriods.append(securityPeriod);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityPeriodsDailyBySecurityId(s32 securityId, QVector<core::VizCSecurityPeriod>& securityPeriods, VizESortOrder order)
{
	QSqlQuery query;

	if(order == viz::VESO_ASCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId ORDER BY vizsecurityperioddaily.ExchangeDate ASC");
	else if(order == viz::VESO_DESCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId ORDER BY vizsecurityperioddaily.ExchangeDate DESC");
	else
		query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :SecurityId");
	
	query.bindValue(":SecurityId", securityId);

	if(!query.exec())
	{
		/*qDebug() << query.isValid();
		qDebug() << query.lastError().text() << query.lastError().type();
		*/
		return false;
	}

	securityPeriods.clear();

	while(query.next())
	{
		core::VizCSecurityPeriod securityPeriod;

		securityPeriod.SecurityPeriodId = query.value(0).toInt();
		securityPeriod.EspeciEvent = query.value(1).toString();
		securityPeriod.OpenPrice = query.value(2).toDouble();
		securityPeriod.ClosePrice = query.value(3).toDouble();
		securityPeriod.MaxPrice = query.value(4).toDouble();
		securityPeriod.MinPrice = query.value(5).toDouble();
		securityPeriod.AveragePrice = query.value(6).toDouble();
		securityPeriod.QuantityTrades = query.value(7).toULongLong();
		securityPeriod.TotalVolume = query.value(8).toDouble();
		securityPeriod.INDOPC = static_cast<VizEContractCorrection>(query.value(9).toInt());
		securityPeriod.StrikePrice = query.value(10).toDouble();
		securityPeriod.MaturityDate = query.value(11).toDate();
		securityPeriod.CurrentValue = query.value(12).toDouble();
		securityPeriod.BestSellOffer = query.value(13).toDouble();
		securityPeriod.BestBuyOffer = query.value(14).toDouble();
		securityPeriod.QuotationFactor = query.value(15).toInt();
		securityPeriod.StrikePricePoints = query.value(16).toDouble();
		securityPeriod.ForwardMarketTermDays = query.value(17).toString();
		securityPeriod.Currency = query.value(18).toString();
		securityPeriod.DISMES = query.value(19).toInt();
		securityPeriod.ExchangeDate = query.value(20).toDate();		

		securityPeriods.append(securityPeriod);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getAllSecurityPeriodDailyIds(QVector<s32>& securityPeriodDailyIds, VizESortOrder order)
{
	QSqlQuery query;

	if(order == viz::VESO_ASCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId FROM VizTrader.vizsecurityperioddaily ORDER BY ExchangeDate ASC");
	else if(order == viz::VESO_DESCENDING_ORDER)
		query.prepare("SELECT SecurityPeriodDailyId FROM VizTrader.vizsecurityperioddaily ORDER BY ExchangeDate DESC");
	else
		query.prepare("SELECT SecurityPeriodDailyId FROM VizTrader.vizsecurityperioddaily");


	if(!query.exec())
	{
		return false;
	}

	securityPeriodDailyIds.clear();

	while(query.next())
	{
		s32 securityPeriodDailyId;
		securityPeriodDailyId = query.value(0).toInt();
		securityPeriodDailyIds.append(securityPeriodDailyId);
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityPeriodsDaily(s32 securityPeriodDailyId, core::VizCSecurityPeriod& securityPeriodDaily)
{
	QSqlQuery query;
	query.prepare("SELECT SecurityPeriodDailyId, EspeciEvent, OpenPrice, ClosePrice, MaxPrice, MinPrice, AveragePrice, QuantityTrades, TotalVolume, INDOPC, StrikePrice, MaturityDate, CurrentValue, BestSellOffer, BestBuyOffer, QuotationFactor, StrikePricePoints, ForwardMarketTermDays, Currency, DISMES, ExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityPeriodDailyId = :SecurityPeriodDailyId");
	query.bindValue(":SecurityPeriodDailyId", securityPeriodDailyId);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		securityPeriodDaily.SecurityPeriodId = query.value(0).toInt();
		securityPeriodDaily.EspeciEvent = query.value(1).toString();
		securityPeriodDaily.OpenPrice = query.value(2).toDouble();
		securityPeriodDaily.ClosePrice = query.value(3).toDouble();
		securityPeriodDaily.MaxPrice = query.value(4).toDouble();
		securityPeriodDaily.MinPrice = query.value(5).toDouble();
		securityPeriodDaily.AveragePrice = query.value(6).toDouble();
		securityPeriodDaily.QuantityTrades = query.value(7).toULongLong();
		securityPeriodDaily.TotalVolume = query.value(8).toDouble();
		securityPeriodDaily.INDOPC = static_cast<VizEContractCorrection>(query.value(9).toInt());
		securityPeriodDaily.StrikePrice = query.value(10).toDouble();
		securityPeriodDaily.MaturityDate = query.value(11).toDate();
		securityPeriodDaily.CurrentValue = query.value(12).toDouble();
		securityPeriodDaily.BestSellOffer = query.value(13).toDouble();
		securityPeriodDaily.BestBuyOffer = query.value(14).toDouble();
		securityPeriodDaily.QuotationFactor = query.value(15).toInt();
		securityPeriodDaily.StrikePricePoints = query.value(16).toDouble();
		securityPeriodDaily.ForwardMarketTermDays = query.value(17).toString();
		securityPeriodDaily.Currency = query.value(18).toString();
		securityPeriodDaily.DISMES = query.value(19).toInt();
		securityPeriodDaily.ExchangeDate = query.value(20).toDate();		
	}

	return true;
}
//-----------------------------------------------------------------------------
bool VizCDatabase::getUser(const QString& username, core::VizSUser& user)
{
	QSqlQuery query;
	query.prepare("SELECT VizUser.UserName, VizUser.UserPassword, VizUser.FullName, VizUser.Email, VizUser.CreationDate, VizUser.FirstLogin, VizUser.LastLogin, VizUser.ActiveAccount, VizAccountType.AccountTypeName, VizAccountType.AccountTypeDescription FROM VizTrader.VizUser, VizTrader.VizAccountType WHERE VizTrader.VizUser.AccountTypeId = VizTrader.VizAccountType.AccountTypeId AND VizUser.UserName = :UserName");
	query.bindValue(":UserName", username);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		user.UserName = query.value(0).toString();
		user.UserPassword = query.value(1).toString();
		user.FullName = query.value(2).toString();
		user.Email = query.value(3).toString();
		user.CreateDate = query.value(4).toDateTime();
		user.FirstLogin = query.value(5).toDateTime();
		user.LastLogin = query.value(6).toDateTime();
		user.AccountTypeActived = query.value(7).toBool();
		user.AccountType.AccountTypeName = query.value(8).toString();
		user.AccountType.AccountTypeDescription = query.value(9).toString();		
	}

	return true;

}
//-----------------------------------------------------------------------------
bool VizCDatabase::getSecurityPeriodDailyDateRange(s32 securityId, QDate& startDate, QDate& endDate)
{
	QSqlQuery query;

	query.prepare("SELECT MIN(ExchangeDate) as firstExchangeDate, MAX(ExchangeDate) as lastExchangeDate FROM VizTrader.vizsecurityperioddaily WHERE SecurityId = :securityId");
	query.bindValue(":securityId", securityId);

	if(!query.exec())
	{
		return false;
	}

	while(query.next())
	{
		startDate = query.value(0).toDate();
		endDate = query.value(1).toDate();		
	}

	return true;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------