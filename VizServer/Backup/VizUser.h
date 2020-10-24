//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZSUSER_H__
#define __VIZSUSER_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <QDateTime>
#include <QString>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
struct VizSAccountType
{
	QString AccountTypeName;
	QString AccountTypeDescription;
	bool AccountTypeActived;
};
//-----------------------------------------------------------------------------
struct VizSUser
{
	QString UserName;
	QString UserPassword;
	QString FullName;
	QString Email;
	QDateTime CreateDate;
	QDateTime FirstLogin;
	QDateTime LastLogin;
	VizSAccountType AccountType;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------