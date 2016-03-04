#======================================================================
# QPF - small toy QLA Processing Framework
#======================================================================
# Library project file
# Author: J C Gonzalez - 2015
#======================================================================

include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

TEMPLATE = lib

TARGET = QPF

LIBS += /opt/cots/lib/libzmq.so -lzmq

INCLUDEPATH += . $$LIBCOMMINC $$JSONCPPINC $$SDCINC $$PSQLCPPINC

LIBS += -L$$LIBCOMMLIB -llibcomm \
        -L$$JSONCPPLIB -ljson \
        -L$$SDCLIB -lsdc \
        -L$$PSQLCPPLIB -l$$PSQLLIB \
        -L/opt/cots/lib -lzmq

HEADERS += \
        common.h \
        dbhdl.h \
        error.h \
        msgtypes.h \
        propdef.h \
        version.h \
    procelem.h \
    cfg.h \
    dbg.h \
    urlhdl.h \
    cfginfo.h \
    filetransfer.h

HEADERS += \
        config.h \
        component.h \
        dbhdlpostgre.h \
        except.h \
        evtmng.h \
        datamng.h \
        logmng.h \
        taskagent.h \
        taskmng.h \
        taskorc.h

SOURCES +=  \
        config.cpp \
        component.cpp \
        dbhdlpostgre.cpp \
        except.cpp \
        evtmng.cpp \
        datamng.cpp \
        logmng.cpp \
        taskagent.cpp \
        taskmng.cpp \
        taskorc.cpp \
    procelem.cpp \
    urlhdl.cpp \
    dbg.cpp \
    cfginfo.cpp
