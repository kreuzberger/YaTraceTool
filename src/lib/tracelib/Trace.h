#ifndef TRACE_H
#define TRACE_H

#ifdef USE_TRACE

  #include "TraceServerCtrl.h"
  #include "TraceScopeLog.h"

  #define TraceDbg1( x ) TraceServerCtrl::getInstance()->sendLog( x);
  #define TraceError( x ) TraceServerCtrl::getInstance()->sendError( x);
  #define TraceWarn( x ) TraceServerCtrl::getInstance()->sendWarning( x);
  #define TraceScope( x ) TraceScopeLog traceScopeLog( #x );
  #define TraceDestroy( ) TraceServerCtrl::destroy();
  #define TraceInit( x ) TraceServerCtrl::getInstance()->setName( #x );


#define BEGIN_TRACESCOPE_DEF(x) class TraceClass##x { public: TraceClass##x(); };   \
    TraceClass##x::TraceClass##x() {
  #define TRACE_SCOPE_DEF(x) TraceServerCtrl::getInstance()->setScopeDef(#x);
  #define END_TRACESCOPE_DEF(x) } \
    static TraceClass##x gsTS##x;

#define TRACECALLBACK_CLASS(x) friend class TraceCallback##x; \
                                public: \
                                static x* mgCallbackHandler;
#define BEGIN_TRACECALLBACK_DEF(x)  class TraceCallback##x { public: TraceCallback##x() {}    
#define TRACE_CALLBACK_DEF(x)   static void callback_##x(const QStringList&);
#define END_TRACECALLBACK_DEF(x) }; \
                                  x* x::mgCallbackHandler = 0;

#define BEGIN_TRACECALLBACK_REGISTER(x) class TraceCallbackRegister##x { public: TraceCallbackRegister##x(); };   \
    TraceCallbackRegister##x::TraceCallbackRegister##x() {
#define TRACE_CALLBACK_REGISTER(x,y) TraceServerCtrl::getInstance()->setCallbackDef(#x,#y,&TraceCallback##x::callback_##y,QStringList()
#define TRACE_CALLBACK_PARAM(x) << #x
#define TRACE_CALLBACK_REGISTER_END );
#define END_TRACECALLBACK_REGISTER(x) } \
    static TraceCallbackRegister##x gsTR##x;

#define TRACECALLBACK_HANDLER(x) x::mgCallbackHandler
#define TRACECALLBACK_HANDLER_INIT(x) x::mgCallbackHandler = this;
//#define TRACECALLBACK_CALL( x, y, a1, a2, a3, a4 ) TraceServerCtrl::getInstance()->call(#x,#y,a1,a2,a3,a4);
#define TRACECALLBACK_IMPL(x,y) void TraceCallback##x::callback_##y(const QStringList& rArgs) {
#define TRACECALLBACK_IMPL_END (void) rArgs; } 
#define TRACECALLBACK_ARG(x) rArgs[x]

#else

#define TraceDbg1( x ) (void) x;
#define TraceError( x ) (void) x;
#define TraceWarn( x ) (void) x;
#define TraceScope( x )
#define TraceInit( x )
#define TraceDestroy( )

#define BEGIN_TRACESCOPE_DEF(x)
#define TRACE_SCOPE_DEF(x)
#define END_TRACESCOPE_DEF(x)
#define TRACE_CALLBACK_CLASS(x)

#define TRACECALLBACK_CLASS(x)
#define BEGIN_TRACECALLBACK_DEF(x)
#define TRACE_CALLBACK_DEF(x)
#define END_TRACECALLBACK_DEF(x)
#define BEGIN_TRACECALLBACK_REGISTER(x)
#define TRACE_CALLBACK_REGISTER(x,y)
#define TRACE_CALLBACK_PARAM(x)
#define TRACE_CALLBACK_REGISTER_END
#define END_TRACECALLBACK_REGISTER(x)
#define TRACECALLBACK_HANDLER(x)
#define TRACECALLBACK_HANDLER_INIT(x)
//#define TRACECALLBACK_CALL( x, y, a1, a2, a3, a4 )
#define TRACECALLBACK_IMPL(x,y)
#define TRACECALLBACK_IMPL_END
#define TRACECALLBACK_ARG(x)

#endif
  
#endif

