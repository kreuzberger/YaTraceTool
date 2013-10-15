project( yatraceunittest )
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )


set (QT_USE_QTTEST 1)
set (SOURCES
  TraceTest.cpp
)

QT4_WRAP_CPP( YATRACETEST_MOC_HEADERS TraceTest.h)
include(${QT_USE_FILE})
add_executable(yatracetest ${SOURCES} ${YATRACETEST_MOC_HEADERS})
target_link_libraries(yatracetest yatracelib ${QT_LIBRARIES} )
add_test( TestYaTrace yatracetest)