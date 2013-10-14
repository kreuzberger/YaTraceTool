#include "TraceScopeLog.h"
#include "TraceServerCtrl.h"

QStringList TraceScopeLog::mScopeList;
QString TraceScopeLog::moFillStr="                                                         ";

TraceScopeLog::TraceScopeLog(const char* ptr)
{
  if( 0 != ptr )
  {
    TraceServerCtrl::getInstance()->setScope( ptr );
    TraceServerCtrl::getInstance()->sendScope(QString("%1--&gt; %2").arg(moFillStr.left(mScopeList.count())).arg(ptr));
    mScopeList.append(ptr);
  }
}

TraceScopeLog::~TraceScopeLog()
{
  if(!TraceServerCtrl::mbDestroyed)
  {
    TraceServerCtrl::getInstance()->sendScope(QString("%1&lt;-- %2").arg(moFillStr.left(mScopeList.count()-1)).arg(mScopeList.last()));
    mScopeList.pop_back();
    if( mScopeList.count())
    {
      TraceServerCtrl::getInstance()->setScope( mScopeList.last() );
    }
  }
}

