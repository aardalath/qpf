#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015-2018
# Copyright (C) 2015-2018 Euclid SOC Team at ESAC
#======================================================================
include (../../common.cmake)

#===== Projec dir. =======
project (exsurvey)

set (exsurvey_src survey.cpp)

add_executable(exsurvey ${exsurvey_src})
target_include_directories (exsurvey PUBLIC .
  ${NNMSG_ROOT_DIR}/include
  ${NNCOMM_ROOT_DIR}
  ${FMK_ROOT_DIR}
  ${JSON_ROOT_DIR}
  ${LOG_ROOT_DIR}
  ${STR_ROOT_DIR}
  ${UUID_ROOT_DIR}
  ${INFIX_ROOT_DIR}
  ${PSQLLIBDIR})
target_link_libraries (exsurvey
  fmk json nncomm infix str log uuidxx
  nanomsg
  ${UUIDLIB} ${PSQLLIB}
  pthread)
set_target_properties (exsurvey PROPERTIES LINKER_LANGUAGE CXX)
install (TARGETS exsurvey
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)
