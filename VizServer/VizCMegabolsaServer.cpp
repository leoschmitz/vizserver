//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCMegabolsaServer.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCMegabolsaServer::VizCMegabolsaServer(QObject* parent) :  QObject(parent) 
{	 
	PacketDump = VizCRealTimePacketDump::instance();

	ProxyDiff = new VizCProxyDiff();

	connect(ProxyDiff, SIGNAL(message(QString, QString)), this, SLOT(receiveProxyDiffMessage(QString, QString)));

	ProxyDiff->start();	
}
//-----------------------------------------------------------------------------
VizCMegabolsaServer::~VizCMegabolsaServer()
{
}
//-----------------------------------------------------------------------------
void VizCMegabolsaServer::receiveThreadRealTimeRequest(QString symbol, bool input)
{
	qDebug() << symbol << input;

	if (input)
		addTradeRequest(symbol);
	else
		removeTradeRequest(symbol);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaServer::receiveProxyDiffMessage(QString id, QString msg)
{
	parseMessage(id, msg);
}
//-----------------------------------------------------------------------------
void VizCMegabolsaServer::addTradeRequest(const QString& symbol)
{
	if (!TradeRequests.contains(symbol))
	{
		TradeRequests.insert(symbol, 1);
	}
	else
	{
		TradeRequests[symbol]++;
	}
}
//-----------------------------------------------------------------------------
void VizCMegabolsaServer::removeTradeRequest(const QString& symbol)
{
	if (TradeRequests.contains(symbol))
	{
		TradeRequests[symbol]--;

		if (TradeRequests[symbol] <= 0)
			TradeRequests.remove(symbol);
	}
}
//-----------------------------------------------------------------------------
void VizCMegabolsaServer::parseMessage(const QString& id, const QString& msg)
{
	VizSMegabolsaHeader header;

	Parser.parseFunctionalHeader(msg.left(47), header);

	if (header.MessageType.at(0) == '0')
	{
		if (header.MessageType.at(1) == '1')
		{
			VizSOpeningTrade openingTrade;
			
			Parser.parseOpeningTrade(msg.mid(47), openingTrade);

			if (openingTrade.IndicEndOfTradesAtOpenPrice)
			{
				core::VizCRealTimePeriod* rtPeriod = new core::VizCRealTimePeriod;

				Parser.fromOpeningTradeToRealTimePeriod(openingTrade, rtPeriod);

				if (TradeRequests.contains(header.StockCode))
				{
					PacketDump->addPacket(id, rtPeriod, TradeRequests.value(header.StockCode));

					emit sendTickToThread(header.StockCode, id);
				}
			}

		}
		else if (header.MessageType.at(1) == '2')
		{
			VizSTrade commomTrade;

			Parser.parseTrade(msg.mid(47), commomTrade);
			
			core::VizCRealTimePeriod* rtPeriod = new core::VizCRealTimePeriod;

			Parser.fromTradeToRealTimePeriod(commomTrade, rtPeriod);

			if (TradeRequests.contains(header.StockCode))
			{
				PacketDump->addPacket(id, rtPeriod, TradeRequests.value(header.StockCode));

				emit sendTickToThread(header.StockCode, id);
			}
		} 
		// 		else if (header.MessageType.at(1) == '3')
		// 		{
		// 			VizSPriceUpdate update;
		// 			parsePriceUpdate(msg.mid(47), update);
		// 		}
		// 		else if (header.MessageType.at(1) == '4')
		// 		{
		// 			VizSFiveBestPrices fiveBestPrices;
		// 			parseFiveBestOfferPrices(msg.mid(47), fiveBestPrices);
		// 		}
		// 		else if (header.MessageType.at(1) == '5')
		// 		{
		// 			VizSChangeStockStatus changeStockStatus;
		// 			parseChangeOfStockStatus(msg.mid(47), changeStockStatus);
		// 		}
		// 		else if (header.MessageType.at(1) == '7')
		// 		{
		// 			emit startOfTradingDay();
		// 		}
		// 		else if (header.MessageType.at(1) == '8')
		// 		{
		// 			emit closingOfDay();
		// 		}
		// 		else if (header.MessageType.at(1) == '9')
		// 		{
		// 			emit systemOnTheAirWarning();
		// 		}
	}	
	// 	else if (header.MessageType.at(0) == '1')
	// 	{
	// 		if (header.MessageType.at(1) == '6')
	// 		{
	// 			VizSAltQuotGroupStatus quotGroupStatus;
	// 			parseAlterationOfQuotationGroupStatus(msg.mid(47), quotGroupStatus);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == '3')
	// 	{
	// 		if (header.MessageType.at(1) == '0')
	// 		{
	// 			VizSTheoreticalOpeningPrice theor;
	// 			parseTheoreticalOpeningPrice(msg.mid(47), theor);
	// 		}
	// 		else if (header.MessageType.at(1) == '2')
	// 		{
	// 			VizSSummaryOfOpening openingSummary;
	// 			parseSummaryOfOpening(msg.mid(47), openingSummary);
	// 		}
	// 		else if (header.MessageType.at(1) == '9')
	// 		{
	// 			VizSSessionSchedule sessionSchedule;
	// 			parseScheduleByQuotationGroup(msg.mid(47), sessionSchedule);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == '5')
	// 	{
	// 		if (header.MessageType.at(1) == '3')
	// 		{
	// 			VizSStockRegistry registry;
	// 			parseRegistryOfStock(msg.mid(47), registry);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == 'S')
	// 	{
	// 		if (header.MessageType.at(1) == '0')
	// 		{
	// 			VizSRetOrderBook retOB;
	// 			parseRetransmitTheOrderBook(msg.mid(47), retOB);
	// 		}
	// 		else if (header.MessageType.at(1) == '3')
	// 		{
	// 			VizSUpdateOrderBook updateOB;
	// 			parseUpdateOrderBook(msg.mid(47), updateOB);
	// 		}
	// 		else if (header.MessageType.at(1) == '4')
	// 		{
	// 			VizSCancelOrderBook cancelOB;
	// 			parseCancellationInOrderBook(msg.mid(47), cancelOB);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == 'A')
	// 	{
	// 		if (header.MessageType.at(1) == '7')
	// 		{
	// 			VizSCompositionOfIndex compIndex;
	// 			parseCompositionOfIndex(msg.mid(47), compIndex);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == 'B')
	// 	{
	// 		if (header.MessageType.at(1) == '1')
	// 		{
	// 			VizSIndex index;
	// 			parseIndex(msg.mid(47), index);
	// 		}
	// 	}
	// 	else if (header.MessageType.at(0) == 'Z')
	// 	{
	// 		if (header.MessageType.at(1) == '1')
	// 		{
	// 			VizSSecurityLoans loan;
	// 			parseSecurityLoans(msg.mid(47), loan);
	// 		}
	// 		else if (header.MessageType.at(1) == '3')
	// 		{
	// 			VizSNews news;
	// 			parseNews(msg.mid(47), news);
	// 		}
	// // 		else if (header.MessageType.at(1) == '5')
	// // 		{
	// // 			VizSFixedIncomeHeader h;
	// // 			parseFixedIncome(msg.mid(47), h);
	// // 		}
	// 	}
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------