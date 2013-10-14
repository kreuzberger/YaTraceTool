#ifndef TOOLS_TRACETOOL_TSSIMPLESERVER_H
#define TOOLS_TRACETOOL_TSSIMPLESERVER_H

#include <QtNetwork/QTcpServer>
#include <QtCore/QDataStream>
//#include <stdlib.h>
#include "lib/tracelib/NTrace.h"

class TSClientSocket;
class QTcpSocket;

//typedef QMap<Trace::TracePort,TSClientSocket*> TSClientMap;
typedef QMap<int,TSClientSocket*> TSClientMap;


class TSSimpleServer : public QTcpServer
{
  Q_OBJECT
public:
  TSSimpleServer( QObject* parent, Trace::TracePort port );
  virtual ~TSSimpleServer();

  void sendMessage(const Trace::TraceMessage& );
  void sendFilterMap(const Trace::TraceFilterMap& rFilter);
  void sendFilter(const Trace::TracePID&, const Trace::TraceFilter& rFilter);
  void sendCallback(const Trace::TracePID&, const Trace::TraceCallback&);
  void sendCallback(const Trace::TraceCallbackMap&);

signals:
    void newConnect( const Trace::TracePort& );
    void readyRead(QTcpSocket* );
    void rcvFilter(QTcpSocket*);
    void rcvCallback(QTcpSocket*);

public slots:
     void closedConnection( const int& socket );

protected:
     virtual void incomingConnection( int socket );

private:
    TSClientMap mConnections;

};


#endif
