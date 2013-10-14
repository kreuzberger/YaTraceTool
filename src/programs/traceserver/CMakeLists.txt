project( traceserver )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
set (QT_USE_QTNETWORK 1)
set (SOURCES
  TraceServer.cpp
  TraceServerMain.cpp
  TSClientSocket.cpp
  TSSimpleServer.cpp
)

QT4_WRAP_CPP( YATRACESERVER_MOC_HEADERS TSSimpleServer.h TraceServer.h TSClientSocket.h)
include(${QT_USE_FILE})
add_executable(yatraceserver ${SOURCES} ${YATRACESERVER_MOC_HEADERS})
target_link_libraries(yatraceserver ${QT_LIBRARIES} yatracelib)
