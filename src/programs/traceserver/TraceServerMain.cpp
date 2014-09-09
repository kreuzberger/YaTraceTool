/****************************************************************************
** $Id: TraceServerMain.cpp,v 1.1 2011-06-14 08:43:24 kreuzberger Exp $
**
** Copyright (C) 1992-2007 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "TraceServer.h"
#include <qapplication.h>
#include <iostream>
void printUsage()
{
  std::cout << "traceserver [options]" << std::endl;
  std::cout << "options:" << std::endl;
  std::cout << " --port=         traceserver  port" << std::endl;
  std::cout << " --tcport=       tracesclient port" << std::endl;
  std::cout << " --help          print usage" << std::endl;
  std::cout << " --verbose       print verbose output" << std::endl;
}


int main( int argc, char** argv )
{
  TraceServerPara sTSPara;
  sTSPara.bLog = false;
  for(int idx = 0; idx < argc; idx++)
  {
    QString str(argv[idx]);
    if("-v" == str)
    {
      sTSPara.bLog = true;
    }
    else if("--verbose" == str)
    {
      sTSPara.bLog = true;
    }
    else if( "-p" == str )
    {
      QString port = argv[idx+1];
      bool bOk = false;
      sTSPara.uiPort = port.toShort(&bOk);
    }
    else if( str.startsWith("--port=") )
    {
      QString port = str.right(str.length()-strlen("--port="));
      bool bOk = false;
      sTSPara.uiPort = port.toShort(&bOk);
      if(!bOk) sTSPara.uiPort = Trace::TraceServer_Port;
    }

    else if( "-tc" == str )
    {
      QString port = argv[idx+1];
      bool bOk = false;
      sTSPara.uiTCPort = port.toShort(&bOk);
      if(!bOk) sTSPara.uiTCPort = Trace::TraceClient_Port;
    }
    else if( str.startsWith("--tcport=") )
    {
      QString port = str.right(str.length()-strlen("--tcport="));
      bool bOk = false;
      sTSPara.uiTCPort = port.toShort(&bOk);
    }
    else if( "-h" == str || "--help" == str)
    {
      printUsage();
      return 0;
    }

  }

  QApplication app( argc, argv );
  TraceServerAbortHandler abortHandler;

  TraceServer ts(sTSPara);

  int iRet = app.exec();
  return iRet;
}
