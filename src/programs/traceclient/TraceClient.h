#ifndef TOOLS_TRACETOOL_TRACECLIENT_H
#define TOOLS_TRACETOOL_TRACECLIENT_H


#include <qobject.h>
//Added by qt3to4:
#include <QTimerEvent>
#include "TCClientSocket.h"
#include "lib/tracelib/NTrace.h"

class QTcpSocket;


class TraceClientAbortHandler: public QObject
{
  Q_OBJECT
public:
  TraceClientAbortHandler()
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


class TraceClient: QObject
{
  Q_OBJECT

public:
    TraceClient( QObject& parent);
    virtual ~TraceClient();

signals:
    void sendText(const Trace::TraceMessage&);
    void sendText(const QString&);
    void updateFilter(const Trace::TraceFilterMap&);
    void updateCallback(const Trace::TraceCallbackMap&);

public slots:
    void connectToHost( const QString &host, Trace::TracePort port );
    void disconnect();
    void setFilter(const Trace::TraceFilterMap&);
    void setCallback(const Trace::TraceCallbackMap&);
    void readyRead(TCClientSocket*);
    void rcvSocketText(const QString& str);

private:
    void sendFilter();
    QObject& moParent;
    TCClientSocket *mpoClientSocket;
    Trace::TracePort mTraceClientPort;
};




#endif

