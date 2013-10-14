project( yatraceclient )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

set (QT_USE_QTNETWORK 1)
set (QT_USE_QT3SUPPORT 1)
set (SOURCES
  TraceClientMain.cpp
  TraceClient.cpp
  TraceClientView.cpp
  TraceClientCBView.cpp
  TCCheckListItem.cpp
  TCCBListItem.cpp
  TCClientSocket.cpp
)

QT4_WRAP_CPP( YATRACECLIENT_MOC_HEADERS TraceClient.h TraceClientCBView.h TCClientSocket.h TraceClientView.h)
include(${QT_USE_FILE})
add_executable(yatraceclient ${SOURCES} ${YATRACECLIENT_MOC_HEADERS})
target_link_libraries(yatraceclient yatracelib ${QT_LIBRARIES})

install(TARGETS yatraceclient
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib/static)
