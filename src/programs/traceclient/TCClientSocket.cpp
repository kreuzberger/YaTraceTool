#include "TCClientSocket.h"
#include <assert.h>
TCClientSocket::TCClientSocket( int port, QObject *parent )
: QTcpSocket( parent )
, miBlockSize(0)
{
	line = 0;
  mConnectPort = port;
  connect( this, SIGNAL(connected()), this,SLOT(socketConnected()) );
  connect( this, SIGNAL(disconnected()),this,SLOT(socketConnectionClosed()) );
  connect( this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)) );
  connect( this, SIGNAL(delayedCloseFinished()), SLOT(socketClosed()) );
  connect( this, SIGNAL(readyRead()), this, SLOT(readyRead()) );
  connect( this, SIGNAL(readyRead(TCClientSocket*)), parent, SLOT(readyRead(TCClientSocket*)) );
  connect(this, SIGNAL(sendText(const QString&)),parent, SLOT(rcvSocketText(const QString&)));
}

TCClientSocket::~TCClientSocket()
{
}

void TCClientSocket::socketConnected()
{
  emit sendText(QString("TraceClient connected to port %1").arg(peerPort()));
}

void TCClientSocket::socketConnectionClosed()
{
  emit sendText(QString("TraceServer closed connection on port %1").arg(peerPort()));
}

void TCClientSocket::socketError(QAbstractSocket::SocketError error)
{
  emit sendText(QString("TraceClient error %1 on port %2: %3").arg(error).arg(peerPort()).arg(errorString()));
}

void TCClientSocket::socketClosed()
{
  emit sendText(QString("TraceClient closed on port %1").arg(peerPort()));
}

void TCClientSocket::readyRead()
{
  QDataStream ds(this);
  ds.setVersion(QDataStream::Qt_4_6);

  do
  {
    if( 0 == miBlockSize )
    {
      if(bytesAvailable() < (int)sizeof(qint32))
      {
        return;
      }
      else
      {
        ds >> miBlockSize;
      }
    }

    assert( 0 != miBlockSize );
    if(bytesAvailable() >= miBlockSize)
    {
      emit readyRead(this);
      miBlockSize = 0;
    }
  } while(bytesAvailable());
}
