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
  }

  QApplication app( argc, argv );
  TraceServerAbortHandler abortHandler;

  TraceServer ts(sTSPara);

  int iRet = app.exec();
  return iRet;
}
