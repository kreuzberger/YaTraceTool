#ifndef TOOLS_SRC_TRACETOOL_COMMON_NTRACE_H
#define TOOLS_SRC_TRACETOOL_COMMON_NTRACE_H

#include <QMap>
#include <QStringList>
#include <QDataStream>
class QDataStream;

namespace Trace
{
  typedef quint16                    TracePort;
  typedef QString                    TracePID;
  typedef QMap<QString,int>          TraceFilter;
  typedef QMap<QString,TraceFilter>  TraceFilterMap;
  typedef QStringList                TraceMessage;
  typedef int                        TraceID;
  typedef QMap<QString,QStringList>  TraceCallback;
  typedef QMap<QString,TraceCallback>  TraceCallbackMap;

  const unsigned int TraceFmt_Invalid   = 0;
  const unsigned int TraceFmt_Filter    = 1;
  const unsigned int TraceFmt_Message   = 2;
  const unsigned int TraceFmt_Callback  = 4;
  const unsigned int TraceFmt_CallbackArg  = 8;
  const unsigned int TraceFmt_PID       = 16;
  const unsigned int TraceFmt_FilterMap = 32;
  const unsigned int TraceFmt_CallbackMap = 64;

  const unsigned int TimeOutRcv = 100;
  const unsigned int TimeOutSnd = 50;

  const TraceID TraceID_Invalid = 0;
  const TraceID TraceID_Log = 1;
  const TraceID TraceID_Warn = 2;
  const TraceID TraceID_Error= 4;
  const TraceID TraceID_EnterExit= 8;

  const Trace::TracePort TraceServer_Port = 4947;
  const Trace::TracePort TraceClient_Port = 4242;

  const unsigned int TraceArgs = 10;

  unsigned int deserialize(QDataStream&, TraceFilter&, TraceMessage&, TraceCallback&);
  unsigned int deserialize(QDataStream&, TraceFilterMap&, TraceMessage&, TraceCallbackMap&);
  unsigned int deserialize(QDataStream&,QString& name, unsigned int& pid);
  unsigned int deserialize(QDataStream&, TraceCallback&);

  bool serialize(QDataStream& ,const TraceFilter&);
  bool serialize(QDataStream& ,const TraceFilterMap&);
  bool serialize(QDataStream&, const TraceMessage& );
  bool serialize(QDataStream&, const QString& name, const unsigned int& pid);
  bool serialize(QDataStream&, const TraceCallback&);
  bool serialize(QDataStream&, const TraceCallbackMap&);


  //  bool deserialize(QSocketDevice*,TraceFilter&, QString&);

}


#endif


