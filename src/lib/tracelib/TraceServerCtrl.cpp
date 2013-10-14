#include "TraceServerCtrl.h"
#include <QWaitCondition>
#include <stdlib.h>

TraceServerCtrl* TraceServerCtrl::_instance = 0;
QString TraceServerCtrl::mName = "";
Trace::TraceFilter TraceServerCtrl::mFilterList;
bool TraceServerCtrl::mbDestroyed = false;
unsigned int TraceServerCtrl::mPID = 0;
Trace::TraceCallback* TraceServerCtrl::mpCallbackMap=0;


#if defined Q_OS_WIN
  #include <process.h>
#elif defined Q_OS_UNIX
  #include <sys/types.h>
  #include <unistd.h>
#endif

TraceServerCtrl::TraceServerCtrl()
{
  mpCallbackMap = new Trace::TraceCallback;
}

TraceServerCtrl::~TraceServerCtrl()
{
  terminate();
}

void TraceServerCtrl::terminate()
{
  QWaitCondition oWait;

  if(mThread.isRunning())
  {
    mThread.quit();
    while( !mThread.isFinished() )
    {
      QMutex mutex;
      mutex.lock();
      oWait.wait( &mutex, 1 );
      mutex.unlock();
    }
  }

  mFilterList.clear();
  if( 0 != mpCallbackMap )
  {
    mpCallbackMap->clear();
    delete mpCallbackMap;
    mpCallbackMap = 0;
  }
}


void TraceServerCtrl::sendScope(const QString& str)
{
  sendMessage(Trace::TraceID_EnterExit,QString("<scope>%1</scope>").arg(str));
}




void TraceServerCtrl::sendMessage(const Trace::TraceID& traceID, const QString& str)
{

  if(mThread.isRunning())
  {
    if(traceID & mSend)
    {
      mThread.appendMessage(mName,mPID,str);
    }
  }
}

void TraceServerCtrl::sendLog(const QString& str)
{
  sendMessage(Trace::TraceID_Log,QString("<log>%1</log>").arg(str));
}

void TraceServerCtrl::sendError(const QString& str)
{
  sendMessage(Trace::TraceID_Error,QString("<error>%1</error>").arg(str));
}

void TraceServerCtrl::sendWarning(const QString& str)
{
  sendMessage(Trace::TraceID_Warn,QString("<warning>%1</warning>").arg(str));
}




TraceServerCtrl* TraceServerCtrl::getInstance( )
{
  if (!_instance)
  {
    _instance = new TraceServerCtrl();
  }
  return _instance;
}

void TraceServerCtrl::destroy()
{
  if( 0 != _instance)
  {
    delete _instance;
    _instance = 0;
    mbDestroyed = true;
  }
}


void
TraceServerCtrl::setName( const QString& name)
{
  if( mName.isEmpty())
  {
    mThread.start();

    mName = name;
    mPID = getpid();
    QStringList lst = moNames.split("\n",QString::SkipEmptyParts);
    for(QStringList::Iterator it = lst.begin(); it != lst.end(); ++it)
    {
      // enable errors and warnings as default;
      if(!(*it).isEmpty())
      {
        mFilterList[*it] = Trace::TraceID_Warn + Trace::TraceID_Error;
      }
    }
    atexit(TraceServerCtrl::destroy);
    mThread.init();
    mThread.sendFilter();
  }
}

void
TraceServerCtrl::setScope( const QString& name)
{
  if( !name.isEmpty())
  {
    if( mFilterList.contains(name) )
    {
       mSend = mFilterList[name];
    }
  }
}

void
TraceServerCtrl::setScopeDef( const QString& str)
{
  moNames += str;
  moNames +="\n";
}

void
TraceServerCtrl::setCallbackDef( const QString& scope, const QString& name, TRACECALLBACK funcPtr, const QStringList& args )
{
  QString callback_name = scope;
  callback_name += "_";
  callback_name += name;
  mCallbackFuncList.insert(callback_name,funcPtr);
  if( 0 != mpCallbackMap )
  {
    mpCallbackMap->insert(callback_name,args);
  }
}


void
TraceServerCtrl::resetFilter()
{
  for(Trace::TraceFilter::iterator it = mFilterList.begin(); it != mFilterList.end(); ++it)
  {
    // enable errors and warnings as default;
    if( !it.key().isEmpty())
    {
      mFilterList[it.key()] = Trace::TraceID_Warn + Trace::TraceID_Error;
    }
  }
}

void
TraceServerCtrl::call(  const QString& scope, const QString& name, const QStringList& args )
{
  QString callback = QString("%1_%2").arg(scope).arg(name);
  
  if( !callback.isEmpty())
  {
    if(mCallbackFuncList.contains(callback) )
    {
      TRACECALLBACK func = mCallbackFuncList[callback];
      if( 0 != func)
      {
        func(args);
      }
    }
  }
}

