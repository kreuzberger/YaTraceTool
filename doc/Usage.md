Usage
-----

To add scopes to your application, the class or code must be first filled with scope definitions.
For usage of callbacks, the class must also be prepared for handlinge the callbacks.

This is all done by adding macros to your class and source files.

**Prepare a class for callbacks**

    class MyDlg
    {
    
    TRACECALLBACK_CLASS(MyDlg)
    
    public:
      MyDlg();
      virtual ~MyDlg();
      void dump(); // a callback
    };

**Definition of trace scopes**

In your source file you define the scopes for a class by adding a block of macros. The scope levels are created by using _ in the scope names

    BEGIN_TRACESCOPE_DEF(MyDlg)
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_main )
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_ctor )
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timer2 )
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timer1 )
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_timerfunc )
      TRACE_SCOPE_DEF( tools_tracetool_tracedemo_mydlg_dump )
    END_TRACESCOPE_DEF(MyDlg)
    
**Definition of callbacks**

Like for scopes, the callbacks are also defined with macros

    BEGIN_TRACECALLBACK_DEF(MyDlg)
      TRACE_CALLBACK_DEF( dump )
    END_TRACECALLBACK_DEF(MyDlg)
    
    BEGIN_TRACECALLBACK_REGISTER(MyDlg)
      TRACE_CALLBACK_REGISTER( MyDlg, dump )
        TRACE_CALLBACK_PARAM( filename )
      TRACE_CALLBACK_REGISTER_END
    END_TRACECALLBACK_REGISTER(MyDlg)
    
If your callback should have parameters, thy have to be defined in the registration part for the callback
In the implementation of your class, you must provide a global pointer to your class to have the traceserver access your callback functions. This is done also with a macro

    MyDlg::MyDlg()
    {
      TRACECALLBACK_HANDLER_INIT(MyDlg)
    }

**Using your scopes**

In any method of your class, just now use the scopes you have defined.
First add the scope you want your output to occur in.
Second add the output with macros: TraceWarn, TraceError, TraceDbg....

    void MyDlg::dump()
    {
      TraceScope(tools_tracetool_tracedemo_mydlg_dump);
    
      QString str;
      str.sprintf("timer event 0 occured %d times", miCntId0);
      TraceError(str);
      str.sprintf("timer event 1 occured %d times", miCntId1);
      TraceError(str);
    }

**Add a definiton of your callbacks**
    #ifdef USE_TRACE
      TRACECALLBACK_IMPL(MyDlg,dump)
        if( 0 != TRACECALLBACK_HANDLER(MyDlg) )
        {
          TRACECALLBACK_HANDLER(MyDlg)->dump();
        }
      TRACECALLBACK_IMPL_END
    #endif


**Init the trace singletion in your process**

For usage of the trace macros, it is required to instantiate the trace singleton as early as possible in your main application. So in the first lines of your main you init the trace library

    int main( int argc, char* argv[] )
    {
      TraceInit( TraceDemo );
  
The singleton destroys on application exit. But if required (e.g. due to using memory checkers, it can also be deleted at end of your applications processing

      TraceDestroy();
      return 0;
    }

