//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QtGui>
#include "VizServer.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
//static VizCClientInfoWidget* DummyGlobalClientInfoWidget = 0;
//-----------------------------------------------------------------------------
void VizServer::eagerInitializer()
{
	qsrand(QDateTime::currentDateTime().toTime_t());	

	Log = VizCLog::instance();

	VizCDatabaseUtils * dummyDbInitializer  = VizCDatabaseUtils::instance();
	//VizCClientList* dummyTreeInitializer = VizCClientList::instance();
}
//-----------------------------------------------------------------------------
//VizServer::VizServer(QWidget *parent) : QMainWindow(parent)
VizServer::VizServer(QObject* parent) 
	: QObject(parent)
{
	/////// Conversores //////////////////////////////////////////////////////
	//BovListToFile = new VizCBovespaListToFile;
	//HistoToFile = new VizCHistoToFile;
	//////////////////////////////////////////////////////////////////////////

	eagerInitializer();

	//LogDump = new QTextEdit(this);
	//LogDump = new QTextEdit;
	//LogDump->setReadOnly(true);
	//LogDump->setDocument(Log);


	//ClientInfoWidget = DummyGlobalClientInfoWidget;
	//setCentralWidget(ClientInfoWidget);

	//createActions();
	//createMenus();
	//createDocks();

	// Começa o servidor
	//SecurityServer = new VizCTPServer(this);
	SecurityServer = new VizCTPServer;

	//setWindowTitle(tr("VizServer"));
}
//-----------------------------------------------------------------------------
// void VizServer::createMenus()
// {
// 	FileMenu = menuBar()->addMenu(tr("&Arquivo"));
// 	FileMenu->addAction(OpenAct);
// 	FileMenu->addSeparator();
// 	FileMenu->addAction(ExitAct);
// 
// 	EditMenu = menuBar()->addMenu(tr("&Editar"));
// 
// 	ViewMenu = menuBar()->addMenu(tr("E&xibir"));
// 	ViewMenu->addAction(FullScreenAct);
// 	ViewMenu->addAction(FullDesktopAct);
// 	ViewMenu->addSeparator();
// 
// 	WindowMenu = menuBar()->addMenu(tr("&Janela"));
// 	menuBar()->addSeparator();
// 
// 	HelpMenu = menuBar()->addMenu(tr("Aj&uda"));
// }
//-----------------------------------------------------------------------------
// void VizServer::open()
// {
// 
// }
//-----------------------------------------------------------------------------
// void VizServer::about()
// {
// 	
// }

//-----------------------------------------------------------------------------
// void VizServer::fullScreen()
// {
// 	if(isFullScreen())
// 	{
// 		showNormal();
// 	}
// 	else
// 	{
// 		showFullScreen();
// 	}
// }
//-----------------------------------------------------------------------------
// void VizServer::fullDesktop()
// {
// 
// }
//-----------------------------------------------------------------------------
// void VizServer::createActions()
// {
// 
// 	OpenAct = new QAction(QIcon(":/VizServer/Resources/open.png"), tr("&Abrir..."), this);
// 	OpenAct->setShortcuts(QKeySequence::Open);
// 	OpenAct->setStatusTip(tr("Abrir um papel"));
// 	connect(OpenAct, SIGNAL(triggered()), this, SLOT(open()));
// 
// 	ExitAct = new QAction(tr("&Sair"), this);
// 	ExitAct->setShortcut(tr("Ctrl+Q"));
// 	ExitAct->setStatusTip(tr("Sai da aplicação"));
// 	connect(ExitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
// 
// 	AboutAct = new QAction(QIcon(":/VizServer/Resources/About.png"), tr("&Sobre o VizServer"), this);
// 	AboutAct->setStatusTip(tr("Sobre o VizServer"));
// 
// 	FullScreenAct = new QAction(tr("Visualização em &tela cheia"), this);
// 	FullScreenAct->setStatusTip(tr("Visualização em tela cheia"));
// 	FullScreenAct->setCheckable(true);
// 	connect(FullScreenAct, SIGNAL(triggered()), this, SLOT(fullScreen()));
// 
// 	FullDesktopAct = new QAction(tr("Visualização em todo &desktop"), this);
// 	FullDesktopAct->setStatusTip(tr("Visualização em todo desktop"));
// 	FullDesktopAct->setCheckable(true);
// 	connect(FullDesktopAct, SIGNAL(triggered()), this, SLOT(fullDesktop()));
// }
//-----------------------------------------------------------------------------
// void VizServer::createDocks()
// {
// 	ClientsTreeDock = new QDockWidget(tr("Clientes"), this);
// 	ClientsTreeDock->setObjectName("ClientTreeDock");
// 	ClientsTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
// 	ClientsInfoTree = VizCClientList::instance();
// 	ClientsInfoTree->setParent(ClientsTreeDock);
// 	ClientsTreeDock->setWidget(ClientsInfoTree);
// 	addDockWidget(Qt::LeftDockWidgetArea, ClientsTreeDock);
// 
// 	LogDumpDock = new QDockWidget(tr("Log"), this);
//  	LogDumpDock->setObjectName("ClientLogDock");
//  	LogDumpDock->setAllowedAreas(Qt::BottomDockWidgetArea);
// 	LogDump->setParent(LogDumpDock);
// 	LogDumpDock->setWidget(LogDump);
//  	addDockWidget(Qt::BottomDockWidgetArea, LogDumpDock);
//  
//  	ViewMenu->addAction(ClientsTreeDock->toggleViewAction());
// 	ViewMenu->addAction(LogDumpDock->toggleViewAction());
// }
//-----------------------------------------------------------------------------
VizServer::~VizServer()
{

}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------