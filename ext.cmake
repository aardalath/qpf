#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015, 2016
# Copyright (C) 2015, 2016 Euclid SOC Team at ESAC
#======================================================================

#===== Projec dir. =======
set (PRJ_ROOT_DIR $ENV{PWD})

set (BUILD_ROOT build)

#===== External libraries =======
set (COTSDIR /opt/cots)
set (HOME $ENV{HOME})

message (STATUS "Now with ${PRJ_ROOT_DIR} . . .")

if (EXISTS ${HOME}/opt)
  message (STATUS "Using /opt")
  set (ZMQDIR ${HOME}/opt/zmq)
  set (CURLDIR ${HOME}/opt/curl)
  set (PSQLDIR ${HOME}/opt/pgsql)
  set (PCRE2DIR ${HOME}/opt/pcre2)
  set (UUIDDIR ${HOME}/opt/uuid)
elseif (EXISTS /opt)
  message (STATUS "Using ${HOME}/opt")
  set (ZMQDIR ${COTSDIR})
  set (CURLDIR ${COTSDIR})
  set (PSQLDIR /opt/pgsql)
  set (PCRE2DIR ${COTSDIR})
  set (UUIDDIR ${COTSDIR})
endif()

if (EXISTS /usr/pgsql-9.6)
  set (PSQLDIR /usr/pgsql-9.6)
endif()

# if (EXISTS ${HOME}/opt)
  # set (GTESTDIR ${HOME}/ws/jcgg/cots/gtest-1.7.0)
  # set (GTESTDIR ${HOME}/ws/cots/gtest-1.7.0)
# endif()

set (PSQLLIB pq)

#===== Project sections/libraries =======
set (LIBCOMMPATH ${PRJ}_ROOT_DIR)
set (LIBCOMMLIB ${LIBCOMMPATH}/${BUILD}_ROOT/libcomm)
set (LIBCOMMINC ${LIBCOMMPATH}/libcomm)

set (JSONCPPPATH ${PRJ}_ROOT_DIR)
set (JSONCPPLIB ${JSONCPPPATH}/${BUILD}_ROOT/json)
set (JSONCPPINC ${JSONCPPPATH}/json)

set (INFIXPATH ${PRJ}_ROOT_DIR)
set (INFIXLIB ${INFIXPATH}/${BUILD}_ROOT/infix)
set (INFIXINC ${INFIXPATH}/infix)

set (SDCPATH ${PRJ}_ROOT_DIR)
set (SDCLIB ${SDCPATH}/${BUILD}_ROOT/sdc)
set (SDCINC ${SDCPATH}/sdc)
