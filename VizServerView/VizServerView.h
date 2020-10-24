//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZSERVER_H__
#define __VIZSERVER_H__
//-----------------------------------------------------------------------------
//#include <QMainWindow>
#include <QtService/QtServiceBase>
#include <QTextEdit>
//#include <QDockWidget>
//#include <QAction>

#include "VizCTPServer.h"
#include "VizCHistoToFile.h"
#include "VizCBovespaListToFile.h"
//#include "VizCClientList.h"
//#include "VizCClientInfoWidget.h"
#include "VizCLog.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
class VizServer : public QObject
{
	Q_OBJECT
public:
	//VizServer(QWidget* parent = 0);
	VizServer(QObject* parent = 0);
	~VizServer();

	//private slots:
	// 	void open();
	// 	void about();
	// 	void fullScreen();
	// 	void fullDesktop();

private:
	//QTextEdit* LogDump;
	VizCLog* Log;

	// 	QMenu* FileMenu;
	// 	QMenu* EditMenu;
	// 	QMenu* ViewMenu;
	// 	QMenu* WindowMenu;
	// 	QMenu* HelpMenu;

	// file
	// 	QAction* OpenAct;
	// 	QAction* ExitAct;

	// view
	// 	QAction* FullScreenAct;
	// 	QAction* FullDesktopAct;

	// about
	// 	QAction* AboutAct;
	// 
	// 	QDockWidget* ClientsTreeDock;
	// 	QDockWidget* LogDumpDock;
	// 	VizCClientInfoWidget* ClientInfoWidget;
	// 	VizCClientList* ClientsInfoTree;

	// 	void createActions();
	// 	void createMenus();	
	// 	void createDocks();
	// 
	void eagerInitializer();

	VizCTPServer* SecurityServer;
	VizCBovespaListToFile* BovListToFile;	
	VizCHistoToFile* HistoToFile;
};
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif // __VIZSERVER_H__