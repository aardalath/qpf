#======================================================================
# SDC - Structured Data Container
#======================================================================
# Library project file
# Author: J C Gonzalez - 2015,2016,2017 
#======================================================================

project (sdc)

set (sdcLib_src
  array.cpp
  dict.cpp
  value.cpp
  valueiojsoncpp.cpp)

set (sdcLib_hdr
  sdc.h
  array.h
  dict.h
  value.h
  valueio.h
  valueiojsoncpp.h)

add_library (sdc SHARED ${sdcLib_src})
target_include_directories (sdc PUBLIC ../json)
add_dependencies (sdc json)
target_link_libraries (sdc json)

install (TARGETS sdc
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)

