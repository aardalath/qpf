#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# qpfgui - QPF HMI Application
#======================================================================
# Author: J C Gonzalez - 2015-2018
# Copyright (C) 2015-2018  Euclid SOC Team at ESAC
#======================================================================
include (../common.cmake)

project (qpfGui)

find_package (Qt5Widgets)
find_package (Qt5Sql)
find_package (Qt5Network)
find_package (Qt5Xml)
#find_package (Qt5WebKit)
#find_package (Qt5WebKitWidgets)

set (qpfGuiLib_src
  acthdl.cpp
  browser.cpp
  configtool.cpp
  connectionwidget.cpp
  dbbrowser.cpp
  dbmng.cpp
  dbtblmodel.cpp
  dbtreemodel.cpp
  dlgalert.cpp
  dlgdataing.cpp
  dlgshowtaskinfo.cpp
  dlgreproc.cpp
  dlguserpwd.cpp
  dlgqdtfilter.cpp
  frm_diagflt.cpp
  anasession.cpp
  exporthdl.cpp
  conthostedit.cpp
  swarmedit.cpp
  ruleedit.cpp
  exttooledit.cpp
  exttoolsdef.cpp
  frm_filter.cpp
  frm_filterset.cpp
  frmagentstatus.cpp
  frmhostinfo.cpp
  hmipxy.cpp
  keyevt.cpp
  logframe.cpp
  logwatcher.cpp
  mainwindow.cpp
  multitextview.cpp
  procfmkmonit.cpp
  procalertmodel.cpp
  proctskstatmodel.cpp
  productsmodel.cpp
  prodfiltmodel.cpp
  progbardlg.cpp
  qdtfilters.cpp
  qjsonitem.cpp
  qjsonmodel.cpp
  qsimpletickergraph.cpp
  sysalertmodel.cpp
  textview.cpp
  txtblmodel.cpp
  types.cpp
  verbleveldlg.cpp
  txitem.cpp
  txitemdlg.cpp
  xmlsyntaxhighlight.cpp)

set (qpfGuiLib_hdr
  acthdl.h
  browser.h
  configtool.h
  connectionwidget.h
  dbbrowser.h
  dbmng.h
  dbtblmodel.h
  dbtreemodel.h
  dlgalert.h
  dlgdataing.h
  dlgshowtaskinfo.h
  dlgreproc.h
  dlguserpwd.h
  dlgqdtfilter.h
  frm_diagflt.h
  anasession.h
  exporthdl.h
  conthostedit.h
  swarmedit.h
  ruleedit.h
  exttooledit.h
  exttoolsdef.h
  filters.h
  frm_filter.h
  frm_filterset.h
  frmagentstatus.h
  frmhostinfo.h
  hmipxy.h
  hmitypes.h
  init.h
  keyevt.h
  logframe.h
  logwatcher.h
  mainwindow.h
  multitextview.h
  procfmkmonit.h
  procalertmodel.h
  proctskstatmodel.h
  productsmodel.h
  prodfiltmodel.h
  progbardlg.h
  qdtfilters.h
  qjsonitem.h
  qjsonmodel.h
  qsimpletickergraph.h
  sysalertmodel.h
  textview.h
  txtblmodel.h
  types.h
  verbleveldlg.h
  txitem.h
  txitemdlg.h
  xmlsyntaxhighlight.h)

set (qpfGuiLib_ui
  mainwindow.ui
  dlgdataing.ui
  dlgshowtaskinfo.ui
  dlgreproc.ui
  dlguserpwd.ui
  dlgqdtfilter.ui
  frm_diagflt.ui
  configtool.ui
  browserwidget.ui
  dbbrowser.ui
  dlgalert.ui
  conthostedit.ui
  swarmedit.ui
  ruleedit.ui
  exttooledit.ui
  exttoolsdef.ui
  multitextview.ui
  frm_filterset.ui
  frm_filter.ui
  frmagentstatus.ui
  frmhostinfo.ui
  verbleveldlg.ui
  logframe.ui
  txitem.ui)

set (qpfGuiLib_qrc
  configtool.qrc
  jsontree.qrc
  icon.qrc
  tx.qrc
  tb.qrc)

set (qpfGuiBin_src main.cpp)

add_definitions(-DQT_DEBUG)
add_definitions(-DCMAKE_BUILD_TYPE=Debug)

qt5_wrap_cpp(qpfGuiLib_hdr_moc ${qpfGuiLib_hdr})
qt5_wrap_ui (qpfGuiLib_ui_moc  ${qpfGuiLib_ui})
qt5_add_resources(qpfGuiLib_ui_qrc ${qpfGuiLib_qrc})

include_directories (. .. 
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR})
link_directories (${PSQLDIR}/lib ${ZMQDIR}/lib ${CURLDIR}/lib
                  ${UUIDDIR}/lib ${PCRE2DIR}/lib)

add_library (qpfGuiLib SHARED
  ${qpfGuiLib_src}
  ${qpfGuiLib_hdr_moc}
  ${qpfGuiLib_ui_moc}
  ${qpfGuiLib_ui_qrc})
target_link_libraries (qpfGuiLib Qt5::Widgets Qt5::Sql Qt5::Network Qt5::Xml) # Qt5::WebKit Qt5::WebKitWidgets)
#qt5_use_modules(qpfGuiLib Widgets Sql Network Xml WebKit)
target_include_directories (qpfGuiLib PUBLIC .
  ${JSON_ROOT_DIR}
  ${INFIX_ROOT_DIR}
  ${NNCOMM_ROOT_DIR}
  ${TOOLS_ROOT_DIR}
  ${STR_ROOT_DIR}
  ${LOG_ROOT_DIR}
  ${UUID_ROOT_DIR}
  ${FMK_ROOT_DIR}
  ${NNMSGINCDIR}
  ${PCRE2INCDIR}
  ${UUIDINCDIR}
  ${PSQLINCDIR})

add_executable(qpfgui ${qpfGuiBin_src})
add_dependencies (qpfgui qpfGuiLib)
target_include_directories (qpfgui PUBLIC .
  ${JSON_ROOT_DIR}
  ${INFIX_ROOT_DIR}
  ${NNCOMM_ROOT_DIR}
  ${TOOLS_ROOT_DIR}
  ${STR_ROOT_DIR}
  ${LOG_ROOT_DIR}
  ${UUID_ROOT_DIR}
  ${FMK_ROOT_DIR}
  ${NNMSGINCDIR}
  ${PCRE2INCDIR}
  ${UUIDINCDIR}
  ${PSQLINCDIR})
target_link_libraries (qpfgui qpfGuiLib 
  json infix str log tools uuid fmk nncomm
  nanomsg ${PCRE2LIB} ${PSQLLIB} pthread)
set_target_properties (qpfgui PROPERTIES LINKER_LANGUAGE CXX)

install (TARGETS qpfGuiLib qpfgui
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)
