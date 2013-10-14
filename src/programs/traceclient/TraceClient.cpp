/****************************************************************************
** $Id: TraceClient.cpp,v 1.2 2011-06-16 06:59:03 kreuzberger Exp $
**
** Copyright (C) 1992-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "TraceClient.h"

#include <qstring.h>
#include <q3socket.h>
#include <qobject.h>
#include <qmap.h>
//Added by qt3to4:
#include <QDataStream>
#include <QByteArray>
#include "lib/tracelib/NTrace.h"
#include <assert.h>

TraceClient::TraceClient( QObject& parent)
: moParent(parent) 
, mpoClientSocket(0) 
, mTraceClientPort(Trace::TraceClient_Port)
{
  connect(&moParent, SIGNAL(connectToHost(const QString&, Trace::TracePort )), this, SLOT(connectToHost( const QString&, Trace::TracePort)));
  connect(&moParent, SIGNAL(closeConnection()), this, SLOT(disconnect()));
  connect(&moParent, SIGNAL(setFilter(const Trace::TraceFilterMap&)), this, SLOT(setFilter(const Trace::TraceFilterMap&)));
  connect(&moParent, SIGNAL(setCallback(const Trace::TraceCallbackMap&)), this, SLOT(setCallback(const Trace::TraceCallbackMap&)));
  connect( this, SIGNAL(sendText(const Trace::TraceMessage &)), &moParent, SLOT(rcvText(const Trace::TraceMessage&)));
  connect( this, SIGNAL(sendText(const QString &)), &moParent, SLOT(rcvText(const QString&)));
  connect( this, SIGNAL(updateFilter(const Trace::TraceFilterMap&)), &moParent, SLOT(updateFilter(const Trace::TraceFilterMap&)));
  connect( this, SIGNAL(updateCallback(const Trace::TraceCallbackMap&)), &moParent, SLOT(updateCallback(const Trace::TraceCallbackMap&)));

  if( 0 == mpoClientSocket)
  {
    mpoClientSocket =  new TCClientSocket(mTraceClientPort,this) ;
  }
}

TraceClient::~TraceClient()
{
  disconnect();

  if( 0 != mpoClientSocket )
  {
    delete mpoClientSocket;
    mpoClientSocket = 0;
  }
}

  
void TraceClient::connectToHost( const QString &host, Trace::TracePort /*port*/ )
{
// create the socket and connect various of its signals
  if( (0 != mpoClientSocket) && (QAbstractSocket::ConnectedState != mpoClientSocket->state()))
  {
    mpoClientSocket->connectToHost(host,mTraceClientPort);
    mpoClientSocket->waitForConnected(1000);
  }
}
    
void TraceClient::disconnect()
{
  if( 0 != mpoClientSocket )
  {
    //mpoClientSocket->close();
    mpoClientSocket->abort();

    emit sendText(QString("Traceclient disconnected"));
  }
}



void TraceClient::readyRead(TCClientSocket* socket)
{
  if(0 != socket)
  {
    if(mTraceClientPort == socket->getConnectPort())
    {
      Trace::TraceFilterMap filter;
      Trace::TraceMessage message;
      Trace::TraceCallbackMap callback;
      QByteArray byteArray = socket->read(socket->getBlockSize());
      QDataStream ds(&byteArray,QIODevice::ReadOnly);
      unsigned int uiRet = Trace::deserialize(ds,filter, message, callback);

      if( uiRet & Trace::TraceFmt_FilterMap)
      {
          qDebug() << "traceclient: new filter map received";
          emit updateFilter(filter);
      }
      if( uiRet & Trace::TraceFmt_Message)
      {
        emit sendText(message);
      }
      if ( uiRet & Trace::TraceFmt_CallbackMap)
      {
        qDebug() << "traceclient: new callback map received";
        emit updateCallback(callback);
      }
    }
  }
}  

void TraceClient::setFilter(const Trace::TraceFilterMap& rFilter )
{
  if( 0 != mpoClientSocket )
  {
    QByteArray outStream;
    QDataStream ds(&outStream, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_6);
    ds << (qint32)0;
    if( Trace::serialize(ds,rFilter) )
    {
      ds.device()->seek(0);
      ds << (qint32) (outStream.size()-sizeof(qint32));
      const qint64 iBytesWritten = mpoClientSocket->write(outStream.data(), outStream.size());
      (void) iBytesWritten;
      assert(iBytesWritten == outStream.size());
      mpoClientSocket->flush();
    }
  }

}

void TraceClient::setCallback(const Trace::TraceCallbackMap& rCallback )
{
  if( 0 != mpoClientSocket )
  {
    QByteArray outStream;
    QDataStream ds(&outStream, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_6);
    ds << (qint32)0;
    if(Trace::serialize(ds,rCallback))
    {
      ds.device()->seek(0);
      ds << (qint32) (outStream.size()-sizeof(qint32));
      const qint64 iBytesWritten = mpoClientSocket->write(outStream.data(), outStream.size());
      (void) iBytesWritten;
      assert(iBytesWritten == outStream.size());
      mpoClientSocket->flush();
    }
  }

}


void TraceClient::rcvSocketText(const QString& str)
{
  emit sendText(str);
}




