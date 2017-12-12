#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015-2018
# Copyright (C) 2015-2018 Euclid SOC Team at ESAC
#======================================================================
include (../common.cmake)

project (nncomm)

set (libnncomm_hdr
  nn.hpp
  scalprotrole.h
  dbg.h
  err.h
  fast.h
  bus.h
  pair.h
  pipeline.h
  pubsub.h
  reqrep.h
  survey.h
  commnode.h
  protlayer.h
)

set (libnncomm_src
  err.cpp
  dbg.cpp
  scalprotrole.cpp
  bus.cpp
  pair.cpp
  pipeline.cpp
  pubsub.cpp
  reqrep.cpp
  survey.cpp
#  commnode.cpp
  protlayer.cpp
)

#===== Project sections/libraries =======

link_directories (${NNMSGLIBDIR})

add_library(nncomm SHARED ${libnncomm_src})
target_include_directories (nncomm PUBLIC . json ${NNMSGINCDIR})
target_link_libraries (nncomm nanomsg pthread)
set_target_properties (nncomm PROPERTIES LINKER_LANGUAGE CXX)
install (TARGETS nncomm
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)
