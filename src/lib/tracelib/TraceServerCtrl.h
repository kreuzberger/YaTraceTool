#ifndef TRACETOOL_TRACELIB_TRACESERVERCTRL_H
#define TRACETOOL_TRACELIB_TRACESERVERCTRL_H

#include "TraceServerCtrlThread.h"

#include <qstring.h>
#include <qmap.h>

typedef void (*TRACECALLBACK)(const QStringList&);

class TraceServerCtrl
{
  friend class TraceServerCtrlThread;
  friend class TraceScopeLog;

public:
  static TraceServerCtrl* getInstance( );

  void sendScope(const QString&);
  void sendLog(const QString&);
  void sendMessage(const Trace::TraceID& uiTraceID, const QString&);
  void sendError(const QString&);
  void sendWarning(const QString&);
  void setName( const QString& );
  void setScope( const QString& );
  void setScopeDef(const QString&);
  void setCallbackDef( const QString&, const QString&, TRACECALLBACK, const QStringList& args );
  void call( const QString&, const QString&, const QStringList&);
  void terminate();

  static void destroy();

 protected:
   static void resetFilter();

private:
  TraceServerCtrl();
  ~TraceServerCtrl();
  TraceServerCtrl( const TraceServerCtrl& );
  /*DWORD getProcessName(wchar_t *name,int nameSize,DWORD pid);*/
  static TraceServerCtrl* _instance;

  TraceServerCtrlThread mThread;

  static QString mName;
  static Trace::TraceFilter mFilterList;
  Trace::TraceID mSend;
  QString moNames;
  static bool mbDestroyed;
  static unsigned int mPID;
  static Trace::TraceCallback* mpCallbackMap;
  QMap<QString,TRACECALLBACK> mCallbackFuncList;
};



extern TraceServerCtrl* gTraceServerCtrl;

#endif
