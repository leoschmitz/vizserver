//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "VizCClientList.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
VizCClientList* VizCClientList::Instance = 0;
//-----------------------------------------------------------------------------
VizCClientList* VizCClientList::instance()
{
	if(!Instance)
	{
		Instance = new VizCClientList;
	}
	return Instance;
}
//-----------------------------------------------------------------------------
VizCClientList::VizCClientList(QWidget* parent) : QListWidget(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	
	setSortingEnabled(true);
	setWordWrap(true);
	
	setEditTriggers(NoEditTriggers);

	setAlternatingRowColors(true);

	setFocusPolicy(Qt::NoFocus);

	connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(onItemClicked(QListWidgetItem*)));
}
//-----------------------------------------------------------------------------
void VizCClientList::addClient(const QString& user, const QString& sessionId, const QString& ip)
{
	QPair<QListWidgetItem*, VizCClientInfoWidget*> pair;
	QListWidgetItem* item;
	VizCClientInfoWidget* widget;

	item = new QListWidgetItem(user);
	widget = new VizCClientInfoWidget(this); 

	pair.first = item;
	pair.second = widget;
	Session.insert(sessionId, pair);

	addItem(Session.value(sessionId).first);
}
//-----------------------------------------------------------------------------
void VizCClientList::removeClient(const QString& sessionId)
{
	if (Session.contains(sessionId))
	{	
		takeItem(row(Session[sessionId].first));
	
		Session.remove(sessionId);
	}
}
//-----------------------------------------------------------------------------
void VizCClientList::onItemClicked(QListWidgetItem * item)
{

}
//-----------------------------------------------------------------------------
VizCClientList::~VizCClientList()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
