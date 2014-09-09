project( traceserver )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
set (QT_USE_QTNETWORK 1)
set (QT_USE_QT3SUPPORT 1)
set (SOURCES
  TraceServer.cpp
  TraceServerMain.cpp
  TSClientSocket.cpp
  TSSimpleServer.cpp
)

QT4_WRAP_CPP( YATRACESERVER_MOC_HEADERS TSSimpleServer.h TraceServer.h TSClientSocket.h)
include(${QT_USE_FILE})
add_executable(yatraceserverapp ${SOURCES} ${YATRACESERVER_MOC_HEADERS})
target_link_libraries(yatraceserverapp ${QT_LIBRARIES} yatracelib)

install(TARGETS yatraceserverapp
          RUNTIME DESTINATION "yatracetool/bin"
)

install( PROGRAMS yatraceserver DESTINATION "bin" )

if(WIN32)
  set( APPS "${CMAKE_INSTALL_PREFIX}/yatracetool/bin/yatraceserverapp.exe")
else()
  set( APPS "${CMAKE_INSTALL_PREFIX}/yatracetool/bin/yatraceserverapp")
endif()
set( DIRS "${QT_LIBRARY_DIR}")
set( LIBS "" )

INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\"  \"${LIBS}\" \"${DIRS}\")
    " COMPONENT Runtime)

