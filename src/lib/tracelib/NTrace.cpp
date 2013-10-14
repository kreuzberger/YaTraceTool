#include "NTrace.h"
#include <QDataStream>
#include <qstringlist.h>
#include <assert.h>

unsigned int Trace::deserialize(QDataStream& ds, Trace::TraceFilterMap& rFilter, Trace::TraceMessage& rStrList, Trace::TraceCallbackMap& rCallback)
{
  unsigned int uiRet = Trace::TraceFmt_Invalid;

  unsigned int uiFmt;
  QString key;
  int data;
  QString name;
  QString nameCallbackMap;

  while(!ds.atEnd())
  {
   // qDebug() << "bytes avail: " << ds.device()->bytesAvailable();
    ds >> uiFmt;

    if( Trace::TraceFmt_Invalid == uiFmt)
    {
      assert(0);
      break;
    }
    else if( Trace::TraceFmt_Filter == uiFmt )
    {
      if(!name.isEmpty())
      {
        ds >> key;
        ds >> data;
        rFilter[name].insert(key,data);
        uiRet |= Trace::TraceFmt_Filter;
      }
    }
    else if( Trace::TraceFmt_Message == uiFmt )
    {
      QString str;
      ds >> str;
      rStrList += str;
      uiRet |= Trace::TraceFmt_Message;
    }
    else if( Trace::TraceFmt_FilterMap == uiFmt )
    {
      ds >> name;
      uiRet |= Trace::TraceFmt_FilterMap;
    }
    else if ( Trace::TraceFmt_CallbackMap == uiFmt )
    {
      ds >> nameCallbackMap;
      uiRet |= Trace::TraceFmt_CallbackMap;
    }
    else if ( Trace::TraceFmt_Callback == uiFmt )
    {
      QString nameCallback;
      unsigned int argCount;
      ds >> nameCallback;
      ds >> argCount;
      rCallback[nameCallbackMap].insert(nameCallback,QStringList());
      uiRet |= Trace::TraceFmt_Callback;
      //ds.readLine();

      for( unsigned int idx=0;idx<argCount;idx++)
      {
        ds >> uiFmt;
        if( Trace::TraceFmt_CallbackArg == uiFmt )
        {
          // leerzeichen ueberlesen
          //ds.skipWhiteSpace();
          QString str;
          ds >> str;
          rCallback[nameCallbackMap][nameCallback].append(str);
          uiRet |= Trace::TraceFmt_CallbackArg;
        }
        else
        {
          uiRet = Trace::TraceFmt_Invalid;
          assert(0);
          break;
        }
      }
    }
    else
    {
      assert(0);
      break;

    }
  }

  return uiRet;
}



unsigned int Trace::deserialize(QDataStream& ds, Trace::TraceFilter& rFilter, Trace::TraceMessage& rStrList, Trace::TraceCallback& rCallback)
{
  unsigned int uiRet = Trace::TraceFmt_Invalid;
  unsigned int uiFmt;
  QString key;
  int data;
  unsigned int count;
  QString str;

  while(!ds.atEnd())
  {
    ds >> uiFmt;
    if( Trace::TraceFmt_Filter == uiFmt )
    {
      ds >> key;
      ds >> data;

      rFilter[key]= data;
      uiRet |= Trace::TraceFmt_Filter;
    }
    else if( Trace::TraceFmt_Message == uiFmt )
    {
      ds >> str;
      rStrList += str;
      uiRet |= Trace::TraceFmt_Message;
    }
    else if (Trace::TraceFmt_Callback == uiFmt )
    {
      ds >> key;
      rCallback[key]= QStringList();
      ds >> count;
      // now read until end of line reached
      //ds.readLine();
      uiRet |= Trace::TraceFmt_Callback;
      for( unsigned int idx = 0; idx < count; idx++)
      {
        ds >> uiFmt;
        if( Trace::TraceFmt_CallbackArg == uiFmt)
        {
          // leerzeichen ueberlesen
          //ds.skipWhiteSpace();
          QString line;
          ds >> line;

          rCallback[key].append(line);
          uiRet |= Trace::TraceFmt_CallbackArg;
        }
      }
    }
    else if (Trace::TraceFmt_Invalid == uiFmt)
    {
      uiRet |= uiFmt;
      assert(0);
      break;
    }
  }
  return uiRet;
}

