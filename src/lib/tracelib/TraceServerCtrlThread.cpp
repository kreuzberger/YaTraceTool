#include "TraceServerCtrlThread.h"
#include "TraceServerCtrl.h"
#include "NTrace.h"
#include <QtCore/QDataStream>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTimer>

QTcpSocket* TraceServerCtrlThread::mpSocket = 0;
bool TraceServerCtrlThread::mbConnected( false );

TraceServerCtrlThread::TraceServerCtrlThread()
: QThread()
, mbTerminate( false )
, mMessages()
, mbSendFilter(false)
, miBlockSize( 0 )
, miTimerID(0)
{

}


TraceServerCtrlThread::~TraceServerCtrlThread()
{
  deleteSocket();
}

void
TraceServerCtrlThread::init()
{
}

bool TraceServerCtrlThread::checkConnection( )
{
  if( (0 == mpSocket) )
  {
    mpSocket = new QTcpSocket();
    //mpSocket->setBlocking(true);
    connect(mpSocket,SIGNAL(connected()), this, SLOT(connected()));
    connect(mpSocket,SIGNAL(disconnected()), this, SLOT(disconnected()));
    if( 0 == miTimerID )
    {
      miTimerID = startTimer(100);
    }
  }

  if ( 0 != mpSocket )
  {
    if( !mbConnected && (QAbstractSocket::ConnectingState != mpSocket->state()))
    {
      //qDebug() << "connecting to " << QHostAddress(0x7f000001) <<":" << Trace::TraceServer_Port;
      //qDebug() << "connection state" << mpSocket->state();
      mpSocket->connectToHost(QHostAddress(0x7f000001), Trace::TraceServer_Port );
    }
    if( (mbConnected) && (!mbTerminate))
    {
      connect(mpSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    }
  }

  return mbConnected;
}

void TraceServerCtrlThread::deleteSocket( void )
{
  mCriticalSection.lock();
  if( 0 != mpSocket)
  {
    if( mpSocket->isValid())
    {
      mpSocket->flush();
      mpSocket->reset();
      mpSocket->close();
    }
    if( 0 != mpSocket)
    {
      delete mpSocket;
      mpSocket = 0;
    }
  }
  mCriticalSection.unlock();
}

void TraceServerCtrlThread::disconnect()
{
  if( mbConnected)
  {
    // close connection
    if( 0 != mpSocket )
    {
      mpSocket->disconnectFromHost();
    }
  }
  mbConnected = false;

}

bool TraceServerCtrlThread::writeToSocket()
{
  bool bOk = true;

  if( (0 != mpSocket) && ( mpSocket->isValid()) )
  {
    mCriticalSection.lock();

    QByteArray outStream;
    QDataStream dataStream(&outStream, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_4_6);
    dataStream << (qint32) 0;


    if(mbSendFilter)
    {
      if ( (Trace::serialize(dataStream,TraceServerCtrl::mName,TraceServerCtrl::mPID)) &&
           (Trace::serialize(dataStream,TraceServerCtrl::mFilterList)) &&
           (Trace::serialize(dataStream,*TraceServerCtrl::mpCallbackMap)) )
      {
        mbSendFilter = false;
      }
    }

    if( Trace::serialize(dataStream,mMessages))
    {
      mMessages.clear();
    }

    if( 0 < outStream.size() )
    {
      dataStream.device()->seek(0);
      dataStream << (qint32) (outStream.size()-sizeof(qint32));
      int iWrote = 0;
      int iWritePos = 0;
      do
      {
        iWrote = 0;
        if( 0 != mpSocket )
        {
          iWrote = mpSocket->write(outStream.data() + iWritePos, outStream.size()-iWritePos);

        }
        iWritePos += iWrote;
      }
      while(  (0 < iWrote ) && // could write to socket
              (!mbTerminate) && //  not terminated
              ( iWritePos < (int)outStream.size() )  ); // something to write
      if(!mbTerminate) mpSocket->waitForBytesWritten();

      if( 0 >= iWrote )
      {
        bOk = false;
        disconnect();
      }
    }

    mCriticalSection.unlock();
  }

  return bOk;
}


void TraceServerCtrlThread::run()
{
  while(0 == qApp)
  {
    msleep(20);
  }
  connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(aboutToQuit()));
  //qDebug() << "thread started, waiting for connection";
  QTimer::singleShot(100,this,SLOT(doProcess()));
  exec();
}

void TraceServerCtrlThread::timerEvent(QTimerEvent* )
{
  doProcess();
}


void TraceServerCtrlThread::doProcess()
{
  if(checkConnection())
  {
    if(!mbSendFilter && mMessages.isEmpty())
    {
    }
    else if(!mbTerminate)
    {
      if(false == writeToSocket())
      {
        deleteSocket();
      }
    }
  }
}


void TraceServerCtrlThread::sendFilter()
{
  mbSendFilter = true;
  //messageReceived.wakeAll();
}


void TraceServerCtrlThread::connected()
{
  //qDebug() << "app on port" << mpSocket->peerPort() << "connected()";
  mbConnected = true;
  sendFilter();
  miBlockSize = 0;
}


void TraceServerCtrlThread::disconnected()
{
  //qDebug() << "app disconnected()";
  if( mbConnected)
  {
    mbSendFilter = true;
    miBlockSize = 0;
  }
  mbConnected = false;
  if( 0 != miTimerID )
  {
    killTimer(miTimerID);
  }
}

void TraceServerCtrlThread::quit()
{
  mbTerminate = true;
  deleteSocket();
  exit(0);
}

void TraceServerCtrlThread::aboutToQuit()
{
  mbTerminate = true;
  deleteSocket();
  exit(0);
}

void TraceServerCtrlThread::readyRead()
{
  if(!mbTerminate && checkConnection())
  {
    if( 0 != mpSocket )
    {
      Trace::TraceFilter oFilterList;
      Trace::TraceCallback oCallback;
      QStringList strList;

      QDataStream dsRcv(mpSocket);
      dsRcv.setVersion(QDataStream::Qt_4_6);

      do
      {
        if( 0 == miBlockSize )
        {
          if(mpSocket->bytesAvailable() < (int)sizeof(qint32))
          {
            return;
          }
          else
          {
            dsRcv >> miBlockSize;
            //qDebug() << "TraceServerCtrlRcvThread next block " << miBlockSize << "bytes" ;
          }
        }

        if(mpSocket->bytesAvailable() >= miBlockSize)
        {
          //qDebug() << "TraceServerCtrlRcvThread received " << miBlockSize << "bytes" ;
          QByteArray byteArray = mpSocket->read(miBlockSize);
          QDataStream ds(&byteArray,QIODevice::ReadOnly);
          miBlockSize = 0;

          while(Trace::TraceFmt_Invalid != Trace::deserialize(ds,oFilterList,strList,oCallback));

          for( Trace::TraceFilter::iterator it = oFilterList.begin(); it != oFilterList.end(); ++it)
          {
            TraceServerCtrl::mFilterList[it.key()]= it.value();
          }

          for( Trace::TraceCallback::iterator it = oCallback.begin(); it != oCallback.end(); ++it)
          {
            QString callbackScope = it.key().section('_',0,0);
            QString callbackFunc = it.key().section('_',1,1);
            QStringList arg = it.value();
            if( !callbackScope.isEmpty() && !callbackFunc.isEmpty())
            {
              TraceServerCtrl::getInstance()->call(callbackScope,callbackFunc,arg);

            }// name contains Scope and callback
          }
        }
      }while(mpSocket->bytesAvailable());
    }
  }
}
