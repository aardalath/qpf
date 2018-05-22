#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# General Project File
#======================================================================
# Author: J C Gonzalez - 2015-2018 
# Copyright (C) 2015-2018  Euclid SOC Team at ESAC
#======================================================================
include (../common.cmake)

project (tests)

enable_testing()

include (CMakeLists.txt.gtest)

set (unitTestsSet_hdr
  infix/test_infixeval.h
  fmk/test_ContainerMng.h
  fmk/test_Component.h
  fmk/test_CfgGrpGeneral.h
  fmk/test_CfgGrpSwarm.h
  fmk/test_CfgGrpNetwork.h
  fmk/test_CfgGrpDB.h
  fmk/test_CfgGrpVOSpace.h
  fmk/test_CfgGrpJupyter.h
  fmk/test_CfgGrpIPython.h
  fmk/test_CfgGrpConnectivity.h
  fmk/test_CfgGrpProducts.h
  fmk/test_CfgGrpRulesList.h
  fmk/test_CfgGrpOrchestration.h
  fmk/test_CfgGrpUserDefToolsList.h
  fmk/test_CfgGrpFlags.h
  fmk/test_Config.h
  fmk/test_DataMng.h
  fmk/test_JValue.h
  fmk/test_JRecord.h
  fmk/test_DBHandler.h
  fmk/test_DBHdlPostgreSQL.h
  fmk/test_DockerMng.h
  fmk/test_EvtMng.h
  fmk/test_Exception.h
  fmk/test_RuntimeException.h
  fmk/test_LogicException.h
  fmk/test_Error.h
  fmk/test_FileNameSpec.h
  fmk/test_FitsMetadataReader.h
  fmk/test_HostInfo.h
  fmk/test_HttpServer.h
  fmk/test_LogMng.h
  fmk/test_Master.h
  fmk/test_MsgHeader.h
  fmk/test_MessageBase.h
  fmk/test_MsgBodyCMD.h
  fmk/test_MsgBodyINDATA.h
  fmk/test_MsgBodyTSK.h
  fmk/test_Message.h
  fmk/test_MetadataInfo.h
  fmk/test_MetadataReader.h
  fmk/test_ServiceMng.h
  fmk/test_Synchronizer.h
  fmk/test_TskAge.h
  fmk/test_TskMng.h
  fmk/test_TskOrc.h
  fmk/test_URLHandler.h
  filehdl/test_GenericFileHandler.h
  filehdl/test_JsonFileHandler.h
  filehdl/test_QDTReportHandler.h
  log/test_Log.h
  nncomm/test_Bus.h
  nncomm/test_CommNode.h
  nncomm/test_Pair.h
  nncomm/test_Pipeline.h
  nncomm/test_ProtocolLayer.h
  nncomm/test_PubSub.h
  nncomm/test_ReqRep.h
  nncomm/test_ScalabilityProtocolRole.h
  nncomm/test_Survey.h
  qpf/test_Deployer.h
  str/test_str.h
  tools/test_Alert.h
  tools/test_DirWatcher.h
  tools/test_MetadataInfo.h
  tools/test_MetadataReader.h
  tools/test_FitsMetadataReader.h
  tools/test_PCRegEx.h
  tools/test_pipe_t.h
  tools/test_pipe_ostreambuf.h
  tools/test_pipe_streambuf.h
  tools/test_process.h
  tools/test_pipeline.h
  tools/test_RWC.h
  tools/test_ScopeExit.h
  tools/test_StateMachine.h
  tools/test_Timer.h
  uuid/test_UUID.h
  vos/test_VOSpaceHandler.h
    )

