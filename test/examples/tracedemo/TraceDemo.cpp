
#include "TraceDemo.h"


#include <stdio.h>
#include <qapplication.h>
#include <qmap.h>
//Added by qt3to4:
#include <QTimerEvent>

BEGIN_TRACESCOPE_DEF(MyDlg)
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_main )
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_ctor )
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timer2 )
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timer1 )
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timerfunc )
  TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_dump )
END_TRACESCOPE_DEF(MyDlg)

BEGIN_TRACECALLBACK_DEF(MyDlg)
  TRACE_CALLBACK_DEF( dump )
END_TRACECALLBACK_DEF(MyDlg)

BEGIN_TRACECALLBACK_REGISTER(MyDlg)
  TRACE_CALLBACK_REGISTER( MyDlg, dump )
    TRACE_CALLBACK_PARAM( filename )
  TRACE_CALLBACK_REGISTER_END
END_TRACECALLBACK_REGISTER(MyDlg)


MyDlg::MyDlg()
{
  TRACECALLBACK_HANDLER_INIT(MyDlg)
  
  TraceScope(tools_tracetool_tracedemo_mydlg_ctor);
  TraceDbg1(QString("%1").arg("start Timer1: 1000"));
  miTimerID1 = startTimer(1000);
  TraceDbg1(QString("%1").arg("start Timer2: 5000"));
  miTimerID2 = startTimer(5000);
  miCntId0 = 0;
  miCntId1 = 0;
}

MyDlg::~MyDlg()
{

}

void MyDlg::timerEvent(QTimerEvent* e)
{
  if(0 != e)
  {
    QString str;
    if( miTimerID1 == e->timerId())
    {
      miCntId0++;
      str.sprintf("timer event %d occured %d times",e->timerId(), miCntId0);
      TraceScope(tools_tracetool_tracedemo_mydlg_timer1);
      TraceDbg1(str);
    }
    else if ( miTimerID2 == e->timerId())
    {
      miCntId1++;
      str.sprintf("timer event %d occured %d times",e->timerId(), miCntId1);
      TraceScope(tools_tracetool_tracedemo_mydlg_timer2);
      timerfunc();
      TraceWarn(str);
    }
  }
}

void MyDlg::timerfunc()
{
  TraceScope(tools_tracetool_tracedemo_mydlg_timerfunc);
  TraceError("no handling defined");
}


void MyDlg::dump()
{
  TraceScope(tools_tracetool_tracedemo_mydlg_dump);
  
  QString str;  
  str.sprintf("timer event 0 occured %d times", miCntId0);
  TraceError(str);
  str.sprintf("timer event 1 occured %d times", miCntId1);
  TraceError(str);
}
//TraceInit("TraceDemo");
//static Test test;

int main( int argc, char* argv[] )
{
  TraceInit( TraceDemo );
  QApplication app(argc, argv);

  TraceScope(tools_tracetool_tracedemo_mydlg_main);
  TraceDbg1("opening Dlg");
 
  MyDlg dlg;
  dlg.show();
  app.setMainWidget(&dlg);
  app.exec();

 // TRACECALLBACK_CALL( MyDlg, dump,0,0,0,0) 
 // TraceDestroy();
  return 0;
}


#ifdef USE_TRACE
  TRACECALLBACK_IMPL(MyDlg,dump)
    if( 0 != TRACECALLBACK_HANDLER(MyDlg) )
    {
      TRACECALLBACK_HANDLER(MyDlg)->dump();
    }
  TRACECALLBACK_IMPL_END
#endif
