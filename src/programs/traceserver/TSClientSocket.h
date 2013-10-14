#ifndef TOOLS_TRACETOOL_TSCLIENTSOCKET_H
#define TOOLS_TRACETOOL_TSCLIENTSOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QtCore/QDataStream>

#include <stdlib.h>
#include "lib/tracelib/NTrace.h"

class TSClientSocket : public QTcpSocket
{
  Q_OBJECT
public:
  TSClientSocket( Trace::TracePort port, int socket, QObject *parent=0 );
  virtual ~TSClientSocket();

  Trace::TracePort port() const ;
  const Trace::TracePID& getPID();

protected slots:
  void connectionClosed();
  void readyRead();


signals:
  void setText( const QString& );
  void rcvFilter( const TSClientSocket&, const Trace::TracePID&, const Trace::TraceFilter& );
  void rcvMessage( const TSClientSocket&, const Trace::TraceMessage& );
  void rcvCallback( const TSClientSocket&, const Trace::TracePID&, const Trace::TraceCallback& );
  void clearFilter( const TSClientSocket& , const Trace::TracePID& );
  void closedConnection( const int& socket );

private:
  TSClientSocket& self(){return (*this);}
  bool havePID( );
  void readyReadSrv();
  void readyReadClient();

  Trace::TracePort mPort;
  QString mName;
  unsigned int mPID;
  Trace::TracePID mTracePID;
  int mID;
  qint32 miBlockSizeSrv;
  qint32 miBlockSizeClient;
};

inline const Trace::TracePID&TSClientSocket::getPID()
{
  return mTracePID;
}

#endif