set (unitTestsSet_src
  infix/test_infixeval.cpp
  main.cpp
  fmk/test_ContainerMng.cpp
  fmk/test_Component.cpp
  fmk/test_CfgGrpGeneral.cpp
  fmk/test_CfgGrpSwarm.cpp
  fmk/test_CfgGrpNetwork.cpp
  fmk/test_CfgGrpDB.cpp
  fmk/test_CfgGrpVOSpace.cpp
  fmk/test_CfgGrpJupyter.cpp
  fmk/test_CfgGrpIPython.cpp
  fmk/test_CfgGrpConnectivity.cpp
  fmk/test_CfgGrpProducts.cpp
  fmk/test_CfgGrpRulesList.cpp
  fmk/test_CfgGrpOrchestration.cpp
  fmk/test_CfgGrpUserDefToolsList.cpp
  fmk/test_CfgGrpFlags.cpp
  fmk/test_Config.cpp
  fmk/test_DataMng.cpp
  fmk/test_JValue.cpp
  fmk/test_JRecord.cpp
  fmk/test_DBHandler.cpp
  fmk/test_DBHdlPostgreSQL.cpp
  fmk/test_DockerMng.cpp
  fmk/test_EvtMng.cpp
  fmk/test_Exception.cpp
  fmk/test_RuntimeException.cpp
  fmk/test_LogicException.cpp
  fmk/test_Error.cpp
  fmk/test_FileNameSpec.cpp
  fmk/test_FitsMetadataReader.cpp
  fmk/test_HostInfo.cpp
  fmk/test_HttpServer.cpp
  fmk/test_LogMng.cpp
  fmk/test_Master.cpp
  fmk/test_MsgHeader.cpp
  fmk/test_MessageBase.cpp
  fmk/test_MsgBodyCMD.cpp
  fmk/test_MsgBodyINDATA.cpp
  fmk/test_MsgBodyTSK.cpp
  fmk/test_Message.cpp
  fmk/test_MetadataInfo.cpp
  fmk/test_MetadataReader.cpp
  fmk/test_ServiceMng.cpp
  fmk/test_Synchronizer.cpp
  fmk/test_TskAge.cpp
  fmk/test_TskMng.cpp
  fmk/test_TskOrc.cpp
  fmk/test_URLHandler.cpp
  filehdl/test_GenericFileHandler.cpp
  filehdl/test_JsonFileHandler.cpp
  filehdl/test_QDTReportHandler.cpp
  log/test_Log.cpp
  nncomm/test_Bus.cpp
  nncomm/test_CommNode.cpp
  nncomm/test_Pair.cpp
  nncomm/test_Pipeline.cpp
  nncomm/test_ProtocolLayer.cpp
  nncomm/test_PubSub.cpp
  nncomm/test_ReqRep.cpp
  nncomm/test_ScalabilityProtocolRole.cpp
  nncomm/test_Survey.cpp
  qpf/test_Deployer.cpp
  str/test_str.cpp
  tools/test_Alert.cpp
  tools/test_DirWatcher.cpp
  tools/test_MetadataInfo.cpp
  tools/test_MetadataReader.cpp
  tools/test_FitsMetadataReader.cpp
  tools/test_PCRegEx.cpp
  tools/test_pipe_t.cpp
  tools/test_pipe_ostreambuf.cpp
  tools/test_pipe_streambuf.cpp
  tools/test_process.cpp
  tools/test_pipeline.cpp
  tools/test_RWC.cpp
  tools/test_ScopeExit.cpp
  tools/test_StateMachine.cpp
  tools/test_Timer.cpp
  uuid/test_UUID.cpp
  vos/test_VOSpaceHandler.cpp
    )

add_executable(unitTestsSet ${unitTestsSet_src})
add_dependencies (unitTestsSet json sdc infix qpf libcomm)

set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS    "-coverage -lgcov -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

target_include_directories (unitTestsSet PUBLIC . ..
  ${NNMSG_ROOT_DIR}/include
  ${NNCOMM_ROOT_DIR}
  ${FMK_ROOT_DIR}
  ${FMK_ROOT_DIR}/../qpf
  ${JSON_ROOT_DIR}
  ${LOG_ROOT_DIR}
  ${STR_ROOT_DIR}
  ${UUID_ROOT_DIR}
  ${INFIX_ROOT_DIR}
  ${PSQLLIBDIR})
target_link_libraries (unitTestsSet
  fmk json nncomm infix str log uuidxx
  nanomsg
  ${UUIDLIB} ${PSQLLIB}
  pthread
  ${googleTestLibraries})

add_test (NAME unitTestsSet COMMAND unitTests)
