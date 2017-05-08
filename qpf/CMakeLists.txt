#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015,2016,2017 
# Copyright (C) 2015,2016,2017  Euclid SOC Team at ESAC
#======================================================================

#===== Projec dir. =======
PROJECT(qpfcore)

set (qpfcoreLib_hdr
  deployer.h
  init.h)

set (qpfcoreLib_src
  main.cpp
  deployer.cpp)

include (../ext.cmake)

link_directories (${PSQLDIR}/lib ${ZMQDIR}/lib ${CURLDIR}/lib
                  ${UUIDDIR}/lib ${PCRE2DIR}/lib)

add_executable(qpfcore ${qpfcoreLib_src})
add_dependencies (qpfcore qpf libcomm json sdc infix)
target_include_directories (qpfcore PUBLIC ../src ../libcomm ../json ../sdc ../infix
  ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
  ${UUIDDIR}/include ${PCRE2DIR}/include)
target_link_libraries (qpfcore
  qpf libcomm json sdc infix
  ${PSQLLIB} zmq curl uuid pcre2-posix)

install (TARGETS qpfcore
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)
