#include "TraceServer.h"
#include "TSClientSocket.h"
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <assert.h>


TraceServer::TraceServer(const TraceServerPara& para)
: QObject()
, mFilterMap()
, mServerPort( para.uiPort )
, mTraceClientPort (para.uiTCPort )
, mServer(self(),mServerPort)
, mTraceClientServer(self(),mTraceClientPort)
, mTSPara(para)
{
  setText("Traceserver started succesfully");
  setText(QString(" TraceClient Port: %1").arg(mTraceClientPort));
  setText(QString(" Application Port: %1").arg(mServerPort));
}

TraceServer::~TraceServer()
{
  setText("Traceserver shutting down");
}


void TraceServer::setText(const QString& str)
{
  if( mTSPara.bLog)
  {
    fprintf(stdout,"%s\n",qPrintable(str));
    fflush(stdout);
  }
}


void TraceServer::dbgFilter(const Trace::TraceFilterMap& filter)
{
  for(Trace::TraceFilterMap::const_iterator it = filter.begin(); it != filter.end(); ++it)
  {
    setText(QString("filter from PID %1").arg(it.key()));
    dbgFilter(it.value());
  }
}

void TraceServer::dbgCallback(const Trace::TraceCallbackMap& callback)
{
  for(Trace::TraceCallbackMap::const_iterator it = callback.begin(); it != callback.end(); ++it)
  {
    setText(QString("callback from PID %1").arg(it.key()));
    dbgCallback(it.value());
  }
}


void TraceServer::dbgFilter(const Trace::TraceFilter& filter)
{
  for (Trace::TraceFilter::const_iterator it = filter.begin(); it != filter.end(); ++it )
  {
    setText(QString("%1 -> %2").arg(it.key()).arg(it.value()));
  }
}

void TraceServer::dbgCallback(const Trace::TraceCallback& callback)
{
  for (Trace::TraceCallback::const_iterator it = callback.begin(); it != callback.end(); ++it )
  {
    setText(QString("callback %1 :args %2").arg(it.key()).arg(it.value().count()));
    int iArgIdx = 0;
    for(QStringList::const_iterator itArg = it.value().begin(); itArg != it.value().end();++itArg)
    {
      setText(QString("           arg %1: %2").arg(iArgIdx).arg(*itArg));
      iArgIdx++;
    }
  }
}

void TraceServer::rcvFilter( const TSClientSocket& s,
                             const Trace::TracePID& pid,
                             const Trace::TraceFilter& filter )
{
  if( Trace::TraceServer_Port == s.port())
  {
    setText(QString("received filter from attached process %1").arg(pid));
    mFilterMap[pid] = filter;
    mTraceClientServer.sendFilterMap(mFilterMap);
  }
  else if (Trace::TraceClient_Port == s.port())
  {
    setText(QString("received filter from traceclient for process %1").arg(pid));
    if(mFilterMap.contains(pid))
    {
      setText(QString("mFiltermap contains pid, adding filter to map"));
      for(Trace::TraceFilter::const_iterator it= filter.begin(); it != filter.end(); ++it )
      {
        mFilterMap[pid].insert(it.key(),it.value());
      }
      mServer.sendFilter(pid, filter);
    }
  }

  dbgFilter(filter);
}


void TraceServer::rcvCallback( const TSClientSocket& s,
                               const Trace::TracePID& pid,
                               const Trace::TraceCallback& callback)
{
  if( Trace::TraceServer_Port == s.port())
  {
    mCallbackMap[pid] = callback;
    mTraceClientServer.sendCallback(mCallbackMap);
  }
  else if (Trace::TraceClient_Port == s.port())
  {
    mServer.sendCallback(pid, callback);
  }

  dbgCallback(callback);
}



void TraceServer::clearFilter(const TSClientSocket& s, const Trace::TracePID& pid)
{
  if( Trace::TraceServer_Port == s.port())
  {
    setText(QString("application %1 closed connection, erase Filter and Callbacks").arg(pid));
    int iRemoved = mFilterMap.remove(pid);
    assert( 0 < iRemoved );
    mTraceClientServer.sendFilterMap(mFilterMap);
    iRemoved = mCallbackMap.remove(pid);
    assert( 0 < iRemoved );
    mTraceClientServer.sendCallback(mCallbackMap);
  }
}


void TraceServer::rcvMessage( const TSClientSocket& , const Trace::TraceMessage& message )
{
  mTraceClientServer.sendMessage(message);
}


void TraceServer::onConnect(const Trace::TracePort& port)
{
  setText(QString("onConnect for port %1").arg(port));
  if(Trace::TraceClient_Port == port)
  {
    setText(QString("sending Filter and Callbacks"));
    dbgFilter(mFilterMap);
    dbgCallback(mCallbackMap);

    mTraceClientServer.sendFilterMap(mFilterMap);
    mTraceClientServer.sendCallback(mCallbackMap);
  }
}


