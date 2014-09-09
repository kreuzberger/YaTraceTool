#ifndef TOOLS_TRACETOOL_TRACESERVER_H
#define TOOLS_TRACETOOL_TRACESERVER_H


#include "TSSimpleServer.h"
#include "lib/tracelib/NTrace.h"
#include <qmap.h>
//Added by qt3to4:
#include <QTimerEvent>

class TSClientSocket;


class TraceServerPara
{
public:
  bool bLog;
  quint16 uiPort;
  quint16 uiTCPort;

  TraceServerPara()
  : bLog (false)
  , uiPort( Trace::TraceServer_Port )
  , uiTCPort( Trace::TraceClient_Port )
  { }
};


class TraceServerAbortHandler: public QObject
{
  Q_OBJECT
public:
  TraceServerAbortHandler()
   : QObject()
   , miTimerAbortId(-1)
  {
    miTimerAbortId = startTimer(50);
  }

public slots:
  void timerEvent(QTimerEvent* pEvent)
  {
    if( 0 != pEvent)
    {
      if( miTimerAbortId == pEvent->timerId() )
      {
      }
    }
  }
private:
  int miTimerAbortId;
};


/*
  The TraceServer class provides a small GUI for the server. It also creates the
  SimpleServer and as a result the server.
*/
class TraceServer: public QObject
{
  Q_OBJECT
public:
  TraceServer(const TraceServerPara& para);
  virtual ~TraceServer();



private slots:

  void rcvFilter( const TSClientSocket& s, const Trace::TracePID&, const Trace::TraceFilter& );
  void rcvMessage( const TSClientSocket& s, const Trace::TraceMessage& );
  void rcvCallback( const TSClientSocket& s, const Trace::TracePID&, const Trace::TraceCallback& );
  //void readyRead(QSocket*);
  void setText(const QString&);
  void clearFilter(const TSClientSocket& s, const Trace::TracePID& );
  void onConnect(const Trace::TracePort& port);

private:
  //void rcvFilter(const Trace::TraceFilter&, Trace::TracePort port);
  //void rcvText(const QStringList& , Trace::TracePort port);
  TraceServer();
  TraceServer( const TraceServer& );
  TraceServer& operator=( const TraceServer& );
  TraceServer* self() {return(this);}
  void dbgFilter(const Trace::TraceFilterMap& filter);
  void dbgFilter(const Trace::TraceFilter& filter);
  void dbgCallback(const Trace::TraceCallbackMap&);
  void dbgCallback(const Trace::TraceCallback&);

  Trace::TraceFilterMap mFilterMap;
  Trace::TraceCallbackMap mCallbackMap;
  Trace::TracePort mServerPort;
  Trace::TracePort mTraceClientPort;
  TSSimpleServer mServer;
  TSSimpleServer mTraceClientServer;
  TraceServerPara mTSPara;


signals:
  void sendText(const Trace::TracePort port, const Trace::TraceMessage&);
  void sendFilter( const Trace::TracePort port, const Trace::TraceFilter&);
  void sendCallback( const Trace::TracePort port, const Trace::TraceCallback&);
  void sendFilterTraceClient( const Trace::TraceFilter&);
};

#endif

