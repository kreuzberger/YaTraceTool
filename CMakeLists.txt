cmake_minimum_required(VERSION 2.6)
project(YaTraceTool)


#set ( QT_QMAKE_EXECUTABLE /usr/bin/qmake-qt4 )
#set ( QT4_QGLOBAL_FILE /usr/include/qt4/Qt/qglobal.h )
set( DESIRED_QT_VERSION 4)

INCLUDE (${CMAKE_ROOT}/Modules/FindQt.cmake)
#INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
if( CMAKE_HOST_UNIX )
  include_directories(/usr/include)
else( CMAKE_HOST_UNIX )
endif( CMAKE_HOST_UNIX )


enable_testing()
include(CTest)


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src  )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/src  )


#ADD_DEFINITIONS( ${QT_DEFINITIONS} )
add_subdirectory(src)
add_subdirectory(test)

