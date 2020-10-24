//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCMEGABOLSAMSGPARSERH__
#define __VIZCMEGABOLSAMSGPARSERH__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "VizTypes.h"
#include "VizCRealTimePeriod.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
// Corretamente Tratado
struct VizSMegabolsaHeader
{
	QString MessageType;
	int QuotationGroup;
	QString StockCode;	
	QDateTime DateAndTime;	
};
//-----------------------------------------------------------------------------
// Corretamente Tratado
struct VizSOpeningTrade
{
	u64 QuantityTraded;
	QChar TradePriceFormat;
	f64 TradePrice;
	QString BuyerBrokerId;
	QString SellerBrokerId;
	u64 QuantityTradesAccumInDay;
	s32 IndicEndOfTradesAtOpenPrice;
	QChar IndicVarInRelationToPreviousPrice;
	u32 TradeNumber;
	QString MarketSegment;
	QDateTime TradeDateTime;
	QChar TradeOrigin;
	QChar VariationFormatIndicator;
	f64 Variation;
};
//-----------------------------------------------------------------------------
// Corretamente Tratado
struct VizSTrade
{
	u64 QuantityTraded;
	QChar TradePriceFormat;
	f64 TradePrice;
	QString BuyerBrokerId;
	QString SellerBrokerId;
	u64 QuantityTradesAccumInDay;
	QChar HighestPriceFormat;
	f64 HighestPrice;
	QChar LowestPriceFormat;
	f64 LowestPrice;
	s32 TypeOfRegistration;
	QChar OriginOfTrade;
	QChar IndicVarInRelationToPreviousPrice;
	u32 TradeNumber;
	QString MarketSegment;
	QString PeriodOfSettlementInDays;
	QDateTime TradeDateTime;
	QChar VariationFormatIndicator;
	f64 Variation;
};
//-----------------------------------------------------------------------------
struct VizSPriceUpdate
{
	QChar LastPriceFormat;
	u64 LastPrice;
	QChar HighestPriceFormat;
	u64 HighestPrice;
	QChar LowestPriceFormat;
	u64 LowestPrice;
	s32 PriceType;
	QChar IndicVarInRelationToPreviousPrice;
	QChar VariationFormatIndicator;
	u64 Variation;
};
//-----------------------------------------------------------------------------
struct VizSFiveBestPrices
{
	QString IndicBestOfferAltered;
	u64 TotalQuantityBids[6];
	s32 NumberOfBids[6];
	QChar BestBidPricesFormat[6];
	QString BestBidPrice[6];
	QChar BestAskPricesFormat[6];
	QString BestAskPrice[6];
	s32 NumberOfAsks[6];
	u64 TotalQuantityOfAsks[6];
	// Nosso controle 
	s32 occurences;
};
//-----------------------------------------------------------------------------
struct VizSChangeStockStatus
{
	QChar TradingStatus;
	QChar Status;
	QChar StockType;
	QTime OpeningTime;
};
//-----------------------------------------------------------------------------
struct VizSAltQuotGroupStatus
{
	QString QuotationGroupCode;
	QChar QuotationGroupCondition;
	s32 NumbOfCollectors;
};
//-----------------------------------------------------------------------------
struct VizSTheoreticalOpeningPrice
{
	QChar TheoreticalOpenPriceFormat;
	u64 TheoreticalOpenPrice;
	u64 TheoreticalQuantityTraded;
	QChar DirectionOfOffer;
	u64 QuantityNotTakenAtOpening;
	QChar VariationFormatIndicator;
	u64 Variation;
};
//-----------------------------------------------------------------------------
struct VizSSummaryOfOpening
{
	QChar FirstPriceFormat;
	u64 FirstPrice;
	QChar LastPriceFormat;
	u64 LastPrice;
	QChar BestPriceFormat;
	u64 BestPrice;
	QChar LowestPriceFormat;
	u64 LowestPrice;
	s32 TypeOfLastPrice;
	u64 AccumulatedAmount;
	QChar IndicVarInRelationToPreviousPrice;
	QChar VariationFormatIndicator;
	u64 Variation;
};
//-----------------------------------------------------------------------------
struct VizSSessionSchedule
{
	QString QuoutationGroupCode;
	s32 PreopeningTime;
	s32 OpeningTime;
	s32 ClosingTime;
};
//-----------------------------------------------------------------------------
struct VizSStockRegistry
{
	QString Name;
	QString Spec;
	u64 NumberOfShares;
	QString QuotationGroup;
	u64 StandardLot;
	s32 FormOfQuotation;
	u32 DateOfLastTrade;
	QChar PrevDayClosingPriceFormat;
	u64 PrevDayClosingPrice;
	QChar OptionExercisePriceFormat;
	u64 OptionExercisePrice;
	QString ISINCode;
	QString AverageQuantity;
	f32 FrozenCrossTradesLastTradePricePercent;
	f32 FrozenCrossTradesAveragePercent;
	f32 FrozenCrossTradesTotalPercent;
	f32 FrozenNonCrossLastTradePricePercent;
	f32 FrozenNonCrossTradesAveragePercent;
	f32 FrozenNonCrossTradesTotalPercent;
	QChar OptionIndication;
	u32 OptionExpirationDate;
	s32 OptionExpirationTime;
	QString TradingCodeOfOption;
	QString MarketSegment;
	QChar IPDDRV;
	u32 QuantityMultiplicationCoef;
};
//-----------------------------------------------------------------------------
// Essa estrutura foi criada pensando no futuro, pois a mensagem que a contem
//  possui campos reservados
struct VizSRetOrderBook
{
	QChar IndicatorBeginAndEnd;
};
//-----------------------------------------------------------------------------
struct VizSUpdateOrderBook
{
	QChar ActionType;
	QChar OfferDirection;
	QChar RegisteredOfferPriceFormat;
	u64 RegisteredOfferPrice;
	u64 QuantityShown;
	QChar TheoreticalOpeningPriceFormat;
	u64 TheoreticalOpeningPrice;
	QString OfferBrokerId;
	u32 OfferDate;
	s32 OfferSequentialNumber;
	QChar PriceType;
	u64 OrderTimeStamp;
};
//-----------------------------------------------------------------------------
struct VizSCancelOrderBook
{
	QChar CancellationType;
	QString OfferBrokerId;
	u32 OfferDate;
	s32 OfferSequentialNumber;
	QChar OfferDirection;
};
//-----------------------------------------------------------------------------
struct VizSCompositionOfIndex
{
	QString StockCode[30];
	QString StockSymbol[30];
	f32 Contribution[30];
	QChar IndLastMsg;
	s32 NumberOfStocks;
	QChar PrevDayIndexFormat;
	u32 PrevDayIndex;
	QChar IndFreqOfCalc;
};
//-----------------------------------------------------------------------------
struct VizSIndex 
{
	QChar IndexType;
	f32 LastIndex;
	f32 HighestIndex;
	s32 HighestIndexTime;
	f32 LowestIndex;
	s32 LowestIndexTime;
	s32 NumberTradedStocks;
	f32 PercentCapital;
	QChar IndDirectionVariation;
	f32 PercentVariation;
	QChar IndDirectionVariationPrevYear;
	f32 PercentVariationPrevYear;
	s32 NumberFalling;
	s32 NumberRising;
	s32 NumberNoVariation;
	s32 NumberNoQuotation;
	s32 NumberReserved;
	s32 NumberSuspended;
	s32 NumberTotal;
	s32 IndSupplementaryData;
};
//-----------------------------------------------------------------------------
struct VizSSecurityLoans
{
	QChar OperationType;
	s32 OfferType;
	QString OfferNumber;
	QString StockTradingCode;
	QString Company;
	u64 Quantity;
	u32 Fee;
	u32 MaxPeriod;
};
//-----------------------------------------------------------------------------
struct VizSNews
{
	s32 AgencyInfo;
	s32 Priority;
	s32 NewsItemNumber;
	s32 NewsSchedule;
	s32 NumberOfPages;
	s32 PageNumber;
	QString News;
};
//-----------------------------------------------------------------------------
struct VizSFixedIncomeHeader
{
	s32 BFSize;
	QChar Market;
	s32 Agent;
	QString Operator;
	s32 Date;
	s32 Time;
	s32 Sequence;
	s32 RegistCode;	
};
//-----------------------------------------------------------------------------
struct VizSFIUpdateSecurity
{
	QString SecurityCode;
	s32 MovementCode;
	QString Description;
	s32 HasCoupon;
	s32 FirstCoupon;
	s32 LastCoupon;
	QChar Currency;
	f32 ResidualValue;
	QChar IndicatorType;
	QString BriefDescription;
	QChar StockStatus;
	u32 DateSuspended;
	s32 TimeSuspended;
	s32 Divisibility;
	QString CurrentCoupon;
	QChar QualifiedOp[20];
	u32 DateValidityInterest;
	u32 DateEndInterest;
	f32 InterestRateToday;
	u32 DateValidityPrevInterest;
	u32 DateEndPrevInterest;
	f32 PrevInterestRate;
	QChar TradingAmbit[10];
	QChar NominalLoad;
	QChar RealDays;
	QString ISIN;
	f32 PercentOnPrice;
	QChar WhomAffected;
	f32 PercentSecurityPaid;
	QChar WhomPaiment;
	u32 StandardLot;
	u32 MaxNonDivisible;
	QChar IntDecIndicator;
	u32 DecimalPrice;
	QString FeeCode;
	QString IPCInd;
	QChar AdditionalFeeSignal;
	f32 AdditionalFee;
	u32 IssueDate;
	u32 ExpiratDate;
	u32 DateLastInterestPaid;
	s32 AnnualBase;
	u32 DivisorPrice;
	QString Formula;
	QString ConsecutiveDaysGoneBy;
	QChar IndSettlement;
	QChar TypeOfDays;
	QChar DirectionAuction;
	QChar ExpirInd;
	s32 NextExpiration;
	QString Settlers;
	QString Proprietor;
	QString BCBIndex;
};
//-----------------------------------------------------------------------------
struct VizSUpdateForSecurityVariable
{
	QChar MovementCode;
	QChar TradingCode;
	QString SecurityCode;
	QChar Currency;
	QString Variable;
	u32 SettlementDate;
	QString ProrprietorInfo;
};
//-----------------------------------------------------------------------------
struct VizSUpdateForAgents
{
	QChar Market;
	s32 Agent;
	QString Operator;
	s32 MovementCode;
	QString Name;
	QString BusinessName;
	QString Address;
	QString Locality;
	s32 PostalCode;
	QString PersonResponsible;
	QString Telephone;
	QString Fax;
	QString TypeOfAgent;
	QChar TradingStatus;
	QString Proprietor;
};
//-----------------------------------------------------------------------------
struct VizSOfficialClosing
{
	QChar Market;
	u32 Date;
	QString InstrumentCode;
	QString SimpleDescription;
	QString Description;
	f64 ClosingPrice;
	QChar SettlementCondition;
	QString ClosingPriceDate;
	f64 BidPrice;
	f64 QuantityOfBid;
	QString DateOfBid;
	QString TimeOfBid;
	f64 AskPrice;
	QString QuantityAsk;
	QString DateAsk;
	QString TimeAsk;
	f64 PriceLastTransaction;
	f64 QuantityLastTransaction;
	QString DateLastTransaction;
	f64 PrevClosing;
	f64 OpeningPrice;
	f64 MinPrice;
	f64 AvgPrice;
	f64 MaxPrice;
	f64 AccumQuantity;
	f64 VolumeOper;
	s32 NumOper;
	QString Proprietor;
};
//-----------------------------------------------------------------------------
struct VizSFIUpdateEachOper
{
	QString InstrumentCode;
	u32 CodeOperOrig;
	s32 TypeOfOperOrig;
	s32 TypeOfOperDest;
	s32 Time;
	f64 TotalQuantity;
	f64 VolumeTraded;
	f64 AvgPrice;
	f64 PricePrevClose;
	f64 LastPrice;
	QChar FluctSignal;
	f32 FluctPrevClose;
	f64 Minimum;
	f64 Maximum;
	s32 Date;
	s32 Time2;
	s32 Sequence;
	QString InstrumentCode2;
	QString OrigAgentCode;
	QString DestAgentCode;
	f64 QuantityOfOper;
	f64 Price;
	f64 RefPrice;
	f32 TopBase;
	f32 Fluctuation;
	s32 TypeOper;
	f64 PriceYester;
	QString CodeRound;
	s32 CodeOperOrig2;
	f64 Interest;
	f32 ResidualValue;
	f64 OperationFee;
	QChar OrigOper;
	f64 Gain;
	f64 GainDollars;
	QString Acronym;
	QString BidNumber;
	QString AskNumber;
	QString ProprietorInfo;
};
//-----------------------------------------------------------------------------
struct VizSFIReferencePrice
{
	QChar TypeOfRecord;
	QString SecurityCode;
	QString CouponNumber;
	QChar Currency;
	f64 RefPrice;
	s32 TermOperation;
	s32 Date;
	s32 Hour;
	QChar TypeOfNegotiation;
};
//-----------------------------------------------------------------------------
struct VizSFITotalsUpdate
{
	QChar TypeIndicator;
	f64 MovementInReais;
	f64 MovementInDollars;
	QString Proprietor;
};
//-----------------------------------------------------------------------------
struct VizSFI5BestLimits
{
	QChar Market;
	QString PitCode;
	QString InstrumCode;
	QString Proprietor;
	QString InstrumCodeDesc;
	// Respectivos
	f64 BestSellPrice[5];
	s32 NumOffersSell[5];
	f64 BestSellPriceQuantity[5];
	// Respectivos  
	f64 BestBuyPrice[5];
	s32 NumOffersBuy[5];
	f64 BestBuyPriceQuantity[5];
};
//-----------------------------------------------------------------------------
struct VizSFIUpdateOffers
{
	s32 CodeOrigOper;
	QString CodeRound;
	QString CodeAttachRound;
	s32 OrderSeqNum;
	QString Argument;
	QString LinkArg;
	s32 ExpirTime;
	f64 ResidualQuantity;
	f64 NominalPrice;
	f32 ResidualValue;
	QString InstrumentCode;
	QChar InfoPrice;
	QChar OrderOffer;
	QString Proprietor;

};
//-----------------------------------------------------------------------------
class VizCMegabolsaMsgParser : public QObject
{	
	Q_OBJECT
private:
	bool isFieldNegative(const QChar& format);
	s32 formatField2DecimalPlaces(const QChar& format);
	f64 formatField2f64(const QChar& format, const QString& s);

