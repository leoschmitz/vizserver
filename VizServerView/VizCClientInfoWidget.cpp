//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "VizCClientInfoWidget.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCClientInfoWidget::VizCClientInfoWidget(QWidget* parent) : QTableWidget(parent), EditingRow(0)
{	
	setMinimumSize(400, 205);
	setFocusPolicy(Qt::NoFocus);
	setColumnCount(4);
	QStringList headers;

	headers << QObject::tr("Data e hora") << QObject::tr("Direção") << QObject::tr("Tipo do pacote") << QObject::tr("Registro");
	setHorizontalHeaderLabels(headers);
}
//-----------------------------------------------------------------------------
void VizCClientInfoWidget::addRecord(const VizCClientInfoItem& item)
{
	if ((EditingRow % 100) == 0)
		setRowCount(rowCount() + 100);
	

	QTableWidgetItem *tableItem = new QTableWidgetItem(tr("%1").arg(item.TimeStart.toString(Qt::ISODate)));
	setItem(EditingRow, 0, tableItem);
	tableItem = new QTableWidgetItem(tr("%1").arg(item.Direction));
	setItem(EditingRow, 1, tableItem);
	tableItem = new QTableWidgetItem(tr("%1").arg(item.PacketType));	
	setItem(EditingRow, 2, tableItem);
	tableItem = new QTableWidgetItem(tr("%1").arg(item.Record));
	setItem(EditingRow, 3, tableItem);
}
//-----------------------------------------------------------------------------
VizCClientInfoWidget::~VizCClientInfoWidget()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
