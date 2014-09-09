project( yatracelib )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

set (SOURCES
  NTrace.cpp
  TraceScopeLog.cpp
  TraceServerCtrl.cpp
  TraceServerCtrlThread.cpp
)

QT4_WRAP_CPP( YATRACELIB_MOC_HEADERS TraceServerCtrlThread.h )
include(${QT_USE_FILE})
add_library(yatracelib ${SOURCES} ${YATRACELIB_MOC_HEADERS})

install(TARGETS yatracelib
          RUNTIME DESTINATION "yatracetool/bin"
          LIBRARY DESTINATION "yatracetool/lib"
          ARCHIVE DESTINATION "yatracetool/lib")

install(FILES Trace.h TraceServerCtrl.h TraceScopeLog.h NTrace.h
         DESTINATION "yatracetool/include"
)

