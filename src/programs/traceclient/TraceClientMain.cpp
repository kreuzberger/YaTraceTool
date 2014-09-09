#include "TraceClient.h"
#include "TraceClientView.h"
#include <qapplication.h>
#include <iostream>
#include "lib/tracelib/NTrace.h"

void printUsage()
{
  std::cout << "traceclient [options]" << std::endl;
  std::cout << "options:" << std::endl;
  std::cout << " --host=         traceserver host ip address" << std::endl;
  std::cout << " --port=         traceserver host port" << std::endl;
  std::cout << " --autoconnect   establisch connection on start" << std::endl;
  std::cout << " --help          print usage" << std::endl;
}


int main( int argc, char** argv )
{
  TraceClientPara oPara;

  oPara.moHost = "127.0.0.1";
  oPara.muiPort = Trace::TraceClient_Port;
  oPara.mbAutoConnect = false;

  for(int idx=0; idx < argc; idx++)
  {
    QString str(argv[idx]);

    if(str.startsWith("--host="))
    {
      oPara.moHost = str.right(str.length()-QString("--host=").length());
    }
    else if (str.startsWith("-h="))
    {
      oPara.moHost = str.right(str.length()-QString("-h=").length());
    }
    else if(str.startsWith("--port="))
    {
      oPara.muiPort = str.right(str.length()-QString("--port=").length()).toUInt();
    }
    else if (str.startsWith("-p="))
    {
      oPara.muiPort = str.right(str.length()-QString("-p=").length()).toUInt();
    }
    else if("--autoconnect" == str)
    {
      oPara.mbAutoConnect = true;
    }
    else if("-a" == str)
    {
      oPara.mbAutoConnect = true;
    }
    else if(("--help" == str) || ("/?" == str) || ("-help" == str))
    {
      printUsage();
      return 0;
    }
  }

  QApplication app( argc, argv );

  TraceClientAbortHandler oAbortHandler;

  TraceClientView tcView;
  TraceClient client(tcView);

  tcView.setPara(oPara);
  tcView.resize( 650, 350 );

  app.setMainWidget( &tcView );
  tcView.show();

  int iRet = app.exec();

  return iRet;
}