bool Trace::serialize(QDataStream& dataStream, const Trace::TraceFilter& filter )
{
  bool bRet = true;
  if( !filter.isEmpty())
  {
    for(Trace::TraceFilter::const_iterator it = filter.begin();it != filter.end();++it)
    {
     dataStream << Trace::TraceFmt_Filter << it.key() << it.value();
    }
  }

  return bRet;
}


bool Trace::serialize(QDataStream& dataStream, const Trace::TraceFilterMap& filter )
{
  bool bRet = false;
  if( !filter.isEmpty())
  {
    for(Trace::TraceFilterMap::const_iterator it = filter.begin();it != filter.end();++it)
    {
     dataStream << Trace::TraceFmt_FilterMap << it.key();
     Trace::serialize(dataStream,it.value());
    }
    bRet = true;
  }
  else
  {
    dataStream << Trace::TraceFmt_FilterMap;
    bRet = true;
  }

  return bRet;
}



bool Trace::serialize(QDataStream& dataStream, const QString& name, const unsigned int& pid )
{
  bool bRet = false;
  if( !name.isEmpty())
  {
    dataStream << Trace::TraceFmt_PID << name << pid;
    bRet = true;
  }

  return bRet;
}

bool Trace::serialize(QDataStream& dataStream, const Trace::TraceMessage& message )
{
  bool bRet = false;
  for(QStringList::const_iterator it = message.begin(); it != message.end(); ++it)
  {
    dataStream << Trace::TraceFmt_Message << (*it);
    bRet = true;
  }

  return bRet;
}


unsigned int Trace::deserialize(QDataStream& dataStream, QString& name, unsigned int& pid )
{
  unsigned int uiRet = TraceFmt_Invalid;
  dataStream >> uiRet;
  if( Trace::TraceFmt_PID == uiRet)
  {
    dataStream >> name;
    dataStream >> pid;
  }
  else
  {
    assert(0);
  }

  return uiRet;
}


unsigned int Trace::deserialize(QDataStream& dataStream, Trace::TraceCallback& callback )
{

  unsigned int uiRet = Trace::TraceFmt_Invalid;
  QString name;
  unsigned int uiFmt;
  unsigned int count;
  QString str;

  dataStream >> uiFmt;
  if( Trace::TraceFmt_Callback == uiFmt)
  {
    dataStream >> name;
    dataStream >> count;
    uiRet |= Trace::TraceFmt_Callback;
    for(unsigned int idx=0; idx < count; idx++)
    {
      dataStream >> uiFmt;
      if( Trace::TraceFmt_CallbackArg == uiFmt)
      {
        // leerzeichen ueberlesen
        //dataStream.skipWhiteSpace();
        dataStream >> str;
        callback[name].append(str);
        uiRet |= Trace::TraceFmt_CallbackArg;
      }
      else
      {
        assert(0);
        break;
        uiRet = Trace::TraceFmt_Invalid;
      }
    }
  }

  return uiRet;

}


bool Trace::serialize(QDataStream& dataStream ,const Trace::TraceCallback& callback)
{
  bool bRet = false;
  if(!callback.isEmpty())
  {
    for(Trace::TraceCallback::const_iterator itMap = callback.begin(); itMap != callback.end(); ++itMap)
    {
      dataStream << Trace::TraceFmt_Callback << (itMap.key()) << itMap.value().count();
      for(QStringList::const_iterator it = itMap.value().begin(); it != itMap.value().end(); ++it)
      {
        dataStream << Trace::TraceFmt_CallbackArg <<  (*it);
      }
      bRet = true;
    }
  }
  else
  {
    bRet = true;
  }
  return bRet;

}


bool Trace::serialize(QDataStream& dataStream ,const Trace::TraceCallbackMap& callback)
{
  bool bRet = false;
  if( !callback.isEmpty())
  {
    for(Trace::TraceCallbackMap::const_iterator it = callback.begin(); it != callback.end(); ++it)
    {
       dataStream << Trace::TraceFmt_CallbackMap << it.key();
       bRet = Trace::serialize(dataStream,it.value());
    }
  }
  else
  {
    dataStream << Trace::TraceFmt_CallbackMap;
    bRet = true;
  }


  return bRet;

}


