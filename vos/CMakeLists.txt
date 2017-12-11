#======================================================================
# VOSHdl - VOSpace Handlers
#======================================================================
# Library project file
# Author: J C Gonzalez - 2015-2017
#======================================================================
cmake_minimum_required(VERSION 2.8)
cmake_policy (SET CMP0015 OLD)

include (../common.cmake)

project (vos)

set (vosLib_hdr
  voshdl.h)

set (vosLib_src
  voshdl.cpp)

#find_package(CURL REQUIRED)

add_library (vos SHARED ${vosLib_src})
set_target_properties (vos PROPERTIES LINKER_LANGUAGE CXX)
#include_directories(. ../tools ${CURL_INCLUDE_DIR})
include_directories(. ../tools ${CURLINCDIR})
link_directories (${CURLLIBDIR})
target_include_directories (vos PUBLIC . ../tools)
target_link_libraries(vos curl tools)
install (TARGETS vos
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)

