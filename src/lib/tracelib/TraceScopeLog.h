#ifndef TRACETOOL_TRACELIB_TRACESCOPELOG_H
#define TRACETOOL_TRACELIB_TRACESCOPELOG_H

#include <qstring.h>
#include <qstringlist.h>

class TraceScopeLog
{
public:
  TraceScopeLog(const char* ptr);
  virtual ~TraceScopeLog();

private:

  static QStringList mScopeList;
  static QString  moFillStr;
};



#endif
