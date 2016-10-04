#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015, 2016
# Copyright (C) 2015, 2016 Euclid SOC Team at ESAC
#======================================================================

#===== CMAKE Init. Section =====
cmake_minimum_required (VERSION 2.8.11)
#cmake_policy (SET CMP0046 NEW)

#===== QPF =====
project (QPF)

set (PROJECT_NAME QPF)

set (QPF_MAJOR 1)
set (QPF_MINOR 0)
set (QPF_PATCH 1)

set (QPF_VERSION ${QPF_MAJOR}.${QPF_MINOR}.${QPF_PATCH})

message (STATUS "Building QPF version ${QPF_VERSION}")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (BUILD_ID $ENV{BUILD_ID})
add_definitions(-DBUILD_ID="${BUILD_ID}")

add_subdirectory (json)
add_subdirectory (sdc)
add_subdirectory (infix)
add_subdirectory (libcomm)
add_subdirectory (src)
add_subdirectory (qpf)

if (HMI)
  message ("HMI component will be generated as well")
  add_subdirectory (qpfgui)
endif()

add_subdirectory (tests)

#== CPack configuration
set (CPACK_PACKAGE_VERSION_MAJOR ${QPF_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${QPF_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${QPF_PATCH})
set (CPACK_GENERATOR "TGZ")
set (CPACK_PACKAGING_INSTALL_PREFIX "")
include (CPack)
