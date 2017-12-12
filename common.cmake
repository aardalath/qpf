#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015-2018
# Copyright (C) 2015-2018 Euclid SOC Team at ESAC
#======================================================================
cmake_minimum_required(VERSION 2.8.2)
cmake_policy (SET CMP0015 NEW)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#===== COTS =====

if ("${COTSDIR}" STREQUAL "")

  set (NNMSG_ROOT_DIR /usr/local)
  set (NNMSGINCDIR ${NNMSG_ROOT_DIR}/include)
  set (NNMSGLIBDIR ${NNMSG_ROOT_DIR}/lib)

  set (UUIDINCDIR /usr/include)
  set (UUIDLIBDIR /usr/lib64)
  
  set (CURLINCDIR /usr/include)
  set (CURLLIBDIR /usr/lib64)
  
  set (PCRE2INCDIR /usr/include)
  set (PCRE2LIBDIR /usr/lib64)
  
  if (EXISTS /usr/pgsql-9.6)
    set (PSQLDIR /usr/pgsql-9.6)
    set (PSQLLIBDIR ${PSQLDIR}/lib)
    set (PSQLINCDIR ${PSQLDIR}/include)
  else()
    set (PSQLLIBDIR /usr/lib/x86_64-linux-gnu)
    set (PSQLINCDIR /usr/include/postgresql)
  endif()
  
else()

  message ("Using a custom COTSDIR = ${COTSDIR}")
  
  set (NNMSG_ROOT_DIR ${COTSDIR}/nanomsg)
  set (NNMSGINCDIR ${NNMSG_ROOT_DIR}/include)
  set (NNMSGLIBDIR ${NNMSG_ROOT_DIR}/lib64)

  set (UUID_ROOT_DIR ${COTSDIR}/uuid)
  set (UUIDINCDIR ${UUID_ROOT_DIR}/include)
  set (UUIDLIBDIR ${UUID_ROOT_DIR}/lib)
  
  set (CURL_ROOT_DIR ${COTSDIR}/curl)
  set (CURLINCDIR ${CURL_ROOT_DIR}/include)
  set (CURLLIBDIR ${CURL_ROOT_DIR}/lib)
  
  set (PCRE2_ROOT_DIR ${COTSDIR}/pcre2)
  set (PCRE2INCDIR ${PCRE2_ROOT_DIR}/include)
  set (PCRE2LIBDIR ${PCRE2_ROOT_DIR}/lib)
  
  set (PSQL_ROOT_DIR ${COTSDIR}/pgsql)
  set (PSQLINCDIR ${PSQL_ROOT_DIR}/include)
  set (PSQLLIBDIR ${PSQL_ROOT_DIR}/lib)

endif()

set (PCRE2LIB pcre2-posix)
set (UUIDLIB uuidxx)
set (PSQLLIB pq)

#===== Project libraries =====

#set (PWD /Users/jcgonzalez/ws/QPFnn)
set (PWD ..)

set (JSON_ROOT_DIR    ${PWD}/json)
set (NNCOMM_ROOT_DIR  ${PWD}/nncomm)
set (INFIX_ROOT_DIR   ${PWD}/infix)
set (TOOLS_ROOT_DIR   ${PWD}/tools)
set (LOG_ROOT_DIR     ${PWD}/log)
set (STR_ROOT_DIR     ${PWD}/str)
set (UUID_ROOT_DIR    ${PWD}/uuid)
set (FILEHDL_ROOT_DIR ${PWD}/filehdl)
set (FMK_ROOT_DIR     ${PWD}/fmk)
set (VOS_ROOT_DIR     ${PWD}/vos)

set (MONGOOSEDIR      ${PWD}/mongoose)

#==== Common directives

link_directories (
  ${NNMSGLIBDIR}
  ${UUIDLIBDIR}
  ${PCRE2LIBDIR}
  ${PSQLLIBDIR}
  ${CURLLIBDIR}
  ${NNCOMM_ROOT_DIR}
  ${FILEHDL_ROOT_DIR}
  ${FMK_ROOT_DIR}
  ${TOOLS_ROOT_DIR}
  ${JSON_ROOT_DIR}
  ${LOG_ROOT_DIR}
  ${INFIX_ROOT_DIR}
  ${STR_ROOT_DIR}
  ${UUID_ROOT_DIR}
  ${VOS_ROOT_DIR}
)
