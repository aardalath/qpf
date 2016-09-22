#======================================================================
# CMakeLists.txt
# LibComm - Node Communication Library
# Libcomm library
#======================================================================
# Author: J C Gonzalez - 2015, 2016
# Copyright (C) 2015, 2016 Euclid SOC Team at ESAC
#======================================================================

#===== Projec dir. =======
project (libcomm)

set (libcommLib_src
  commnode.cpp
  counter.cpp
  log.cpp
  r2rpeer.cpp
  statem.cpp
  thread.cpp
  tools.cpp)

set (libcommLib_hdr
  commnode.h
  common.h
  counter.h
  error.h
  log.h
  propdef.h
  r2rpeer.h
  statem.h
  thread.h
  tools.h
  version.h)

include (../ext.cmake)
link_directories (${ZMQDIR}/lib)

add_library (libcomm SHARED ${libcommLib_src})
add_dependencies (libcomm json)
target_include_directories (libcomm PUBLIC ../json ${ZMQDIR}/include)
target_link_libraries (libcomm json zmq)

install (TARGETS libcomm
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)

