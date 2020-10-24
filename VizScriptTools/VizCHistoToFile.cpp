//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCHistoToFile.h"
#include <QtAlgorithms>
#include <QString>

#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QTextStream>

#include <QTime>
#include <QDebug>

//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCHistoToFile::VizCHistoToFile() 
{
	DbUtils = VizCDatabaseUtils::instance();	

	// Ainda não estão na base de dados
// 	openMegaBolsaFile("COTAHIST.A1986");
// 	openMegaBolsaFile("COTAHIST.A1987");
// 	openMegaBolsaFile("COTAHIST.A1988");
// 	openMegaBolsaFile("COTAHIST.A1989");
// 	openMegaBolsaFile("COTAHIST.A1990");
// 	openMegaBolsaFile("COTAHIST.A1991");
// 	openMegaBolsaFile("COTAHIST.A1992");
// 	openMegaBolsaFile("COTAHIST.A1993");
// 	openMegaBolsaFile("COTAHIST.A1994");
// 	HistoricCompanies.clear();
// 	HistoricSecurities.clear();	
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A1995");	
// 	mountCompanies();
// 	DbUtils->insertNoCheckFullStockRegister(HistoricCompanies.values());
// 	HistoricCompanies.clear();
// 	HistoricSecurities.clear();
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A1996");	
// 	mountCompanies();
// 	DbUtils->insertNoCheckFullStockRegister(HistoricCompanies.values());
// 	HistoricCompanies.clear();
// 	HistoricSecurities.clear();
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A1997");
// 	mountCompanies();
// 	DbUtils->insertNoCheckFullStockRegister(HistoricCompanies.values());
// 	HistoricCompanies.clear();
// 	HistoricSecurities.clear();
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A1998");
// 	mountCompanies();
// 	DbUtils->insertNoCheckFullStockRegister(HistoricCompanies.values());
// 	HistoricCompanies.clear();
// 	HistoricSecurities.clear();

	// Já estão na base de dados
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A1999");
// 	openMegaBolsaFile("BovespaData\\COTAHIST.A2000");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2001");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2002.TXT");
//	openMegaBolsaFile("BovespaData\\COTAHIST_A2003.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2004.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2005.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2006.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2007.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2008.TXT");
// 	openMegaBolsaFile("BovespaData\\COTAHIST_A2009.TXT");	

// 	mountCompanies();
// 	DbUtils->insertNoCheckFullStockRegister(HistoricCompanies.values());

	// Apenas teste
//	openMegaBolsaFile("DemoCotacoesHistoricas12022003.txt");	
}
//-----------------------------------------------------------------------------
VizCHistoToFile::~VizCHistoToFile()
{
}
//-----------------------------------------------------------------------------
bool VizCHistoToFile::readHeader(QString head)
{	
	if (head.size() != 245)
		return false;
	// Tipo de registro 00
	// Nome do arquivo COTAHIST.AAAA
	if (head.left(11) != "00COTAHIST.")
		return false;
	bool ok = false;
	int year = head.mid(11, 4).toInt(&ok);
	if (!ok) 
		return false;
	
	// Fixo "BOVESPA "
	if (head.mid(15, 7) != "BOVESPA")
		return false;	
	
	// Data da geracao "AAAAMMDD"
	int genYear = head.mid(23, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int genMonth = head.mid(27, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int genDay = head.mid(29, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate generation(genYear, genMonth, genDay);		
	// O resto eh reserva	

	return true;
}
//-----------------------------------------------------------------------------
int VizCHistoToFile::makeISINcheckBit(QString isin)
{
	int d1, d2, sum, multiply, i;

	for (sum = 0, multiply = 1, i = 10; i > -1; --i) 
	{
		switch (i) 
		{
			case 0:
			case 1:				
				d1 = isin.at(i).toAscii() - 'A' + 10;
				break;
			default:
				if (isin.at(i).isUpper())
					d1 = isin.at(i).toAscii() - 'A' + 10;
				else if (isin.at(i).isDigit())
					d1 = isin.at(i).toAscii() - '0';
				else
					return 0;
				break;
		}

		if (d1 < 10) 
		{
			d1 *= (multiply ? 2 : 1);
			multiply = !multiply;
		} 
		else 
		{
			d2 = d1 / 10;
			d1 %= 10;
			d1 *= (multiply ? 2 : 1);
			d2 *= (multiply ? 1 : 2);
			sum += (d2 % 10) + (d2 / 10);
		}
		sum += (d1 % 10) + (d1 / 10);
	}

	sum %= 10;
	sum = 10 - sum;
	sum %= 10;

	return sum;
}
//-----------------------------------------------------------------------------
QString VizCHistoToFile::makeISIN(QString possibleCODINT)
{
	if (possibleCODINT.left(2) == "BR")
	{
		if (!possibleCODINT.contains(' '))
			return possibleCODINT;		
	}

	QString isin;
	QString tipoAtivo;
	QString identificadorTitulo;

	QString socemi = possibleCODINT.left(4);
	QString tiptit = possibleCODINT.mid(4, 2);
	QString espano = possibleCODINT.mid(6, 4);
	QString dismes = possibleCODINT.mid(10, 3);

	// Descobrindo o tipo de ativo
	if (tiptit == "AC")
	{
		if (espano.at(1) == 'N')
		{
			if (espano.at(2) == 'R')
				 tipoAtivo = "ARN";
			else tipoAtivo = "ACN";
		}		

		if (socemi == "YPFL")
			tipoAtivo = "CDA";
	}

	if (tiptit == "CI")
	{
		if (socemi == "UBBR")
			tipoAtivo = "CDA";
		else 
			if (socemi == "BCBR")
				tipoAtivo = "CPC";
			else tipoAtivo = "CTF";
	}

	if (tiptit == "IN")
		tipoAtivo = "IND";

	if ((tiptit == "B1") || (tiptit == "S1"))
		tipoAtivo = "A01";

	if (tiptit == "R1")
		tipoAtivo = "R01";

	if (tiptit == "BC")
		tipoAtivo = "CPB";

	if (tiptit == "TX")
		tipoAtivo = "IND";

	// Descobrindo identificador de titulo
	if (espano.at(0) == 'O')
		identificadorTitulo = "OR";		
	
	if (espano.at(0) == 'P')
		identificadorTitulo = "PR";

	switch (espano.at(2).toAscii())
	{
	case 'A':
		identificadorTitulo = "PA";
		break;
	case 'B':
		identificadorTitulo = "PB";
		break;
	case 'C':
		identificadorTitulo = "PC";
		break;
	case 'D':
		identificadorTitulo = "PD";
		break;
	case 'E':
		identificadorTitulo = "PE";
		break;
	case 'F':
		identificadorTitulo = "PF";
		break;
	case 'G':
		identificadorTitulo = "PG";
		break;
	case 'H':
		identificadorTitulo = "PH";
		break;
	}

	isin = "BR";
	isin.append(socemi);
	isin.append(tipoAtivo);
	isin.append(identificadorTitulo);

	if (!tipoAtivo.isNull() && !identificadorTitulo.isNull())
	{
		int checkdigit = makeISINcheckBit(isin);
		isin.append(QString::number(checkdigit));
	}
	else
	{
		isin = "";
	}
	

	return QString(isin);
}
//-----------------------------------------------------------------------------
bool VizCHistoToFile::readRegister(QString reg)
{
	core::VizCSecurity	security;
	core::VizCSecurityPeriod securityPeriod;

	bool ok = false;
	if (reg.size() != 245)
		return false;

	// Fixo "01"
	if (reg.left(2) != "01")
		return false;	
	// Data do pregao "AAAAMMDD"	
	int genYear = reg.mid(2, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int genMonth = reg.mid(6, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int genDay = reg.mid(8, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate sessionDate(genYear, genMonth, genDay);		
	QDateTime date(sessionDate);
	// Codigo BDI
	int bdiCode = reg.mid(10, 2).toInt(&ok);
	if (!ok) 
		return false;
	// Codigo de negociacao
	QString securityID = reg.mid(12, 12).trimmed();
	// Tipo de mercado
	int marketType = reg.mid(24, 3).toInt(&ok);
	if (!ok)
		return false;
	// Nome resumido da empresa emissora do papel
	QString securityName = reg.mid(27,12).trimmed();	
	// Especificacao do papel
	QString securitySpec = reg.mid(39,10);
	// Prazo em dias do mercado a termo
	QString timeBargainMarket = reg.mid(49, 3);
	// Moeda de referencia
	QString currency = reg.mid(52, 4);
	// Preco abertura
	double openPrice = reg.mid(56, 11).toDouble(&ok);
	if (!ok)
		return false;
	openPrice += reg.mid(67, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Preco maximo
	double maxPrice = reg.mid(69, 11).toDouble(&ok);
	if (!ok)
		return false;
	maxPrice += reg.mid(80, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Preco minimo
	double minPrice = reg.mid(82, 11).toDouble(&ok);
	if (!ok)
		return false;
	minPrice += reg.mid(93, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Preco medio
	double avPrice = reg.mid(95, 11).toDouble(&ok);
	if (!ok)
		return false;
	avPrice += reg.mid(106, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Ultimo preco
	double lastPrice = reg.mid(108, 11).toDouble(&ok);
	if (!ok)
		return false;
	lastPrice += reg.mid(119, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Melhor preco de compra
	double bestBuyOffer = reg.mid(121, 11).toDouble(&ok);
	if (!ok)
		return false;
	bestBuyOffer += reg.mid(132, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Melhor preco de venda
	double bestSellOffer = reg.mid(134, 11).toDouble(&ok);
	if (!ok)
		return false;
	bestSellOffer += reg.mid(145, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Numero de negocios efetuados 
	int numDeals = reg.mid(147, 5).toInt(&ok);
	if (!ok)
		return false;
	// Quantidade total de titulos negociados 
	long long quantVol = reg.mid(152, 18).toLongLong(&ok);
	if (!ok)
		return false;
	// Volume total de titulos negociados 
	double totalVolume = reg.mid(170, 16).toDouble(&ok);
	if (!ok)
		return false;
	totalVolume += reg.mid(186, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Preco de exercicio 
	double strikePrice = reg.mid(188, 11).toDouble(&ok);
	if (!ok)
		return false;
	strikePrice += reg.mid(199, 2).toDouble(&ok) / 100;
	if (!ok)
		return false;
	// Indicador de correcao de preco
	int strikeCorrectionIndex = reg.mid(201, 1).toInt(&ok);
	if (!ok)
		return false;
	// data do vencimento "AAAAMMDD"	
	int deadYear = reg.mid(202, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int deadMonth = reg.mid(206, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int deadDay = reg.mid(208, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate dueDate(deadYear, deadMonth, deadDay);
	// Fator de cotacao
	int askingPriceFactor = reg.mid(210, 7).toLong(&ok);
	if (!ok)
		return false;
	// Preco de exercicio (dolar)
	double strikePricePoints = reg.mid(217, 7).toDouble(&ok);
	if (!ok)
		return false;
	strikePricePoints += reg.mid(224, 6).toDouble(&ok) / 1000000;
	if (!ok)
		return false;
	// Codigo ISIN
	QString ISINcode;
	if (sessionDate.year() > 1996)
		ISINcode = reg.mid(230, 12);
	else
		ISINcode = makeISIN(reg.mid(230,12));
	
	// Numero de distribuicao
	int distribNum = reg.mid(242, 3).toInt(&ok);
	if (!ok)
		return false;	

	// INSERE PERIODO E SECURITY
	s32 databaseId;
	security.ISIN = ISINcode;
	security.Symbol = securityID;
	security.Bdi = static_cast<VizESecurityBDI>(bdiCode);
	//security.TPMerc = static_cast<VizEMarketType>(marketType);
	security.MarketType = static_cast<VizEMarketType>(marketType);
	security.SecurityType = securitySpec;

	securityPeriod.AveragePrice = avPrice;
	securityPeriod.BestBuyOffer = bestBuyOffer;
	securityPeriod.BestSellOffer = bestSellOffer;
	securityPeriod.ClosePrice = lastPrice;
	securityPeriod.ExchangeDate = sessionDate;
	//securityPeriod.CloseTimeStamp = QDateTime(sessionDate);
	securityPeriod.Currency = currency;
	securityPeriod.CurrentValue = lastPrice;
	securityPeriod.DISMES = distribNum;
	securityPeriod.EspeciEvent = securitySpec;
	securityPeriod.ForwardMarketTermDays = timeBargainMarket;
	securityPeriod.INDOPC = static_cast<viz::VizEContractCorrection>(strikeCorrectionIndex);
	securityPeriod.MaturityDate = dueDate;
	securityPeriod.MaxPrice = maxPrice;
	securityPeriod.MinPrice = minPrice;
	securityPeriod.OpenPrice = openPrice;
	//securityPeriod.OpenTimeStamp = QDateTime(sessionDate);
	securityPeriod.QuantityTrades = numDeals;
	securityPeriod.QuotationFactor = askingPriceFactor;
	securityPeriod.StrikePrice = strikePrice;
	securityPeriod.StrikePricePoints = strikePricePoints;
	securityPeriod.TotalVolume = totalVolume;

	// Alguns leiloes da bovespa de antes de 99 nao continham o L devido
	//  ex: GEPA3 na compra da paranapanema pela duke
	if (security.MarketType == VEMT_LEILAO)
	{
		if (security.Symbol.at(security.Symbol.size() - 1) != QChar('L'))
		{
			security.Symbol.append('L');
			securityID.append('L');
		}
	}
	
	if (HistoricSecurities.contains(securityID))
	{
		HistoricSecurities[securityID].SecurityPeriods.push_back(securityPeriod);
	} 
	else
	{
		security.SecurityPeriods.push_back(securityPeriod);
		HistoricSecurities.insert(securityID, security);
	}

	return true; 
}
//-----------------------------------------------------------------------------
bool VizCHistoToFile::readTrailer(QString trailer)
{
	if (trailer.size() != 245)
		return false;
	// Tipo de registro 99
	// Nome do arquivo COTAHIST.AAAA
	if (trailer.left(11) != "99COTAHIST.")
		return false;
	bool ok = false;
	int year = trailer.mid(11, 4).toInt(&ok);
	if (!ok) 
		return false;

	//Fixo "BOVESPA "
	if (trailer.mid(15, 7) != "BOVESPA")
		return false;	

	//Data da geracao "AAAAMMDD"
	int genYear = trailer.mid(23, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int genMonth = trailer.mid(27, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int genDay = trailer.mid(29, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate generation(genYear, genMonth, genDay);		

	// Total de registros
	long totalLines = trailer.mid(31, 11).toLong(&ok);
	if (!ok)
		return false;
	// O resto eh reserva	

	return true;
}
//-----------------------------------------------------------------------------
bool VizCHistoToFile::openMegaBolsaFile(QString fileName)
{
	// Tem que estar no diretorio executavel

	QFile finputFromBovespa(fileName);

	if (!finputFromBovespa.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QByteArray inputFromBovespa = finputFromBovespa.readAll();

	finputFromBovespa.close();

	QTextStream in(&inputFromBovespa, QIODevice::ReadOnly);

	QString head = (!in.atEnd()) ? in.readLine() : "";

	if (!readHeader(head))
		return false;
	
	bool hadTrailer = false;

	while (!in.atEnd()) 
	{			
		QString line = in.readLine();

		if (line.at(0) == '0')
		{
			if (!readRegister(line))
				return false;
			
		}
		else 
		{
			if (!readTrailer(line))
				return false;

			 hadTrailer = true;
		}				
	}

	return hadTrailer;	
}
//-----------------------------------------------------------------------------
void VizCHistoToFile::mountCompanies()
{
	foreach (core::VizCSecurity security, HistoricSecurities)
	{
		if (HistoricCompanies.contains(security.Symbol.left(4)))
		{
			HistoricCompanies[security.Symbol.left(4)].Securities.push_back(security);			
		}
		else
		{
			core::VizCCompany companyToInsert;
			companyToInsert.PartialSecuritySymbol = security.Symbol.left(4);
			companyToInsert.Securities.push_back(security);
			HistoricCompanies.insert(security.Symbol.left(4), companyToInsert);
		}
	}	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
