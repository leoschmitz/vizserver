//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCCLIENTLISTH__
#define __VIZCCLIENTLISTH__
//-----------------------------------------------------------------------------
#include "VizCClientInfoWidget.h"

#include <QPair>
#include <QListWidget>
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizCClientList : public QListWidget
{	
	Q_OBJECT
public:
	static VizCClientList* instance();

public slots:
	void addClient(const QString& user, const QString& sessionId, const QString& ip);
	void removeClient(const QString& sessionId);

private slots:
	void onItemClicked(QListWidgetItem * item);

private:		
	QMap<QString, QPair<QListWidgetItem*, VizCClientInfoWidget*> > Session;

	static VizCClientList* Instance;	

	VizCClientList(QWidget* parent = 0);
	~VizCClientList();
signals:
	void setCurrentCentralWidget(QString user);
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif