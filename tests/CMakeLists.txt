INCLUDE(../defaults.cmake OPTIONAL)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += ${GTESTDIR}/include
QMAKE_LIBDIR += /opt/cots/lib ${GTESTDIR}/lib
LIBS += -L../src -llibcomm -lgtest -lpthread
LIBS += -lzmq

SOURCES += main.cpp \
    sdc/test_array.cpp \
    sdc/test_dict.cpp \
    sdc/test_value.cpp \
    sdc/test_valueiojsoncpp.cpp \
    infix/test_infixeval.cpp \
    libcomm/test_commnode.cpp \
    libcomm/test_log.cpp \
    libcomm/test_r2rpeer.cpp \
    libcomm/test_statem.cpp \
    libcomm/test_thread.cpp \
    libcomm/test_tools.cpp \
    src/test_cfginfo.cpp \
    src/test_component.cpp \
    src/test_config.cpp \
    src/test_datamng.cpp \
    src/test_datatypes.cpp \
    src/test_dbg.cpp \
    src/test_dbhdlpostgre.cpp \
    src/test_evtmng.cpp \
    src/test_except.cpp \
    src/test_filenamespec.cpp \
    src/test_filetransfer.cpp \
    src/test_logmng.cpp \
    src/test_procelem.cpp \
    src/test_str.cpp \
    src/test_taskagent.cpp \
    src/test_taskmng.cpp \
    src/test_taskorc.cpp \
    src/test_urlhdl.cpp \
    src/test_uuid.cpp

HEADERS += \
    sdc/test_array.h \
    sdc/test_dict.h \
    sdc/test_value.h \
    sdc/test_valueiojsoncpp.h \
    infix/test_infixeval.h \
    libcomm/test_commnode.h \
    libcomm/test_log.h \
    libcomm/test_r2rpeer.h \
    libcomm/test_statem.h \
    libcomm/test_thread.h \
    libcomm/test_tools.h \
    src/test_cfginfo.h \
    src/test_component.h \
    src/test_config.h \
    src/test_datamng.h \
    src/test_datatypes.h \
    src/test_dbg.h \
    src/test_dbhdlpostgre.h \
    src/test_evtmng.h \
    src/test_except.h \
    src/test_filenamespec.h \
    src/test_filetransfer.h \
    src/test_logmng.h \
    src/test_procelem.h \
    src/test_str.h \
    src/test_taskagent.h \
    src/test_taskmng.h \
    src/test_taskorc.h \
    src/test_urlhdl.h \
    src/test_uuid.h

INCLUDEPATH += . ../src $$LIBCOMMINC $$SDCINC $$INFIXINC $$JSONCPPINC $$PSQLCPPINC

LIBS += -L../src -lQPF \
        -L$$LIBCOMMLIB -llibcomm \
        -L$$SDCLIB -lsdc \
        -L$$JSONCPPLIB -ljson \
        -L$$INFIXLIB -linfix \
        -L$$PSQLCPPLIB -l$$PSQLLIB \
        -L/opt/cots/lib -lzmq -lcurl -luuid

RESOURCES += \
    configtool.qrc
ADD_EXECUTABLE( _sources)
