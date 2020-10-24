//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCLIENTINFOWIDGETH__
#define __VIZCCLIENTINFOWIDGETH__
//-----------------------------------------------------------------------------
#include "VizCClientInfoItem.h"

#include <QObject>
#include <QWidget>
#include <QTableWidget>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCClientInfoWidget : public QTableWidget
{	
	Q_OBJECT
public:
	void addRecord(const VizCClientInfoItem& item);

	VizCClientInfoWidget(QWidget* parent = 0);
	~VizCClientInfoWidget();

private:		
	long unsigned int EditingRow;

};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif