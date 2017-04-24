#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015, 2016
# Copyright (C) 2015, 2016 Euclid SOC Team at ESAC
#======================================================================

#===== Projec dir. =======
project (libqpf)

set (libqpfLib_hdr
  alert.h
  cfg.h
  cfginfo.h
  common.h
  component.h
  config.h
  datamng.h
  datatypes.h
  dbg.h
  dbhdl.h
  dbhdlpostgre.h
  dwatcher.h
  error.h
  evtmng.h
  except.h
  filenamespec.h
  filetransfer.h
  logmng.h
  procelem.h
  propdef.h
  str.h
  taskagent.h
  taskmng.h
  taskorc.h
  urlhdl.h
  uuid.h
  version.h)

set (libqpfLib_src
  alert.cpp
  cfginfo.cpp
  component.cpp
  config.cpp
  datamng.cpp
  datatypes.cpp
  dbg.cpp
  dbhdlpostgre.cpp
  dwatcher.cpp
  evtmng.cpp
  except.cpp
  filenamespec.cpp
  filetransfer.cpp
  logmng.cpp
  procelem.cpp
  str.cpp
  taskagent.cpp
  taskmng.cpp
  taskorc.cpp
  urlhdl.cpp
  uuid.cpp)

include (../ext.cmake)
link_directories (${PSQLDIR}/lib ${ZMQDIR}/lib ${CURLDIR}/lib
                  ${UUIDDIR}/lib ${PCRE2DIR}/lib)

add_library(qpf SHARED ${libqpfLib_src})
add_dependencies (qpf json sdc infix)
target_include_directories (qpf PUBLIC
  ../libcomm ../json ../sdc ../infix
  ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
  ${UUIDDIR}/include ${PCRE2DIR}/include)
target_link_libraries (qpf json sdc infix ${PSQLLIB} zmq curl uuid)

install (TARGETS qpf 
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)

