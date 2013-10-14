project( yatracedemo )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

set (QT_USE_QT3SUPPORT 1)
set (SOURCES
  TraceDemo.cpp
)

QT4_WRAP_CPP( YATRACEDEMO_MOC_HEADERS TraceDemo.h)
include(${QT_USE_FILE})
add_executable(yatracedemo ${SOURCES} ${YATRACEDEMO_MOC_HEADERS})
target_link_libraries(yatracedemo ${QT_LIBRARIES} )
