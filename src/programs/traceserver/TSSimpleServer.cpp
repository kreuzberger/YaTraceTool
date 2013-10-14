#include "TSSimpleServer.h"
#include "TSClientSocket.h"
//Added by qt3to4:
#include <QDataStream>
#include <assert.h>

TSSimpleServer::TSSimpleServer( QObject* parent, Trace::TracePort port )
: QTcpServer( parent )
, mConnections()
{
  if ( !listen(QHostAddress::LocalHost,port) )
  {
    qWarning("Failed to bind to port %d", port);
    exit(1);
  }

  connect(this,SIGNAL(newConnect(const Trace::TracePort&)),parent,SLOT(onConnect(const Trace::TracePort&)));
}

TSSimpleServer::~TSSimpleServer()
{
  for(TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    delete (it.value());
  }

}

void TSSimpleServer::incomingConnection( int socket )
{
  if( mConnections.contains(socket))
  {
    qFatal("already connected to port %d", serverPort());
    // maybe close
    //if( QSocket::Idle != mpSocket->state() )
    //{
    //  mpSocket->clearPendingData();
    //  mpSocket->close();
    //}
    //delete mpSocket;
    //mpSocket = 0;
  }
  else
  {
    qDebug("new client connection");
    mConnections[socket] = new TSClientSocket( serverPort(), socket, this );
    emit newConnect( serverPort() );
  }
}


void TSSimpleServer::closedConnection( const int& socket )
{
  if( mConnections.contains(socket) )
  {
    //TSClientSocket* pClientSocket = mConnections.take(socket);
    mConnections.remove(socket);
   // delete pClientSocket;
  }
}

void TSSimpleServer::sendMessage(const Trace::TraceMessage& rMessage)
{
  for (TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    TSClientSocket* psClient = it.value();
    if( 0 != psClient)
    {
      QByteArray outStream;
      QDataStream ds(&outStream, QIODevice::WriteOnly);
      ds.setVersion(QDataStream::Qt_4_6);
      ds << (qint32) 0;
      if(Trace::serialize(ds,rMessage))
      {
        ds.device()->seek(0);
        ds << (qint32) (outStream.size()-sizeof(qint32));
        const qint64 iBytesWritten = psClient->write(outStream.data(),outStream.size());
        (void) iBytesWritten;
        assert(iBytesWritten == outStream.size());
        psClient->flush();
      }
    }
  }
}


void TSSimpleServer::sendFilterMap(const Trace::TraceFilterMap& rFilter)
{
  for (TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    TSClientSocket* psClient = it.value();
    if( 0 != psClient)
    {
      QByteArray outStream;
      QDataStream ds(&outStream, QIODevice::WriteOnly);

      ds.setVersion(QDataStream::Qt_4_6);
      ds << (qint32) 0;
      if(Trace::serialize(ds,rFilter))
      {
        ds.device()->seek(0);
        ds << (qint32) (outStream.size()-sizeof(qint32));
        const qint64 iBytesWritten = psClient->write(outStream.data(),outStream.size());
        (void) iBytesWritten;
        assert(iBytesWritten == outStream.size());
        psClient->flush();
        //qDebug() << "send " << iBytesWritten << " bytes FilterMap to client on port " << psClient->peerPort() << "successfull" << bFlushed;

      }
    }
  }

}


void TSSimpleServer::sendCallback(const Trace::TraceCallbackMap& rCallback)
{
  for (TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    TSClientSocket* psClient = it.value();
    if( 0 != psClient)
    {
      QByteArray outStream;
      QDataStream ds(&outStream, QIODevice::WriteOnly);

      ds.setVersion(QDataStream::Qt_4_6);
      ds << (qint32) 0;
      if(Trace::serialize(ds,rCallback))
      {
        ds.device()->seek(0);
        ds << (qint32) (outStream.size()-sizeof(qint32));
        const qint64 iBytesWritten = psClient->write(outStream.data(),outStream.size());
        (void) iBytesWritten;
        assert(iBytesWritten == outStream.size());
        psClient->flush();
        //qDebug() << "send " << iBytesWritten << " bytes CallbackMap to client on port " << psClient->peerPort() << "successfull" << bFlushed;
      }
    }
  }

}



void TSSimpleServer::sendFilter(const Trace::TracePID& pid, const Trace::TraceFilter& rFilter)
{
  for (TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    TSClientSocket* psClient = it.value();
    if( 0 != psClient)
    {
      if( pid == psClient->getPID() )
      {
        //qDebug() << "pid " << pid << "matched to attached client on port " << psClient->peerPort();
        QByteArray outStream;
        QDataStream ds(&outStream, QIODevice::WriteOnly);

        ds.setVersion(QDataStream::Qt_4_6);
        ds << (qint32) 0;
        if(Trace::serialize(ds,rFilter))
        {
          ds.device()->seek(0);
          ds << (qint32) (outStream.size()-sizeof(qint32));
          const qint64 iBytesWritten = psClient->write(outStream.data(),outStream.size());
          (void) iBytesWritten;
          assert(iBytesWritten == outStream.size());
          psClient->flush();
          //qDebug() << "send " << iBytesWritten << " bytes to client on port " << psClient->peerPort() << "successfull" << bFlushed;
        }
      };
    }
  }
}


void TSSimpleServer::sendCallback(const Trace::TracePID& pid, const Trace::TraceCallback& rCallback)
{
  for (TSClientMap::iterator it = mConnections.begin(); it != mConnections.end(); ++it)
  {
    TSClientSocket* psClient = it.value();
    if( 0 != psClient)
    {
      if( pid == psClient->getPID() )
      {
        QByteArray outStream;
        QDataStream ds(&outStream, QIODevice::WriteOnly);

        ds.setVersion(QDataStream::Qt_4_6);
        ds << (qint32) 0;
        if(Trace::serialize(ds,rCallback))
        {
          ds.device()->seek(0);
          ds << (qint32) (outStream.size()-sizeof(qint32));
          const qint64 iBytesWritten = psClient->write(outStream.data(),outStream.size());
          (void) iBytesWritten;
          assert(iBytesWritten == outStream.size());
          psClient->flush();
        }
      };
    }
  }
}

