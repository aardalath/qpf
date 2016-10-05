#======================================================================
# CMakeLists.txt
# QPF - Prototype of QLA Processing Framework
# qpfgui - QPF HMI Application
#======================================================================
# Author: J C Gonzalez - 2015, 2016
# Copyright (C) 2015, 2016 Euclid SOC Team at ESAC
#======================================================================

project (qpfGui)

find_package (Qt5Widgets)
find_package (Qt5Sql)

set (qpfGuiLib_src
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
  exttooledit.cpp
  exttoolsdef.cpp
  frmagentstatus.cpp
  logframe.cpp
  logwatcher.cpp
  mainwindow.cpp
  procalertmodel.cpp
  proctskstatmodel.cpp
  productsmodel.cpp
  progbardlg.cpp
  qjsonitem.cpp
  qjsonmodel.cpp
  sysalertmodel.cpp
  textview.cpp
  txtblmodel.cpp
  types.cpp
  verbleveldlg.cpp
  txitem.cpp
  txitemdlg.cpp
  xmlsyntaxhighlight.cpp)

set (qpfGuiLib_hdr
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
  exttooledit.h
  exttoolsdef.h
  frmagentstatus.h
  hmitypes.h
  init.h
  logframe.h
  logwatcher.h
  mainwindow.h
  procalertmodel.h
  proctskstatmodel.h
  productsmodel.h
  progbardlg.h
  qjsonitem.h
  qjsonmodel.h
  sysalertmodel.h
  textview.h
  txtblmodel.h
  types.h
  verbleveldlg.h
  version.h
  txitem.h
  txitemdlg.h
  xmlsyntaxhighlight.h)

set (qpfGuiLib_ui
  mainwindow.ui
  dlgdataing.ui
  dlgshowtaskinfo.ui
  configtool.ui
  browserwidget.ui
  dbbrowser.ui
  dlgalert.ui
  exttooledit.ui
  exttoolsdef.ui
  frmagentstatus.ui
  verbleveldlg.ui
  logframe.ui
  txitem.ui)

set (qpfGuiLib_qrc
  configtool.qrc
  icon.qrc
  tx.qrc)

set (qpfGuiBin_src main.cpp)

qt5_wrap_cpp(qpfGuiLib_hdr_moc ${qpfGuiLib_hdr})
qt5_wrap_ui (qpfGuiLib_ui_moc  ${qpfGuiLib_ui})

include (../ext.cmake)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})
link_directories (${PSQLDIR}/lib ${ZMQDIR}/lib ${CURLDIR}/lib
                  ${UUIDDIR}/lib ${PCRE2DIR}/lib)

add_library (qpfGuiLib SHARED
  ${qpfGuiLib_src}
  ${qpfGuiLib_hdr_moc}
  ${qpfGuiLib_ui_moc})
#target_link_libraries (qpfGuiLib Qt5::Widgets Qt5::Sql)
qt5_use_modules(qpfGuiLib Widgets Sql Network Xml)
target_include_directories (qpfGuiLib PUBLIC ../src ../libcomm ../json ../sdc ../infix
  ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
  ${UUIDDIR}/include ${PCRE2DIR}/include)

add_executable(qpfgui ${qpfGuiBin_src})
add_dependencies (qpfgui qpfGuiLib)
target_include_directories (qpfgui PUBLIC ../src ../libcomm ../json ../sdc ../infix
  ${PSQLDIR}/include ${ZMQDIR}/include ${CURLDIR}/include
  ${UUIDDIR}/include ${PCRE2DIR}/include)
target_link_libraries (qpfgui qpfGuiLib qpf libcomm json sdc infix ${PSQLLIB} zmq curl uuid pcre2-posix)
#target_link_libraries (qpfcore qpf libcomm json sdc infix ${PSQLLIB} zmq curl uuid pcre2-posix)

install (TARGETS qpfGuiLib qpfgui
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib)

#CONFIG += c++11

#INCLUDEPATH += . ../src $$LIBCOMMINC $$SDCINC $$INFIXINC $$JSONCPPINC $$PSQLCP#PINC

#LIBS += -L../src -lQPF
#    -L$$LIBCOMMLIB -llibcomm
#    -L$$JSONCPPLIB -ljson
#    -L$$SDCLIB -lsdc
#    -L$$INFIXLIB -linfix
#    -L$$PSQLCPPLIB -l$$PSQLLIB
#    -L/opt/cots/lib -lzmq -lcurl -luuid
#ADD_EXECUTABLE(qpfgui qpfgui_sources)
