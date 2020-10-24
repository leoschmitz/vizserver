//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCOMPANY_H__
#define __VIZCOMPANY_H__
//-----------------------------------------------------------------------------
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
class VizCompany
{
public:
	VizSecurity() : Id(-1), ISIN(""), Symbol(""), Name(""), WholeName(""), Bdi(viz::VESB_INVALID), TPMerc(viz::VEMT_INVALID), Type("")
	{
	}
	VizSecurity(s32 id, const QString& isin, const QString& symbol, const QString& name, const QString& wholeName, const VizESecurityBDI bdi, const VizEMarketType tpmerc, const QString& type) : Id(id), ISIN(isin), Symbol(symbol), Name(name), WholeName(wholeName), Bdi(bdi), TPMerc(tpmerc), Type(type)
	{
	}
	VizSecurity(const VizSecurity& other)
	{
		set(other);
	}
	inline void set(s32 id, const QString& isin, const QString& symbol, const QString& name, const QString& wholeName, const VizESecurityBDI bdi, const VizEMarketType tpmerc, const QString& type)
	{
		Id = Id;
		ISIN = isin;
		Symbol = symbol;
		WholeName = wholeName;
		Name = name;
		Bdi = bdi;
		TPMerc = tpmerc;
		Type = type;
	}
	inline void get(s32& id, QString& isin, QString& symbol, QString& name, QString& wholeName, VizESecurityBDI bdi, VizEMarketType tpmerc, QString& type) const
	{
		id = Id;
		isin = ISIN;
		symbol = Symbol;
		wholeName = WholeName;
		name = Name;
		bdi = Bdi;
		tpmerc = TPMerc;
		type = Type;
	}
	inline void set(const VizSecurity& other)
	{
		set(other.Id, other.ISIN, other.Symbol, other.Name, other.WholeName, other.Bdi, other.TPMerc, other.Type);
	}
	inline void get(VizSecurity& other) const
	{
		get(other.Id, other.ISIN, other.Symbol, other.Name, other.WholeName, other.Bdi, other.TPMerc, other.Type);
	}
	inline VizSecurity& operator=(const VizSecurity& other)
	{
		set(other);
		return *this;
	}		
	inline bool operator==(const VizSecurity& other) const
	{
		return (crc() == other.crc());
	}
	inline bool operator!=(const VizSecurity& other) const
	{
		return crc() != other.crc();
	}
	inline bool operator<(const VizSecurity& other) const
	{
		return Symbol < other.Symbol;
	}
	inline quint16 crc() const
	{
		QString toChar(ISIN);
		toChar.append(QString::number(Id));
		toChar.append(Symbol);
		toChar.append(Name);
		toChar.append(WholeName);
		toChar.append(QString::number(Bdi));
		toChar.append(QString::number(TPMerc));
		toChar.append(Type);

		return qChecksum(toChar.toLatin1(), toChar.size());
	}
	s32 Id;
	QString ISIN; // International Securities Identifying Number
	QString Symbol;
	QString Name;
	QString WholeName;
	VizESecurityBDI Bdi;
	VizEMarketType TPMerc;	
	QString Type;
};
//-----------------------------------------------------------------------------
static inline QDataStream& operator>>(QDataStream& in, VizSecurity& security)
{
	in >> security.Id;
	in >> security.ISIN;
	in >> security.Symbol;
	in >> security.Name;
	in >> security.WholeName;
	in >> security.Bdi;
	in >> security.TPMerc;
	in >> security.Type;

	return in;	
}
//-----------------------------------------------------------------------------
static inline QDataStream& operator<<(QDataStream& out, const VizSecurity& security)
{
	out << security.Id;
	out << security.ISIN;
	out << security.Symbol;
	out << security.Name;
	out << security.WholeName;
	out << security.Bdi;
	out << security.TPMerc;
	out << security.Type;

	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
