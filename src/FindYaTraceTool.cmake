# - Find YaTraceTools
#
#  USE_YATRACETOOL               - have the yatracetool found



if( NOT "${YATRACETOOL_INCLUDE_DIR}" STREQUAL "" AND NOT "${YATRACETOOL_INCLUDE_DIR}" STREQUAL "YATRACETOOL_INCLUDE_DIR-NOTFOUND")
  set( YATRACETOOL_FOUND true )
else()
  if( WIN32 )
    find_path(YATRACETOOL_INCLUDE_DIR
        NAMES Trace.h
        PATHS $ENV{PATH} "$ENV{PROGRAMFILES}/YaTraceTool/yatracetool" "$ENV{PROGRAMFILES(x86}/YaTraceTool/yatracetool"
        PATH_SUFFIXES include
        DOC "Path to include files for tracelib" )
  else()
    find_path(YATRACETOOL_INCLUDE_DIR
        NAMES Trace.h
        PATHS $ENV{PATH} /usr/local/yatracetool
        PATH_SUFFIXES include
        DOC "Path to include files for tracelib" )
  
  endif()
  if( WIN32 )
    find_library(YATRACETOOL_LIBRARY
        NAMES yatracelib
        PATHS $ENV{PATH} "$ENV{PROGRAMFILES}/YaTraceTool/yatracetool" "$ENV{PROGRAMFILES(x86}/YaTraceTool/yatracetool"
        PATH_SUFFIXES lib
        DOC "Path to tracelib archive library" )
  else()
    find_library(YATRACETOOL_LIBRARY
        NAMES yatracelib
        PATHS $ENV{PATH} /usr/local/yatracetool
        PATH_SUFFIXES lib
        DOC "Path to tracelib archive library" )
  endif()


  if( YATRACETOOL_INCLUDE_DIR STREQUAL "YATRACETOOL_INCLUDE_DIR-NOTFOUND" )
    set( USE_YATRACETOOL false )
    set( YATRACETOOL_INCLUDE_DIR )
  else()
      set( YATRACETOOL_FOUND true )

      if(NOT YaTraceTool_FIND_QUIETLY)
        message(STATUS "Found YaTraceTool! " ${YATRACETOOL_INCLUDE_DIR})
      endif()
  endif()

  if( NOT YATRACETOOL_FOUND )
    if(YaTraceTool_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find YaTraceTool")
    endif()
  endif()

endif()

set( YATRACETOOL_USE_FILE ${CMAKE_CURRENT_LIST_DIR}/FindYaTraceTool.cmake)
set( YATRACETOOL_INCLUDE_DIRS ${YATRACETOOL_INCLUDE_DIR} )
set( YATRACETOOL_LIBRARIES ${YATRACETOOL_LIBRARY} )

#mark_as_advanced( YATRACETOOL_INCLUDE_DIRS YATRACETOOL_LIBRARIES)

