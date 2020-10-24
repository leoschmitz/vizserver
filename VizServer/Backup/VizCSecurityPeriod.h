//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCSECURITYPERIODS_H__
#define __VIZCSECURITYPERIODS_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QByteArray>
#include <QDateTime>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCSecurityPeriod
{
public:
	VizCSecurityPeriod();
	VizCSecurityPeriod(const QString& especiEvent, const f64 openPrice, const f64& closePrice, const f64& maxPrice, const f64& averagePrice, const f64& minPrice, const viz::u64& quantityTrades, const f64& totalVolume, const VizEContractCorrection& indopc, const f64& strikePrice, const QDate& maturityDate, const f64& currentValue, const f64& bestSellOffer, const f64& bestBuyOffer, const f64& variation, const s32& quotationFactor, const f64& strikePricePoints, const QString& forwardMarketTermDays, const QString& currency, const s32& dismes, const QDateTime& openTimeStamp, const QDateTime& closeTimeStamp);
	VizCSecurityPeriod(const VizCSecurityPeriod& other);

	void set(s32 securityPeriodId, const QString& especiEvent, const f64& openPrice, const f64& closePrice, const f64& maxPrice, const f64& minPrice, const f64& averagePrice, const viz::u64& quantityTrades, const f64& totalVolume, const VizEContractCorrection& indopc, const f64& strikePrice, const QDate& maturityDate, const f64& currentValue, const f64& bestSellOffer, const f64& bestBuyOffer, const f64& variation, const s32& quotationFactor, const f64& strikePricePoints, const QString& forwardMarketTermDays, const QString& currency, const s32& dismes, const QDateTime& openTimeStamp, const QDateTime& closeTimeStamp);
	void get(s32& securityPeriodId, QString& especiEvent, f64& openPrice, f64& closePrice, f64& maxPrice, f64& minPrice, f64& averagePrice, viz::u64& quantityTrades, f64& totalVolume, VizEContractCorrection& indopc, f64& strikePrice, QDate& maturityDate, f64& currentValue, f64& bestSellOffer, f64& bestBuyOffer, f64& variation, s32& quotationFactor, f64& strikePricePoints, QString& forwardMarketTermDays, QString& currency, s32& dismes, QDateTime& openTimeStamp, QDateTime& closeTimeStamp) const;
	void set(const VizCSecurityPeriod& other);
	void get(VizCSecurityPeriod& other) const;

	bool isValid() const;
	u16 crc() const;
	VizCSecurityPeriod toLogarithmic() const;

	bool operator!=(const VizCSecurityPeriod& other) const;
	bool operator==(const VizCSecurityPeriod& other) const;
	bool operator<(const VizCSecurityPeriod& other);
	VizCSecurityPeriod& operator=(const VizCSecurityPeriod& other);

	s32 SecurityPeriodId;
	QString EspeciEvent;
	f64 OpenPrice;
	f64 ClosePrice;
	f64 MaxPrice;
	f64 MinPrice;
	f64 AveragePrice;
	viz::u64 QuantityTrades;
	f64 TotalVolume;
	VizEContractCorrection INDOPC;
	f64 StrikePrice;
	QDate MaturityDate;
	f64 CurrentValue;
	f64 BestSellOffer;
	f64 BestBuyOffer;
	f64 Variation;
	s32 QuotationFactor;
	f64 StrikePricePoints;
	QString ForwardMarketTermDays;
	QString Currency;
	s32 DISMES;
	QDateTime OpenTimeStamp;
	QDateTime CloseTimeStamp;
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizCSecurityPeriod& securityPeriod)
{
	in
		>> securityPeriod.SecurityPeriodId
		>> securityPeriod.EspeciEvent
		>> securityPeriod.OpenPrice
		>> securityPeriod.ClosePrice
		>> securityPeriod.MaxPrice
		>> securityPeriod.MinPrice
		>> securityPeriod.AveragePrice
		>> securityPeriod.QuantityTrades
		>> securityPeriod.TotalVolume
		>> securityPeriod.INDOPC
		>> securityPeriod.StrikePrice
		>> securityPeriod.MaturityDate
		>> securityPeriod.CurrentValue
		>> securityPeriod.BestSellOffer
		>> securityPeriod.BestBuyOffer
		>> securityPeriod.Variation
		>> securityPeriod.QuotationFactor
		>> securityPeriod.StrikePricePoints
		>> securityPeriod.ForwardMarketTermDays
		>> securityPeriod.Currency
		>> securityPeriod.DISMES
		>> securityPeriod.OpenTimeStamp
		>> securityPeriod.CloseTimeStamp;

	return in;
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizCSecurityPeriod& securityPeriod)
{
	out
		<< securityPeriod.SecurityPeriodId
		<< securityPeriod.EspeciEvent
		<< securityPeriod.OpenPrice
		<< securityPeriod.ClosePrice
		<< securityPeriod.MaxPrice
		<< securityPeriod.MinPrice
		<< securityPeriod.AveragePrice
		<< securityPeriod.QuantityTrades
		<< securityPeriod.TotalVolume
		<< securityPeriod.INDOPC
		<< securityPeriod.StrikePrice
		<< securityPeriod.MaturityDate
		<< securityPeriod.CurrentValue
		<< securityPeriod.BestSellOffer
		<< securityPeriod.BestBuyOffer
		<< securityPeriod.Variation
		<< securityPeriod.QuotationFactor
		<< securityPeriod.StrikePricePoints
		<< securityPeriod.ForwardMarketTermDays
		<< securityPeriod.Currency
		<< securityPeriod.DISMES
		<< securityPeriod.OpenTimeStamp
		<< securityPeriod.CloseTimeStamp;

	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
