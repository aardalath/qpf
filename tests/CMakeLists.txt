#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015,2016,2017 
# Copyright (C) 2015,2016,2017  Euclid SOC Team at ESAC
#======================================================================

#===== tests =====
project (tests)

enable_testing()

set (GTESTDIR $ENV{GTESTDIR})
include_directories(${GTESTDIR}/include)

set (qpfUnitTests_hdr
    # sdc/test_array.h
    # sdc/test_dict.h
    # sdc/test_value.h
    # sdc/test_valueiojsoncpp.h
    infix/test_infixeval.h
    libcomm/test_commnode.h
    libcomm/test_log.h
    libcomm/test_r2rpeer.h
    libcomm/test_statem.h
    libcomm/test_thread.h
    libcomm/test_tools.h
    # src/test_cfginfo.h
    # src/test_component.h
    # src/test_config.h
    # src/test_datamng.h
    # src/test_datatypes.h
    # src/test_dbg.h
    # src/test_dbhdlpostgre.h
    # src/test_evtmng.h
    # src/test_except.h
    # src/test_filenamespec.h
    # src/test_filetransfer.h
    # src/test_logmng.h
    # src/test_procelem.h
    # src/test_str.h
    # src/test_taskagent.h
    # src/test_taskmng.h
    # src/test_taskorc.h
    # src/test_urlhdl.h
    # src/test_uuid.h
    )

set (qpfUnitTests_src
    main.cpp
    # sdc/test_array.cpp
    # sdc/test_dict.cpp
    # sdc/test_value.cpp
    # sdc/test_valueiojsoncpp.cpp
    infix/test_infixeval.cpp
    libcomm/test_commnode.cpp
    libcomm/test_log.cpp
    libcomm/test_r2rpeer.cpp
    libcomm/test_statem.cpp
    libcomm/test_thread.cpp
    libcomm/test_tools.cpp
    # src/test_cfginfo.cpp
    # src/test_component.cpp
    # src/test_config.cpp
    # src/test_datamng.cpp
    # src/test_datatypes.cpp
    # src/test_dbg.cpp
    # src/test_dbhdlpostgre.cpp
    # src/test_evtmng.cpp
    # src/test_except.cpp
    # src/test_filenamespec.cpp
    # src/test_filetransfer.cpp
    # src/test_logmng.cpp
    # src/test_procelem.cpp
    # src/test_str.cpp
    # src/test_taskagent.cpp
    # src/test_taskmng.cpp
    # src/test_taskorc.cpp
    # src/test_urlhdl.cpp
    # src/test_uuid.cpp
    )

include (../ext.cmake)
link_directories (${PSQLDIR}/lib ${ZMQDIR}/lib ${CURLDIR}/lib
                  ${UUIDDIR}/lib ${PCRE2DIR}/lib ${GTESTDIR}/lib)

add_executable(qpfUnitTests ${qpfUnitTests_src})
add_dependencies (qpfUnitTests json sdc infix qpf libcomm)
target_include_directories (qpfUnitTests PUBLIC
    ../src ../libcomm ../json ../sdc ../infix
    ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
    ${UUIDDIR}/include ${PCRE2DIR}/include
    ${GTESTDIR}/include)
target_link_libraries (qpfUnitTests
    qpf libcomm json sdc infix
    ${PSQLLIB} zmq curl uuid pcre2-posix
    gtest gtest_main)

add_executable(testfs src/test-filenamespec.cpp)
add_dependencies (qpf libcomm)
target_include_directories (testfs PUBLIC . ../src ../libcomm 
  ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
  ${UUIDDIR}/include ${PCRE2DIR}/include)
target_link_libraries (testfs
  qpf libcomm 
  ${PSQLLIB} zmq curl uuid pcre2-posix)

add_test (NAME qpfUnitTests COMMAND qpfUnitTests)
