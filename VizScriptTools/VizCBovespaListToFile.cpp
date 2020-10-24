//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCBovespaListToFile.h"
//#include "VizCSecurityCache.h"
//#include "VizCLoader.h"

#include <QtAlgorithms>
#include <QString>

#include <QSet>
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
VizCBovespaListToFile::VizCBovespaListToFile() 
{
	DbUtils = VizCDatabaseUtils::instance();
	// Já está na base
	// openMegaBolsaEmissorFile("BovespaData\\EMISSOR.TXT");
	// Ja está na base:
	//openMegaBolsaNegotiableFile("BovespaData\\TITULOS_NEGOCIAVEIS.TXT");
}
//-----------------------------------------------------------------------------
VizCBovespaListToFile::~VizCBovespaListToFile()
{
	
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::readEmissorLine(QString line)
{
	QString symbol;
	QString wholeName;
	long int n1;
	long int n2;

	symbol = line.mid(1, 4);

	wholeName = "";

	int i = 8;
	while ((i != line.mid(8).size() + 8) && (line.at(i) != '\"'))
	{
		wholeName.append(line.at(i));
		i++;
	}

	//pula o , e "
	i+=3;
	QString cnpj = "";
	while ((i != line.mid(8).size() + 8) && (line.at(i) != '\"'))
	{
		cnpj.append(line.at(i));
		i++;
	}
	
	i+=3;
	QString sIssuerCreation = "";
	while ((i != line.mid(8).size() + 8) && (line.at(i) != '\"'))
	{
		sIssuerCreation.append(line.at(i));
		i++;
	}
	
	wholeName = expandSecurityNameAbreviations(wholeName).trimmed();

	QDate issuerCreation(sIssuerCreation.left(4).toInt(), sIssuerCreation.mid(4, 2).toInt(), sIssuerCreation.mid(6, 2).toInt());

	core::VizCCompany companyToInsert;
	companyToInsert.WholeName = wholeName;
	companyToInsert.PartialSecuritySymbol = symbol;
	companyToInsert.CNPJ = cnpj;
	companyToInsert.IssuerCreation = issuerCreation;
	// Nao tem como saber com essa lista.
	// companyToInsert.CurrentStatus = "";
	
	DbUtils->insertCheckingFullStockRegister(companyToInsert);
	return true;
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::readNegotiableHeader(QString head)
{	
	if (head.size() != 220)
		return false;
	// Tipo de registro 00
	// Nome do arquivo TITULOS NEGOCIAVEIS
	if (head.left(21) != "00TITULOS NEGOCIAVEIS")
		return false;

	bool ok = false;
	// Data da geracao "AAAA-MM-DD"
	int genYear = head.mid(30, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int genMonth = head.mid(35, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int genDay = head.mid(38, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate generation(genYear, genMonth, genDay);		
	// O resto eh reserva	

	return true;
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::readNegotiableCompanyRegister(QString reg, core::VizCCompany& company)
{
	if (reg.size() != 220)
		return false;
	// Controle
	if (reg.left(2) != "01")
		return false;

	// Simbolo da empresa
	QString simplifiedSymbol = reg.mid(2, 4);
	// Nome completo (denominacao social)
	QString wholeName = reg.mid(6, 60);	
	wholeName = expandSecurityNameAbreviations(wholeName).trimmed();
	// Nome simplificado
	QString simplifiedName = reg.mid(66, 12).trimmed();
	// O resto eh reserva

	// insere empresa
	company.Name = simplifiedName;
	company.WholeName = wholeName;
	company.PartialSecuritySymbol = simplifiedSymbol;

// 	Nao tem como saber a partir desta lista
// 	companyToInsert.IssuerCreation; 	
	company.CurrentStatus = tr("Ativa");
	

	return true;	
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::readNegotiableSecurityRegister(QString reg, core::VizCCompany& company)
{
	bool ok = false;
	if (reg.size() != 220)
		return false;
	// Controle
	if (reg.left(2) != "02")
		return false; 
	// Simbolo completo, com o tipo
	QString symbol = reg.mid(2, 12).trimmed();
	// Codigo da empresa
	QString companyCode = reg.mid(14, 4);
	// BDI
	VizESecurityBDI bdi = static_cast<VizESecurityBDI>(reg.mid(18, 3).toInt(&ok));
	if (!ok)
		return false;
	// Descricao do codigo bdi
	QString bdiDescription = reg.mid(21, 60).trimmed();
	// codigo internacional
	QString isin = reg.mid(81, 12).trimmed();
	// codigo isin do papel objeto
	QString isinObjeto = reg.mid(93, 12).trimmed();
	// Numero de distribuicao do papel
	s32 distribNumber = reg.mid(105, 3).toInt(&ok);
	if (!ok)
		return false;
	// Codigo do mercado
	s32 MarketType = reg.mid(108, 3).toInt(&ok);
	if (!ok)
		return false;
	// Descricao do mercado
	QString marketDescription = reg.mid(111, 15).trimmed();
	// Numero de serie para os mercados de opcoes, termo secundario e futuro
	s32 serial = reg.mid(126, 7).toInt(&ok);
	if (!ok)
		return false;
	// Especificacao do papel objeto
	QString type = reg.mid(133, 10).trimmed();
	// Data de vencimento para os mercados de opcoes de compra/venda, termo secundario e futuro
	// Formato "AAAA-MM-DD"
	int dueYear = reg.mid(143, 4).toInt(&ok);
	if (!ok) 
		return false;	
	int dueMonth = reg.mid(148, 2).toInt(&ok);
	if (!ok) 
		return false;	
	int dueDay = reg.mid(151, 2).toInt(&ok);
	if (!ok) 
		return false;	
	QDate dueDate(dueYear, dueMonth, dueDay);
	// Preco
	// =====
	// se mercado = 30, valor do contrato
	// se mercado = 70 ou 80, preco de exercicio
	// se mercado = 50, preco de ajuste, 
	// caso contrario, conterah zeros
	double price = reg.mid(153, 11).toDouble(&ok);
	if (!ok) 
		return false;	
	price += (reg.mid(164, 7).toDouble(&ok) / 1000000);
	if (!ok) 
		return false;	
	// Estilo da opcao
	// ===============
	// Para mercado de opcoes
	// 'E' = europeia
	// 'A' = americana
	// para os demais mercados
	// ' ' = fixo
	QString optionStyle = reg.mid(171, 1);
	// Indicador do tipo de moeda/correcao
	// precos de exercicio de opcoes de compra/venda
	// ou valor de contrato para o mercado de termo secundario
	s32 currecyType = reg.mid(172, 3).toInt(&ok);
	if (!ok)
		return false;
	// Descricao do indicador do tipo de moeda/correcao
	// precos de exercicio de opcoes de compra/venda
	// ou valor de contrato para o mercado de termo secundario
	// 
	// 'R$' = se icoatv = 0
	// 'PONTOS' = se icoatv = 6
	QString currecyTypeDescription = reg.mid(175, 15).trimmed();
	// Protecao
	QString protection = reg.mid(190, 3);
	// O resto eh reserva

	// insere ativo na companhia
	core::VizCSecurity securityToInsert
	(
		isin, isinObjeto, symbol, bdi, bdiDescription, static_cast<VizEMarketType>(MarketType),
		marketDescription, serial, type, distribNumber, dueDate, price, optionStyle, currecyType,
		currecyTypeDescription, protection, QDate::currentDate(), QDate(), QDate(), true
	);
	company.Securities.push_back(securityToInsert);
	return true;
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::readNegotiableTrailer(QString trailer)
{
	if (trailer.size() != 220)
		return false;
	// Tipo de registro 09
	if (trailer.left(2) != "09")
		return false;
	bool ok = false;
	// Numero de registros
	int numberOfRegisters = trailer.mid(2, 7).toInt(&ok);
	if (!ok) 
		return false;
	// O resto eh reserva	

	return true;
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::openMegaBolsaNegotiableFile(QString fileName)
{
	QDate currentDate = QDate::currentDate();
	DbUtils->removeLastNegotiableRegisters(currentDate.addDays(-1));

	// Tem que estar no diretorio executavel
	QFile finputFromBovespa(fileName);

	if (!finputFromBovespa.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QByteArray inputFromBovespa = finputFromBovespa.readAll();

	finputFromBovespa.close();

	QTextStream in(&inputFromBovespa, QIODevice::ReadOnly);

	QString head = (!in.atEnd()) ? in.readLine() : "";

	if (!readNegotiableHeader(head))
		return false;

	bool hadTrailer = false;

	core::VizCCompany* newCompany;
	bool firstRegister = true;

	while (!in.atEnd()) 
	{			
		QString line = in.readLine();

		if (line.left(2) == "00")
		{
			if (!readNegotiableHeader(line))
				return false;
		}
		else if (line.left(2) == "01")
		{
			if (!firstRegister)
			{
				if (!DbUtils->insertCheckingFullStockRegister(*newCompany))
					return false;
			}
			else 			
				firstRegister = false;
			
			newCompany = new core::VizCCompany;

			if (!readNegotiableCompanyRegister(line, *newCompany))
				return false;			
		} 
		else if (line.left(2) == "02")
		{
			if (!readNegotiableSecurityRegister(line, *newCompany))
				return false;
		}
		else 
		{
			// insere o ultimo
			if (!DbUtils->insertCheckingFullStockRegister(*newCompany))
				return false;

			if (!readNegotiableTrailer(line))
				return false;

			hadTrailer = true;
		}
	}	

	return hadTrailer;	
}
//-----------------------------------------------------------------------------
bool VizCBovespaListToFile::openMegaBolsaEmissorFile(QString fileName)
{
	// Tem que estar no diretorio executavel

	QFile finputFromBovespa(fileName);

	if (!finputFromBovespa.open(QIODevice::ReadOnly | QIODevice::Text))
	     return false;

	QByteArray inputFromBovespa = finputFromBovespa.readAll();
		
	finputFromBovespa.close();

	QTextStream in(&inputFromBovespa, QIODevice::ReadOnly);

	while (!in.atEnd()) 
	{			
		QString line = in.readLine();

		if (!readEmissorLine(line))
			return false;
	}

	return true;	
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
