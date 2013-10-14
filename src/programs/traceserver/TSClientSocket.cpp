#include "TSClientSocket.h"


TSClientSocket::TSClientSocket( Trace::TracePort port, int socket,QObject *parent)
: QTcpSocket( parent)
, mPort(port)
, mName()
, mPID(-1)
, mTracePID()
, mID( socket )
, miBlockSizeSrv(0)
, miBlockSizeClient(0)
{
  if( 0 != parent)
  {
    connect( this, SIGNAL(disconnected()),this, SLOT(connectionClosed()) );
    connect( this ,SIGNAL(setText(const QString&)),parent->parent(),SLOT(setText(const QString&)));
    connect( this ,SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect( this ,SIGNAL(rcvFilter(const TSClientSocket&, const Trace::TracePID&, const Trace::TraceFilter&)),parent->parent(),SLOT(rcvFilter(const TSClientSocket&,const Trace::TracePID&, const Trace::TraceFilter&)));
    connect( this ,SIGNAL(rcvMessage(const TSClientSocket&, const Trace::TraceMessage&)),parent->parent(),SLOT(rcvMessage(const TSClientSocket&,const Trace::TraceMessage&)));
    connect( this ,SIGNAL(rcvCallback(const TSClientSocket&, const Trace::TracePID&,const Trace::TraceCallback&)),parent->parent(),SLOT(rcvCallback(const TSClientSocket&, const Trace::TracePID&, const Trace::TraceCallback&)));
    connect( this ,SIGNAL(clearFilter(const TSClientSocket& , const Trace::TracePID& )),parent->parent(),SLOT(clearFilter(const TSClientSocket& , const Trace::TracePID& )));
    connect( this ,SIGNAL(closedConnection(const int&)),parent,SLOT(closedConnection(const int&) ));

    setSocket(socket);

    if( Trace::TraceServer_Port == mPort)
    {
      emit setText(QString("new app connection on port %1 from %2").arg(mPort).arg(peerPort()));
    }
    else if( Trace::TraceClient_Port == mPort)
    {
      emit setText(QString("new traceclient connection on port %1").arg(mPort));
    }
    else
    {
      emit setText(QString("new UNKNOWN connection on port %1 from %2").arg(mPort).arg(peerPort()));
    }
  }
}

TSClientSocket::~TSClientSocket()
{
}

void TSClientSocket::connectionClosed()
{
    if( Trace::TraceServer_Port == mPort)
    {
      emit setText(QString("closed app connection on port %1").arg(mPort));
      emit clearFilter(self(), mTracePID);
      emit closedConnection(mID);
    }
    else if( Trace::TraceClient_Port == mPort)
    {
      emit setText(QString("closed traceclient connection on port %1").arg(mPort));
      emit closedConnection(mID);
    }
    else
    {
      emit setText(QString("closed UNKNOWN connection on port %1").arg(mPort));
    }

    deleteLater();
}

void TSClientSocket::readyRead()
{
  switch(mPort)
  {
  case Trace::TraceServer_Port:
    {
      readyReadSrv();
    }
    break;
  case Trace::TraceClient_Port:
    {
      readyReadClient();
    }
    break;
  default:
      emit setText(QString("UNKNOWN ready read on port %1").arg(mPort));
    break;

  }
}

bool TSClientSocket::havePID()
{
  return ( (!mName.isEmpty()));
}

void TSClientSocket::readyReadSrv()
{
  QDataStream ds(this);
  ds.setVersion(QDataStream::Qt_4_6);

  do
  {
    if( 0 == miBlockSizeSrv )
    {
      if(bytesAvailable() < (int)sizeof(qint32))
      {
        return;
      }
      else
      {
        ds >> miBlockSizeSrv;
      }
    }

    if(bytesAvailable() >= miBlockSizeSrv)
    {
      miBlockSizeSrv = 0;

      if(!havePID())
      {
        unsigned int uiFmt = Trace::deserialize(ds,mName,mPID);
        if( uiFmt & Trace::TraceFmt_PID )
        {
          emit setText(QString("received app PID %1(%2)").arg(mName).arg(mPID));
          mTracePID = QString("%1(%2)").arg(mName).arg(mPID);
        }
      }

      if(havePID())
      {
        Trace::TraceFilter filter;
        Trace::TraceMessage message;
        Trace::TraceCallback callback;
        unsigned int uiRet = Trace::deserialize(ds,filter,message,callback);

        if(uiRet & Trace::TraceFmt_Filter)
        {
          emit setText(QString("received app filter on port %1").arg(mPort));
          emit rcvFilter(self(), mTracePID, filter);
        }
        if(uiRet & Trace::TraceFmt_Message)
        {
          emit setText(QString("received app trace on port %1").arg(mPort));
          emit rcvMessage(self(),message);
        }
        if(uiRet & Trace::TraceFmt_Callback)
        {
          emit setText(QString("received app callback on port %1").arg(mPort));
          emit rcvCallback(self(),mTracePID,callback);
        }

      }
    }
  }while(bytesAvailable());
}


void TSClientSocket::readyReadClient()
{
  QDataStream dsBlock(this);
  dsBlock.setVersion(QDataStream::Qt_4_6);

  do
  {
    if( 0 == miBlockSizeClient )
    {
      if(bytesAvailable() < (int)sizeof(qint32))
      {
        return;
      }
      else
      {
        dsBlock >> miBlockSizeClient;
        emit setText(QString("received traceclient update, size %1 ").arg(miBlockSizeClient));
      }
    }
    if(( 0 != miBlockSizeClient) && (bytesAvailable() >= miBlockSizeClient))
    {
      Trace::TraceFilterMap oFilterMap;
      Trace::TraceMessage oMessage;
      Trace::TraceCallbackMap oCallbackMap;
      QByteArray byteArray = read(miBlockSizeClient);
      miBlockSizeClient = 0;
      QDataStream ds(&byteArray,QIODevice::ReadOnly);
      unsigned int uiFmt = Trace::deserialize(ds,oFilterMap,oMessage, oCallbackMap);
      if( uiFmt != Trace::TraceFmt_Invalid )
      {
        if( 0 < oFilterMap.count() )
        {
          emit setText(QString("received traceclient filter "));
          for(Trace::TraceFilterMap::iterator it= oFilterMap.begin();  it != oFilterMap.end(); ++it)
          {
            emit rcvFilter(self(),it.key(),it.value());
          }
        }
        else if ( 0 < oCallbackMap.count())
        {
          emit setText(QString("received traceclient callback "));
          for(Trace::TraceCallbackMap::iterator it= oCallbackMap.begin();  it != oCallbackMap.end(); ++it)
          {
            emit rcvCallback(self(),it.key(),it.value());
          }
        }
      }
    }
  }while(bytesAvailable());
}

Trace::TracePort TSClientSocket::port() const
{
  return mPort;
}