	f64 string2f64(const QString& s, int integerDigits, int fractDigits);
	f32 string2f32(const QString& s, int integerDigits, int fractDigits);

public:
	void parseFunctionalHeader(const QString& head, VizSMegabolsaHeader& header);
	void parseOpeningTrade(const QString& trade, VizSOpeningTrade& openingTrade);
	void fromOpeningTradeToRealTimePeriod(const VizSOpeningTrade& openingTrade, core::VizCRealTimePeriod* rt);
	void parseTrade(const QString& trade, VizSTrade& tradeStruct);	
	void fromTradeToRealTimePeriod(const VizSTrade& trade, core::VizCRealTimePeriod* rt);
	void parsePriceUpdate(const QString& update, VizSPriceUpdate& priceUpdate);
	void parseFiveBestOfferPrices(const QString& fiveBestMsg, VizSFiveBestPrices& fiveBestPrices);
	void parseChangeOfStockStatus(const QString& change, VizSChangeStockStatus& changeOfStockStatus);
	void parseAlterationOfQuotationGroupStatus(const QString& status, VizSAltQuotGroupStatus& quotGroupStatus);
	void parseTheoreticalOpeningPrice(const QString& theoretical, VizSTheoreticalOpeningPrice& theoreticalOpenPrice);
	void parseSummaryOfOpening(const QString& summary, VizSSummaryOfOpening& openingSummary);
	void parseScheduleByQuotationGroup(const QString& sched, VizSSessionSchedule& sessionSchedule);
	void parseRegistryOfStock(const QString& registry, VizSStockRegistry& registryOfStock);
	void parseRetransmitTheOrderBook(const QString& ret, VizSRetOrderBook& retOrderBook);
	void parseUpdateOrderBook(const QString& update, VizSUpdateOrderBook& updateOrderBook);
	void parseCancellationInOrderBook(const QString& cancel, VizSCancelOrderBook& cancelOB);
	void parseCompositionOfIndex(const QString& index, VizSCompositionOfIndex& compOfIndex);
	void parseIndex(const QString& ind, VizSIndex& index);
	void parseSecurityLoans(const QString& loan, VizSSecurityLoans& secLoan);
	void parseNews(const QString& n, VizSNews& news);
	void parseFixedIncome(const QString& fix, VizSFixedIncomeHeader& head);
	void parseFIUpdateForSecurity(const QString& update, VizSFIUpdateSecurity& updateSecurity);
	void parseFIUpdateForSecurityVariable(const QString& update, VizSUpdateForSecurityVariable& updateSecurity);
	void parseFIUpdateForAgents(const QString& update, VizSUpdateForAgents& agents);
	void parseFIOfficialClosing(const QString& close, VizSOfficialClosing& closing);
	void parseFIUpdateForEachOperation(const QString& update, VizSFIUpdateEachOper updateOper);
	void parseFIReferencePrice(const QString& p, VizSFIReferencePrice& refPrice);
	void parseFITotalsUpdateEveryMinute(const QString& up, VizSFITotalsUpdate& totals);
	void parseFI5BestLimits(const QString& five, VizSFI5BestLimits& topFive);
	void parseFIUpdateOfOffers(const QString& up, VizSFIUpdateOffers& offer);

	VizCMegabolsaMsgParser();
	~VizCMegabolsaMsgParser();	

signals:
	void startOfTradingDay();
	void closingOfDay();
	void systemOnTheAirWarning();	
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif