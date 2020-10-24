//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMegabolsaMsgParser.h"

#include <cmath>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCMegabolsaMsgParser::VizCMegabolsaMsgParser()
{
	
}
//-----------------------------------------------------------------------------
VizCMegabolsaMsgParser::~VizCMegabolsaMsgParser()
{
}
//-----------------------------------------------------------------------------
bool VizCMegabolsaMsgParser::isFieldNegative(const QChar& format)
{
	return (format.isLetter());
}
//-----------------------------------------------------------------------------
s32 VizCMegabolsaMsgParser::formatField2DecimalPlaces(const QChar& format)
{
	if (format.isNumber())
	{
		return format.digitValue();
	}
	else if (format.isLetter())
	{
		return QChar(static_cast<int>(format.toAscii()) - 17).digitValue();
	}	
	
	return 0;
}
//-----------------------------------------------------------------------------
f64 VizCMegabolsaMsgParser::formatField2f64(const QChar& format, const QString& s)
{
	return (isFieldNegative(format) ? -1.0 : 1.0)
		* string2f64(s, s.size() - formatField2DecimalPlaces(format), formatField2DecimalPlaces(format));
}
//-----------------------------------------------------------------------------
f64 VizCMegabolsaMsgParser::string2f64(const QString& s, int integerDigits, int fractDigits)
{
	if ((integerDigits + fractDigits) > s.size())
		return 0.0;

	int iDigit = s.left(integerDigits).toInt();
	int fDigit = s.mid(integerDigits, fractDigits).toInt();

	if (fractDigits > 0)
		return f64(static_cast<f64>(iDigit) + (static_cast<f64>(fDigit) / static_cast<f64>(pow(10.0, fractDigits))));

	return f64(static_cast<f64>(iDigit));
}
//-----------------------------------------------------------------------------
f32 VizCMegabolsaMsgParser::string2f32(const QString& s, int integerDigits, int fractDigits)
{
	if ((integerDigits + fractDigits) > s.size())
		return 0.0f;

	int iDigit = s.left(integerDigits).toInt();
	int fDigit = s.mid(integerDigits, fractDigits).toInt();

	if (fractDigits > 0)
		return f32(static_cast<f32>(iDigit) + (static_cast<f32>(fDigit) / static_cast<f32>(pow(10.0f, fractDigits))));
	
	return f32(static_cast<f32>(iDigit));
}
// Nao checado -> sem exemplo no sinal
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIUpdateOfOffers(const QString& up, VizSFIUpdateOffers& offer)
{
	offer.CodeOrigOper = up.mid(0, 4).toInt(); 
	offer.CodeRound = up.mid(4, 4); 
	offer.CodeAttachRound = up.mid(8, 4); 
	offer.OrderSeqNum = up.mid(12, 5).toInt(); 
	offer.Argument = up.mid(17, 30); 
	offer.LinkArg = up.mid(47, 30); 
	offer.ExpirTime = up.mid(77, 6).toInt(); 
	offer.ResidualQuantity = string2f64(up.mid(83, 18), 14, 4); 
	offer.NominalPrice = string2f64(up.mid(101, 18), 12, 6); 
	offer.ResidualValue = string2f32(up.mid(119, 5), 3, 2); 
	offer.InstrumentCode = up.mid(124, 15); 
	offer.InfoPrice = up.mid(139, 1).at(0); 
	offer.OrderOffer = up.mid(140, 1).at(0); 
	offer.Proprietor = up.mid(141, 2); 
}
// Nao checado -> sem exemplo no sinal
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFITotalsUpdateEveryMinute(const QString& up, VizSFITotalsUpdate& totals)
{
	totals.TypeIndicator = up.mid(0, 1).at(0);
	totals.MovementInReais = string2f64(up.mid(1, 18), 12, 6);
	totals.MovementInDollars = string2f64(up.mid(19, 18), 12, 6);
	totals.Proprietor = up.mid(37, 2);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFI5BestLimits(const QString& five, VizSFI5BestLimits& topFive)
{
	topFive.Market = five.mid(0, 1).at(0);
	topFive.PitCode = five.mid(1, 4);
	topFive.InstrumCode = five.mid(5, 15);
	topFive.Proprietor = five.mid(20, 2);
	topFive.InstrumCodeDesc = five.mid(22, 30);

	for (int i = 0; i < 5; i++)
	{
		topFive.BestSellPrice[i] = string2f64(five.mid(53 + (i * 39), 18), 10, 8);
		topFive.NumOffersSell[i] = five.mid(71 + (i * 39), 3).toInt();
		topFive.BestSellPriceQuantity[i] = string2f64(five.mid(74 + (i * 39), 18), 14, 4);
	}

	for (int i = 0; i < 5; i++)
	{
		topFive.BestBuyPrice[i] = string2f64(five.mid(248 + (i * 39), 18), 10, 8);
		topFive.NumOffersBuy[i] = five.mid(266 + (i * 39), 3).toInt();
		topFive.BestBuyPriceQuantity[i] = string2f64(five.mid(269 + (i * 39), 18), 14, 4);
	}
}
// Nao checado -> sem exemplo no sinal
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIReferencePrice(const QString& p, VizSFIReferencePrice& refPrice)
{
	refPrice.TypeOfRecord = p.mid(0, 1).at(0);
	refPrice.SecurityCode = p.mid(1, 5);
	refPrice.CouponNumber = p.mid(6, 3);
	refPrice.Currency = p.mid(9, 1).at(0);
	refPrice.RefPrice = string2f64(p.mid(10, 18), 10, 8);
	refPrice.TermOperation = p.mid(28, 3).toInt();
	refPrice.Date = p.mid(31, 6).toInt();
	refPrice.Hour = p.mid(37, 6).toInt();
	refPrice.TypeOfNegotiation = p.mid(43, 1).at(0);	
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIUpdateForEachOperation(const QString& update, VizSFIUpdateEachOper updateOper)
{
	updateOper.InstrumentCode = update.mid(0, 15);
	updateOper.CodeOperOrig = update.mid(15, 4).toInt();
	updateOper.TypeOfOperOrig  = update.mid(19, 1).toInt();
	updateOper.TypeOfOperDest = update.mid(20, 1).toInt();
	updateOper.Time = update.mid(21, 6).toInt();
	updateOper.TotalQuantity = string2f64(update.mid(27, 18), 14, 4);
	updateOper.VolumeTraded = string2f64(update.mid(45, 18), 12, 6);
	updateOper.AvgPrice = string2f64(update.mid(63, 18), 10, 8);
	updateOper.PricePrevClose = string2f64(update.mid(81, 18), 10, 8);
	updateOper.LastPrice = string2f64(update.mid(99, 18), 10, 8);
	updateOper.FluctSignal = update.mid(117, 1).at(0);
	updateOper.FluctPrevClose = string2f32(update.mid(118, 6), 4, 2);
	updateOper.Minimum = string2f64(update.mid(124, 18), 10, 8);
	updateOper.Maximum = string2f64(update.mid(142, 18), 10, 8);
	updateOper.Date = update.mid(160, 6).toInt();
	updateOper.Time2 = update.mid(166, 6).toInt();
	updateOper.Sequence = update.mid(172, 5).toInt();
	updateOper.InstrumentCode2 = update.mid(177, 15);
	updateOper.OrigAgentCode = update.mid(192, 6);
	updateOper.DestAgentCode = update.mid(198, 6);
	updateOper.QuantityOfOper = string2f64(update.mid(204, 18), 14, 4);
	updateOper.Price = string2f64(update.mid(222, 18), 10, 8);
	updateOper.RefPrice = string2f64(update.mid(240, 18), 10, 8);
	updateOper.TopBase = string2f32(update.mid(258, 5), 3, 2);
	updateOper.Fluctuation = string2f32(update.mid(263, 5), 3, 2);
	updateOper.TypeOper = update.mid(268, 2).toInt();
	updateOper.PriceYester = string2f64(update.mid(270, 18), 10, 8);
	updateOper.CodeRound = update.mid(288, 4);
	updateOper.CodeOperOrig2 = update.mid(292, 4).toInt();
	updateOper.Interest = string2f64(update.mid(296, 18), 16, 2);
	updateOper.ResidualValue = string2f32(update.mid(314, 5), 3, 2);
	updateOper.OperationFee = string2f64(update.mid(319, 18), 10, 8);
	updateOper.OrigOper = update.mid(337, 1).at(0);
	updateOper.Gain = string2f64(update.mid(338, 18), 16, 2);
	updateOper.GainDollars = string2f64(update.mid(356, 18), 16, 2);
	updateOper.Acronym = update.mid(374, 30);
	updateOper.BidNumber = update.mid(404, 5);
	updateOper.AskNumber = update.mid(409, 5);
	updateOper.ProprietorInfo = update.mid(414, 2);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIOfficialClosing(const QString& close, VizSOfficialClosing& closing)
{
	closing.Market = close.mid(0, 1).at(0);
	closing.Date = close.mid(1, 8).toUInt();
	closing.InstrumentCode = close.mid(9, 15);
	closing.SimpleDescription = close.mid(24, 10);
	closing.Description = close.mid(34, 30);
	closing.ClosingPrice = string2f64(close.mid(64, 18), 10, 8);
	closing.SettlementCondition = close.mid(82, 1).at(0);
	closing.ClosingPriceDate = close.mid(83, 8);
	closing.BidPrice = string2f64(close.mid(91, 18), 10, 8);
	closing.QuantityOfBid = string2f64(close.mid(109, 18), 14, 4);
	closing.DateOfBid = close.mid(127, 8);
	closing.TimeOfBid = close.mid(135, 6);
	closing.AskPrice = string2f64(close.mid(141, 18), 10, 8);
	closing.QuantityAsk = string2f64(close.mid(159, 18), 14, 4);
	closing.DateAsk = close.mid(177, 8);
	closing.TimeAsk = close.mid(185, 6);
	closing.PriceLastTransaction = string2f64(close.mid(191, 18), 10, 8);
	closing.QuantityLastTransaction = string2f64(close.mid(209, 18), 14, 4);
	closing.DateLastTransaction = close.mid(227, 8);
	closing.PrevClosing = string2f64(close.mid(235, 18), 10, 8);
	closing.OpeningPrice = string2f64(close.mid(253, 18), 10, 8);
	closing.MinPrice = string2f64(close.mid(271, 18), 10, 8);
	closing.AvgPrice = string2f64(close.mid(289, 18), 10, 8);
	closing.MaxPrice = string2f64(close.mid(307, 18), 10, 8);
	closing.AccumQuantity = string2f64(close.mid(325, 18), 14, 4);
	closing.VolumeOper = string2f64(close.mid(343, 14), 12, 2);
	closing.NumOper = close.mid(357, 5).toInt();
	closing.Proprietor = close.mid(362, 2);	
}
// Nao checado -> sem exemplo no sinal
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIUpdateForAgents(const QString& update, VizSUpdateForAgents& agents)
{
	agents.Market = update.mid(0, 1).at(0);
	agents.Agent = update.mid(1, 3).toInt();
	agents.Operator = update.mid(4, 2);
	agents.MovementCode = update.mid(6, 1).toInt();
	agents.Name = update.mid(7, 10);
	agents.BusinessName = update.mid(17, 30);
	agents.Address = update.mid(47, 30);
	agents.Locality = update.mid(77, 15);
	agents.PostalCode = update.mid(92, 4).toInt();
	agents.PersonResponsible = update.mid(96, 30);
	agents.Telephone = update.mid(126, 20);
	agents.Fax = update.mid(146, 10);
	agents.TypeOfAgent = update.mid(156, 10);
	agents.TradingStatus = update.mid(166, 1).at(0);
	// Reserved 8
	agents.Proprietor = update.mid(175, 2);
}
// Nao checado -> sem exemplo no sinal
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIUpdateForSecurityVariable(const QString& update, VizSUpdateForSecurityVariable& updateSecurity)
{
	updateSecurity.MovementCode = update.mid(0, 1).at(0);
	updateSecurity.TradingCode = update.mid(1, 1).at(0);
	updateSecurity.SecurityCode = update.mid(2, 5);
	updateSecurity.Currency = update.mid(7, 1).at(0);
	updateSecurity.Variable = update.mid(8, 7);
	updateSecurity.SettlementDate = update.mid(15, 8).toUInt();
	updateSecurity.ProrprietorInfo = update.mid(23, 2);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFIUpdateForSecurity(const QString& update, VizSFIUpdateSecurity& updateSecurity)
{
	updateSecurity.SecurityCode = update.mid(0, 5);
	updateSecurity.MovementCode = update.mid(5, 1).toInt();
	updateSecurity.Description = update.mid(6, 30);
	updateSecurity.HasCoupon = update.mid(36, 1).toInt();
	updateSecurity.FirstCoupon = update.mid(37, 3).toInt();
	updateSecurity.LastCoupon = update.mid(40, 3).toInt();
	updateSecurity.Currency = update.mid(43, 1).at(0);
	updateSecurity.ResidualValue = string2f32(update.mid(44, 9), 3, 6);
	updateSecurity.IndicatorType = update.mid(53, 1).at(0);
	updateSecurity.BriefDescription = update.mid(54, 10);
	updateSecurity.StockStatus = update.mid(64, 1).at(0);
	updateSecurity.DateSuspended = update.mid(65, 8).toUInt();
	updateSecurity.TimeSuspended = update.mid(73, 6).toInt();
	updateSecurity.Divisibility = update.mid(79, 7).toInt();
	updateSecurity.CurrentCoupon = update.mid(86, 3);
	for (int i = 0; i < 20; i++)
		updateSecurity.QualifiedOp[i] = update.mid(i + 89, 1).at(0);
	updateSecurity.DateValidityInterest = update.mid(109, 8).toUInt();
	updateSecurity.DateEndInterest = update.mid(117, 8).toUInt();
	updateSecurity.InterestRateToday = string2f32(update.mid(125, 8), 2, 6);
	updateSecurity.DateValidityPrevInterest = update.mid(133, 8).toInt();
	updateSecurity.DateEndPrevInterest = update.mid(141, 8).toInt();
	updateSecurity.PrevInterestRate = string2f32(update.mid(149, 8), 2, 6);
	for (int i = 0; i < 10; i++)
		updateSecurity.TradingAmbit[i] = update.mid(i + 157, 1).at(0);
	updateSecurity.NominalLoad = update.mid(167, 1).at(0);
	updateSecurity.RealDays = update.mid(168, 1).at(0);
	updateSecurity.ISIN = update.mid(169, 12);
	updateSecurity.PercentOnPrice = string2f32(update.mid(181, 5), 3, 2);
	updateSecurity.WhomAffected = update.mid(186, 1).at(0);
	updateSecurity.PercentSecurityPaid = string2f32(update.mid(187, 5), 3, 2);
	updateSecurity.WhomPaiment = update.mid(192, 1).at(0);
	updateSecurity.StandardLot = update.mid(193, 8).toUInt();
	updateSecurity.MaxNonDivisible = update.mid(201, 8).toUInt();
	updateSecurity.IntDecIndicator = update.mid(209, 1).at(0);
	updateSecurity.DecimalPrice = update.mid(210, 8).toUInt();
	updateSecurity.FeeCode = update.mid(218, 3);
	updateSecurity.IPCInd = update.mid(221, 3);
	updateSecurity.AdditionalFeeSignal = update.mid(224, 1).at(0);
	updateSecurity.AdditionalFee = string2f32(update.mid(225, 10), 4, 6);
	updateSecurity.IssueDate = update.mid(235, 8).toUInt();
	updateSecurity.ExpiratDate = update.mid(243, 8).toUInt();
	updateSecurity.DateLastInterestPaid = update.mid(251, 8).toUInt();
	updateSecurity.AnnualBase = update.mid(259, 3).toInt();
	updateSecurity.DivisorPrice = update.mid(262, 8).toUInt();
	updateSecurity.Formula = update.mid(270, 2);
	updateSecurity.ConsecutiveDaysGoneBy = update.mid(272, 3);
	updateSecurity.IndSettlement = update.mid(275, 1).at(0);
	updateSecurity.TypeOfDays = update.mid(276, 1).at(0);
	updateSecurity.DirectionAuction = update.mid(277, 1).at(0);
	updateSecurity.ExpirInd = update.mid(278, 1).at(0);
	updateSecurity.NextExpiration = update.mid(279, 4).toInt();
	updateSecurity.Settlers = update.mid(283, 20);
	updateSecurity.Proprietor = update.mid(303, 2);
	updateSecurity.BCBIndex = update.mid(305, 7);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFixedIncome(const QString& fix, VizSFixedIncomeHeader& head)
{
	// STX
	head.BFSize = fix.mid(1, 4).toInt();
	head.Market = fix.mid(5, 1).at(0);
	head.Agent = fix.mid(6, 3).toInt();
	head.Operator = fix.mid(9, 2);
	head.Date = fix.mid(11, 6).toInt();
	head.Time = fix.mid(17, 6).toInt();
	head.Sequence = fix.mid(23, 5).toInt();
	head.RegistCode = fix.mid(28, 4).toInt();	

	switch (head.RegistCode)
	{
	case 2101:
		{
			VizSFIUpdateSecurity update;
			parseFIUpdateForSecurity(fix.mid(32), update);
		}
		break;
	case 2102:
		{
			VizSUpdateForSecurityVariable update;
			parseFIUpdateForSecurityVariable(fix.mid(32), update);
		}
		break;
	case 2201:
		{
			VizSUpdateForAgents agents;
			parseFIUpdateForAgents(fix.mid(32), agents);
		}
		break;
	case 8104:
		{
			VizSOfficialClosing closing;
			parseFIOfficialClosing(fix.mid(32), closing);
		}
		break;
	case  9110:
		{
			VizSFIUpdateEachOper updateOper;
			parseFIUpdateForEachOperation(fix.mid(32), updateOper);
		}
		break;
	case 9120:
		{
			VizSFIReferencePrice refPrice;
			parseFIReferencePrice(fix.mid(32), refPrice);
		}
		break;
	case 9130:
		{
			VizSFITotalsUpdate update;
			parseFITotalsUpdateEveryMinute(fix.mid(32), update);
		}
		break;
	case 9140:
		{
			VizSFIUpdateOffers offers;
			parseFIUpdateOfOffers(fix.mid(32), offers);
		}
		break;
	case 9141:
		{
			VizSFI5BestLimits topFive;
			parseFI5BestLimits(fix.mid(32), topFive);
		}
		break;	
	}
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseNews(const QString& n, VizSNews& news)
{
	news.AgencyInfo = n.mid(0, 3).toInt();
	news.Priority = n.mid(3, 1).toInt();
	news.NewsItemNumber = n.mid(4, 4).toInt();
	news.NewsSchedule = n.mid(8, 6).toInt();
	news.NumberOfPages = n.mid(14, 2).toInt();
	news.PageNumber= n.mid(16, 2).toInt();
	news.News = n.mid(18);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseSecurityLoans(const QString& loan, VizSSecurityLoans& secLoan)
{
	secLoan.OperationType = loan.mid(0, 1).at(0);
	secLoan.OfferType = loan.mid(1, 1).toInt();
	secLoan.OfferNumber = loan.mid(2, 9);
	secLoan.StockTradingCode = loan.mid(11, 12);
	secLoan.Company = loan.mid(23, 30);
	secLoan.Quantity = loan.mid(53, 18).toULongLong();
	secLoan.Fee = loan.mid(71, 8).toUInt();
	secLoan.MaxPeriod = loan.mid(79, 8).toUInt();	
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseIndex(const QString& ind, VizSIndex& index)
{
	index.IndexType = ind.mid(0, 1).at(0);

	int integerPart(-1);
	int fractionalPart(-1);

	integerPart = ind.mid(1, 5).toInt();
	fractionalPart = ind.mid(6, 2).toInt();
	index.LastIndex = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f);
	integerPart = ind.mid(8, 5).toInt();
	fractionalPart = ind.mid(13, 2).toInt();
	index.HighestIndex = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f); 
	index.HighestIndexTime = ind.mid(15, 6).toInt();
	integerPart = ind.mid(21, 5).toInt();
	fractionalPart = ind.mid(26, 2).toInt();
	index.LowestIndex = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f);
	index.LowestIndexTime = ind.mid(28, 6).toInt();
	index.NumberTradedStocks = ind.mid(34, 3).toInt();
	integerPart = ind.mid(37, 3).toInt();
	fractionalPart = ind.mid(40, 2).toInt();
	index.PercentCapital = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f);
	// Reserved 6
	index.IndDirectionVariation = ind.mid(48, 1).at(0);
	integerPart = ind.mid(49, 3).toInt();
	fractionalPart = ind.mid(52, 2).toInt();
	index.PercentVariation = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f);
	// Reserved 6
	index.IndDirectionVariationPrevYear = ind.mid(60, 1).at(0);
	integerPart = ind.mid(61, 3).toInt();
	fractionalPart = ind.mid(64, 2).toInt();
	index.PercentVariationPrevYear	= static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 100.0f);
	// Reserved 16
	index.NumberFalling = ind.mid(82, 3).toInt();
	index.NumberRising = ind.mid(85, 3).toInt();
	index.NumberNoVariation = ind.mid(88, 3).toInt();
	index.NumberNoQuotation = ind.mid(91, 3).toInt();
	index.NumberReserved = ind.mid(94, 3).toInt();
	index.NumberSuspended = ind.mid(97, 3).toInt();
	index.NumberTotal = ind.mid(100, 3).toInt();
	// Reserved 19
	index.IndSupplementaryData = ind.mid(122, 1).toInt();	
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseCompositionOfIndex(const QString& index, VizSCompositionOfIndex& compOfIndex)
{
	for (int i = 0; i < 30; i++)
	{
		compOfIndex.StockCode[i] = index.mid(0 + (27*i), 12);
		compOfIndex.StockSymbol[i] = index.mid(12 + (27*i), 8);

		int integerPart(-1);
		int fractionalPart(-1);

		integerPart = index.mid(20 + (27*i), 3).toInt();
		fractionalPart = index.mid(23 + (27*i), 4).toInt();

		compOfIndex.Contribution[i] = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 10000.0f);
	}

	compOfIndex.IndLastMsg = index.mid(27 + (27*29), 1).at(0);
	compOfIndex.NumberOfStocks = index.mid(28 + (27*29), 3).toInt();
	compOfIndex.PrevDayIndexFormat = index.mid(31 + (27*29), 1).at(0);
	compOfIndex.PrevDayIndex = index.mid(32 + (27*29), 7).toUInt();
	compOfIndex.IndFreqOfCalc = index.mid(39 + (27*29), 1).at(0);		
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseCancellationInOrderBook(const QString& cancel, VizSCancelOrderBook& cancelOB)
{
	cancelOB.CancellationType = cancel.mid(0, 1).at(0);
	cancelOB.OfferBrokerId = cancel.mid(1, 8);
	cancelOB.OfferDate = cancel.mid(9, 8).toUInt();
	cancelOB.OfferSequentialNumber = cancel.mid(17, 6).toInt();
	cancelOB.OfferDirection = cancel.mid(23, 1).at(0);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseUpdateOrderBook(const QString& update, VizSUpdateOrderBook& updateOrderBook)
{
	updateOrderBook.ActionType = update.mid(0, 1).at(0);
	updateOrderBook.OfferDirection = update.mid(1, 2).at(0);
	updateOrderBook.RegisteredOfferPriceFormat = update.mid(2, 3).at(0);
	updateOrderBook.RegisteredOfferPrice = update.mid(3, 13).toULongLong();
	updateOrderBook.QuantityShown = update.mid(16, 12).toULongLong();
	// Reserved 12
	updateOrderBook.TheoreticalOpeningPriceFormat = update.mid(40, 1).at(0);
	updateOrderBook.TheoreticalOpeningPrice = update.mid(41, 13).toULongLong();
	// Reserved 3
	updateOrderBook.OfferBrokerId = update.mid(57, 8);
	updateOrderBook.OfferDate = update.mid(65, 8).toUInt();
	updateOrderBook.OfferSequentialNumber = update.mid(73, 6).toInt();
	updateOrderBook.PriceType = update.mid(79, 1).at(0);
	updateOrderBook.OrderTimeStamp = update.mid(80, 20).toULongLong();
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseRetransmitTheOrderBook(const QString& ret, VizSRetOrderBook& retOrderBook)
{
	retOrderBook.IndicatorBeginAndEnd = ret.mid(0, 1).at(0); 
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseRegistryOfStock(const QString& registry, VizSStockRegistry& registryOfStock)
{
	registryOfStock.Name = registry.mid(0, 12);
	registryOfStock.Spec = registry.mid(12, 10);
	// Reserved 20
	registryOfStock.NumberOfShares = registry.mid(42, 15).toULongLong();
	// Reserved 139
	registryOfStock.QuotationGroup = registry.mid(196, 2);
	// Reserved 21
	registryOfStock.StandardLot = registry.mid(219, 12).toULongLong();
	// Reserved 5
	registryOfStock.FormOfQuotation = registry.mid(236, 1).toInt();
	registryOfStock.DateOfLastTrade = registry.mid(237, 8).toUInt();
	// Reserved 89
	registryOfStock.PrevDayClosingPriceFormat = registry.mid(334, 1).at(0);
	registryOfStock.PrevDayClosingPrice = registry.mid(335, 13).toULongLong();
	// Reserved 14
	registryOfStock.OptionExercisePriceFormat = registry.mid(362, 1).at(0);
	registryOfStock.OptionExercisePrice = registry.mid(363, 13).toULongLong();
	// Reserved 73
	registryOfStock.ISINCode = registry.mid(449, 12);
	// Reserved 84
	registryOfStock.AverageQuantity = registry.mid(545, 11);
	
	int integerPart(-1);
	int fractionalPart(-1);

	//integerPart = registry.mid(556, 4).toInt();
//	fractionalPart = registry.mid(560, 3).toInt();
//	registryOfStock.FrozenCrossTradesLastTradePricePercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	registryOfStock.FrozenCrossTradesLastTradePricePercent = string2f32(registry.mid(556, 7), 4, 3);
	integerPart = registry.mid(563, 4).toInt();
	fractionalPart = registry.mid(567, 3).toInt();
	registryOfStock.FrozenCrossTradesAveragePercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	integerPart = registry.mid(570, 4).toInt();
	fractionalPart = registry.mid(574, 3).toInt();
	registryOfStock.FrozenCrossTradesTotalPercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	integerPart = registry.mid(577, 4).toInt();
	fractionalPart = registry.mid(581, 3).toInt();	
	registryOfStock.FrozenNonCrossLastTradePricePercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	integerPart = registry.mid(584, 4).toInt();
	fractionalPart = registry.mid(588, 3).toInt();
	registryOfStock.FrozenNonCrossTradesAveragePercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	integerPart = registry.mid(591, 4).toInt();
	fractionalPart = registry.mid(595, 3).toInt();
	registryOfStock.FrozenNonCrossTradesTotalPercent = static_cast<f32>(integerPart) + (static_cast<f32>(fractionalPart) / 1000.0f);
	// Reserved 10
	registryOfStock.OptionIndication = registry.mid(608, 1).at(0);
	registryOfStock.OptionExpirationDate = registry.mid(609, 8).toUInt();
	registryOfStock.OptionExpirationTime = registry.mid(617, 6).toInt();
	registryOfStock.TradingCodeOfOption = registry.mid(623, 12);
	// Reserved 1
	registryOfStock.MarketSegment = registry.mid(636, 2);
	// Reserved 19
	registryOfStock.IPDDRV = registry.mid(657, 1).at(0);
	registryOfStock.QuantityMultiplicationCoef = registry.mid(658, 8).toUInt();

}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseScheduleByQuotationGroup(const QString& sched, VizSSessionSchedule& sessionSchedule)
{
	sessionSchedule.QuoutationGroupCode = sched.mid(0, 2);
	sessionSchedule.PreopeningTime = sched.mid(2, 6).toInt();
	sessionSchedule.OpeningTime = sched.mid(8, 6).toInt();
	sessionSchedule.ClosingTime = sched.mid(14, 6).toInt();
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseSummaryOfOpening(const QString& summary, VizSSummaryOfOpening& openingSummary)
{
	openingSummary.FirstPriceFormat = summary.mid(0, 1).at(0);
	openingSummary.FirstPrice = summary.mid(1, 13).toULongLong();
	openingSummary.LastPriceFormat = summary.mid(14, 1).at(0);
	openingSummary.LastPrice = summary.mid(15, 13).toULongLong();
	openingSummary.BestPriceFormat = summary.mid(28, 1).at(0);
	openingSummary.BestPrice = summary.mid(29, 13).toULongLong();
	openingSummary.LowestPriceFormat = summary.mid(42, 1).at(0);
	openingSummary.LowestPrice = summary.mid(43, 13).toULongLong();
	openingSummary.TypeOfLastPrice = summary.mid(56, 2).toInt();
	openingSummary.AccumulatedAmount = summary.mid(58, 12).toULongLong();
	// Reserved 6
	openingSummary.IndicVarInRelationToPreviousPrice = summary.mid(76, 1).at(0);
	openingSummary.VariationFormatIndicator = summary.mid(77, 1).at(0);
	openingSummary.Variation = summary.mid(78, 13).toULongLong();
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseTheoreticalOpeningPrice(const QString& theoretical, VizSTheoreticalOpeningPrice& theoreticalOpenPrice)
{
	theoreticalOpenPrice.TheoreticalOpenPriceFormat = theoretical.mid(0, 1).at(0);
	theoreticalOpenPrice.TheoreticalOpenPrice = theoretical.mid(1, 13).toULongLong();
	theoreticalOpenPrice.TheoreticalQuantityTraded = theoretical.mid(14, 12).toULongLong();
	// Reserved 6
	theoreticalOpenPrice.DirectionOfOffer = theoretical.mid(32, 1).at(0);
	theoreticalOpenPrice.QuantityNotTakenAtOpening = theoretical.mid(33, 12).toULongLong();
	// Reserved 52
	theoreticalOpenPrice.VariationFormatIndicator = theoretical.mid(97, 1).at(0);
	theoreticalOpenPrice.Variation = theoretical.mid(98, 13).toULongLong();
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseAlterationOfQuotationGroupStatus(const QString& status, VizSAltQuotGroupStatus& quotGroupStatus)
{
	quotGroupStatus.QuotationGroupCode = status.mid(0, 2);
	quotGroupStatus.QuotationGroupCondition = status.mid(2, 1).at(0);
	quotGroupStatus.NumbOfCollectors = status.mid(3, 2).toInt();
	// Reserved 23
	// Length of text in body message: 28
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseChangeOfStockStatus(const QString& change, VizSChangeStockStatus& changeOfStockStatus)
{
	changeOfStockStatus.TradingStatus = change.mid(0, 1).at(0);
	// Reserved 16
	changeOfStockStatus.Status = change.mid(17, 1).at(0);
	changeOfStockStatus.StockType = change.mid(18, 1).at(0);
	int hour = change.mid(19, 2).toInt();
	int minute = change.mid(19 + 2, 2).toInt();
	int second = change.mid(19 + 2 + 2, 2).toInt();
	changeOfStockStatus.OpeningTime = QTime(hour, minute, second);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFiveBestOfferPrices(const QString& fiveBestMsg, VizSFiveBestPrices& fiveBestPrices)
{
	// Reserved 7
	fiveBestPrices.IndicBestOfferAltered = fiveBestMsg.mid(7, 6);	

	QString bestOffer(fiveBestMsg.mid(12, 62));
	fiveBestPrices.occurences = 0;
	while (bestOffer.size() == 62)
	{
		fiveBestPrices.TotalQuantityBids[fiveBestPrices.occurences] = bestOffer.mid(0, 12).toULongLong();
		fiveBestPrices.NumberOfBids[fiveBestPrices.occurences] = bestOffer.mid(12, 4).toInt();
		fiveBestPrices.BestBidPricesFormat[fiveBestPrices.occurences] = bestOffer.mid(16, 1).at(0);
		fiveBestPrices.BestBidPrice[fiveBestPrices.occurences] = bestOffer.mid(17, 13).toULongLong();
		fiveBestPrices.BestAskPricesFormat[fiveBestPrices.occurences] = bestOffer.mid(30, 1).at(0);
		fiveBestPrices.BestAskPrice[fiveBestPrices.occurences] = bestOffer.mid(31, 13).toULongLong();
		fiveBestPrices.NumberOfAsks[fiveBestPrices.occurences] = bestOffer.mid(44, 4).toInt();
		fiveBestPrices.TotalQuantityOfAsks[fiveBestPrices.occurences] = bestOffer.mid(48, 12).toULongLong();
		// Reserved 2 
		fiveBestPrices.occurences++;

		bestOffer = fiveBestMsg.mid((fiveBestPrices.occurences * 62) + 12, 62);
	}
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parsePriceUpdate(const QString& update, VizSPriceUpdate& priceUpdate)
{
	priceUpdate.LastPriceFormat = update.mid(0, 1).at(0);	
	priceUpdate.LastPrice = update.mid(1, 13).toULongLong();
	priceUpdate.HighestPriceFormat = update.mid(14, 1).at(0);	
	priceUpdate.HighestPrice = update.mid(15, 13).toULongLong();
	priceUpdate.LowestPriceFormat = update.mid(28, 1).at(0);	
	priceUpdate.LowestPrice = update.mid(29, 13).toULongLong();
	priceUpdate.PriceType = update.mid(42, 2).toInt();
	// Reserved 6
	priceUpdate.IndicVarInRelationToPreviousPrice = update.mid(50, 1).at(0);
	priceUpdate.VariationFormatIndicator = update.mid(51, 1).at(0);
	priceUpdate.Variation = update.mid(52, 13).toULongLong();
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseTrade(const QString& trade, VizSTrade& tradeStruct)
{
	tradeStruct.QuantityTraded = trade.mid(0, 12).toULongLong();
	tradeStruct.TradePriceFormat = trade.mid(12, 1).at(0);
	tradeStruct.TradePrice = formatField2f64(tradeStruct.TradePriceFormat, trade.mid(13, 13));
	tradeStruct.BuyerBrokerId = trade.mid(26, 8);
	tradeStruct.SellerBrokerId = trade.mid(34, 8);
	tradeStruct.QuantityTradesAccumInDay = trade.mid(42, 12).toULongLong();
	// Reserved 8  
	tradeStruct.HighestPriceFormat = trade.mid(62, 1).at(0);
	tradeStruct.HighestPrice = formatField2f64(tradeStruct.HighestPriceFormat, trade.mid(63, 13));
	tradeStruct.LowestPriceFormat = trade.mid(76, 1).at(0);
	tradeStruct.LowestPrice = formatField2f64(tradeStruct.LowestPriceFormat, trade.mid(77, 13));
	tradeStruct.TypeOfRegistration = trade.mid(90, 2).toInt();
	// Reserved 4
	tradeStruct.OriginOfTrade = trade.mid(96, 1).at(0);
	// Reserved 2
	tradeStruct.IndicVarInRelationToPreviousPrice = trade.mid(99, 1).at(0);
	tradeStruct.TradeNumber = trade.mid(100, 7).toUInt();
	// Reserved 3
	tradeStruct.MarketSegment = trade.mid(110, 2);
	// Reserved 69
	tradeStruct.PeriodOfSettlementInDays = trade.mid(181, 4);
	// Reserved 13
	int year = trade.mid(198, 4).toInt();
	int month = trade.mid(198 + 4, 2).toInt();
	int day = trade.mid(198 + 4 + 2, 2).toInt();
	QDate d(year, month, day);
	int hour = trade.mid(206, 2).toInt();
	int minute = trade.mid(206 + 2, 2).toInt();
	int second = trade.mid(206 + 2 + 2, 2).toInt();
	QTime t(hour, minute, second);
	tradeStruct.TradeDateTime = QDateTime(d, t);
	tradeStruct.VariationFormatIndicator = trade.mid(212, 1).at(0);
	tradeStruct.Variation = formatField2f64(tradeStruct.VariationFormatIndicator, trade.mid(213, 13));
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::fromTradeToRealTimePeriod(const VizSTrade& trade, core::VizCRealTimePeriod* rt)
{
	rt->QuantityTrades = trade.QuantityTraded;
	rt->TradePrice = trade.TradePrice;
	rt->QuantityTradesAccumInDay = trade.QuantityTradesAccumInDay;
	rt->HighestPrice = trade.HighestPrice;
	rt->LowestPrice = trade.LowestPrice;
	rt->MarketSegment = static_cast<VizEMarketSegment>(trade.MarketSegment.toInt());
	rt->PeriodOfSettlementInDays = trade.PeriodOfSettlementInDays; 
	rt->TradeDateTime = trade.TradeDateTime;	
	rt->Variation = trade.Variation;

	//qDebug() << rt->TradeDateTime << rt->TradePrice << " " << trade.BuyerBrokerId << " " << trade.SellerBrokerId;
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseOpeningTrade(const QString& trade, VizSOpeningTrade& openingTrade)
{
	openingTrade.QuantityTraded = trade.mid(0, 12).toULongLong();
	openingTrade.TradePriceFormat = trade.mid(12, 1).at(0);
	openingTrade.TradePrice = formatField2f64(openingTrade.TradePriceFormat, trade.mid(13, 13));
	openingTrade.BuyerBrokerId = trade.mid(26, 8);
	openingTrade.SellerBrokerId = trade.mid(34, 8);
 	openingTrade.QuantityTradesAccumInDay = trade.mid(42, 12).toULongLong();
	// Reserved 11  
 	openingTrade.IndicEndOfTradesAtOpenPrice = trade.mid(65, 1).toInt();
	// Reserved 2 
	openingTrade.IndicVarInRelationToPreviousPrice = trade.mid(68, 1).at(0);
 	openingTrade.TradeNumber = trade.mid(69, 7).toUInt();
	// Reserved 3
 	openingTrade.MarketSegment = trade.mid(79, 2);
	// Reserved 86
	int year = trade.mid(167, 4).toInt();
	int month = trade.mid(167 + 4, 2).toInt();
	int day = trade.mid(167 + 4 + 2, 2).toInt();
	QDate d(year, month, day);
	int hour = trade.mid(175, 2).toInt();
	int minute = trade.mid(175 + 2, 2).toInt();
	int second = trade.mid(175 + 2 + 2, 2).toInt();
	QTime t(hour, minute, second);
 	openingTrade.TradeDateTime = QDateTime(d, t);
 	openingTrade.TradeOrigin = trade.mid(181, 1).at(0);
 	openingTrade.VariationFormatIndicator = trade.mid(182, 1).at(0);
	openingTrade.Variation = formatField2f64(openingTrade.VariationFormatIndicator, trade.mid(183, 13));
	
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::fromOpeningTradeToRealTimePeriod(const VizSOpeningTrade& openingTrade, core::VizCRealTimePeriod* rt)
{
	rt->QuantityTrades = openingTrade.QuantityTraded;
	rt->TradePrice = openingTrade.TradePrice;
	rt->QuantityTradesAccumInDay = openingTrade.QuantityTradesAccumInDay;
	rt->HighestPrice = openingTrade.TradePrice;
	rt->LowestPrice = openingTrade.TradePrice;
	rt->MarketSegment = static_cast<VizEMarketSegment>(openingTrade.MarketSegment.toInt());
	rt->PeriodOfSettlementInDays = ""; // Nao eh termo
	rt->TradeDateTime = openingTrade.TradeDateTime;	
	rt->Variation = openingTrade.Variation;

	//qDebug() << rt->TradeDateTime << rt->TradePrice << " " << openingTrade.BuyerBrokerId << " " << openingTrade.SellerBrokerId;
}
//-----------------------------------------------------------------------------
void VizCMegabolsaMsgParser::parseFunctionalHeader(const QString& head, VizSMegabolsaHeader& header)
{
	header.MessageType = head.mid(3, 2);
	int quotationGroup = head.mid(11, 2).toInt();
	header.StockCode = head.mid(13, 12).trimmed();

	//qDebug() << header.StockCode;

	int year = head.mid(33, 4).toInt();
	int month = head.mid(33 + 4, 2).toInt();
	int day = head.mid(33 + 4 + 2, 2).toInt();

	int hour = head.mid(41, 2).toInt();
	int minute = head.mid(41 + 2, 2).toInt();
	int second = head.mid(41 + 2 + 2, 2).toInt();

	QDate msgDate(year, month, day);
	QTime msgTime(hour, minute, second);

	header.DateAndTime = QDateTime(msgDate, msgTime);	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
