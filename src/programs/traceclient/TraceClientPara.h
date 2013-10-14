#ifndef TOOLS_TRACETOOL_TRACECLIENTPARA_H
#define TOOLS_TRACETOOL_TRACECLIENTPARA_H

#include <qstring.h>
#include "lib/tracelib/NTrace.h"

class TraceClientPara
{
public:
  QString moHost;
  Trace::TracePort muiPort;
  bool     mbAutoConnect;
};


#endif
