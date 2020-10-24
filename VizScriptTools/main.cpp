//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include <QCoreApplication>
#include <QApplication>
// Tools
#include "VizCCCVMToDatabase.h"
#include "VizCHistoToFile.h"
#include "VizCBovespaListToFile.h"
#include "VizCBovespaIANDownload.h"
#include "VizCSplitToDatabase.h"
//-----------------------------------------------------------------------------
using namespace viz;
using namespace server;
//-----------------------------------------------------------------------------

class IANScriptsApplication : public QApplication
{
public:
	IANScriptsApplication(int& argc, char** argv) : QApplication(argc, argv)
	{
		//download IAN
		//VizCBovespaIANDownload* ian = new VizCBovespaIANDownload();
		//ian->getIANFiles();

		//generate SPLITS
		VizCBovespaIANSplitGenerator* ian = new VizCBovespaIANSplitGenerator();
		ian->generateIANBonde();
	}
	~IANScriptsApplication()
	{
	}
};
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	/////// Conversores //////////////////////////////////////////////////////
	//  VizCBovespaListToFile* BovListToFile = new VizCBovespaListToFile;
	//  VizCHistoToFile* HistoToFile = new VizCHistoToFile;
	//  VizCCCVMToDatabase* CCVM = new VizCCCVMToDatabase;
	//VizCSplitToDatabase* splits = new VizCSplitToDatabase;
	////////////////////////////////////////////////////////////////////////// 	

	/////// AIN DOWNLOAD / SPLITS GENERRATOR /////////////////////////////////
	//IANScriptsApplication app(argc, argv);
	//app.exec();
	return 0;
}
//-----------------------------------------------------------------------------
