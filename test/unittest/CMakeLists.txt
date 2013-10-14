project( yatraceunittest )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

set (SOURCES
  TraceTest.cpp
)

include(${QT_USE_FILE})
add_executable(yatracetest ${SOURCES})
target_link_libraries(yatracetest ${QT_LIBRARIES} )
