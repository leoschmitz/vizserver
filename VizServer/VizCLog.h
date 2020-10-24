//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCLOGH__
#define __VIZCLOGH__
//-----------------------------------------------------------------------------
#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>
#include <QFile>

#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace server
{
//-----------------------------------------------------------------------------
#define vizStartLocalTimer() QTime time;  \
	time.start()
#define vizLogLocalTimerElapsed() vizServerLog() << QString("Demorou %1 ms.").arg(QString::number(time.elapsed()))
//-----------------------------------------------------------------------------
class VizCServerLog  : public QObject
{	
	Q_OBJECT
public:
	static VizCServerLog* instance();		

	void block();
	void unblock();
	bool isBlocked();

	void append(const s32& i);
	void append(const char* c);	
	void append(const QString& s);	

private:
	QFile Traces;		

	inline void appendToDoc(const QString& s);

	//QMutex TimerProtectMutex;	
	
	static VizCServerLog* Instance;

	VizCServerLog(QObject* parent = 0);    	
	~VizCServerLog();
};
//-----------------------------------------------------------------------------
static inline VizCServerLog& vizServerLog()
{
	return *VizCServerLog::instance();
}
//-----------------------------------------------------------------------------
static inline VizCServerLog& operator<<(VizCServerLog& in, const char * c)
{
	in.append(c);

	return in;
}
//-----------------------------------------------------------------------------
static inline VizCServerLog& operator<<(VizCServerLog& in, const s32& i)
{
	in.append(i);

	return in;
}
//-----------------------------------------------------------------------------
static inline VizCServerLog& operator<<(VizCServerLog& in, const QString& s)
{
	in.append(s);

	return in;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif