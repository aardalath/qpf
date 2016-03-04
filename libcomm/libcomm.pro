#======================================================================
# LibComm - small library for node communication
#======================================================================
# Library project file
# Author: J C Gonzalez - 2015
#======================================================================

include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

TEMPLATE = lib

TARGET = libcomm

LIBS += -lzmq

HEADERS += \
        commnode.h \
        common.h \
        error.h \
        log.h \
        propdef.h \
        r2rpeer.h \
        statem.h \
        thread.h \
        tools.h \
        version.h \
    counter.h
SOURCES += \
        commnode.cpp \
        log.cpp \
        r2rpeer.cpp \
        statem.cpp \
        thread.cpp \
        tools.cpp \
    counter.cpp
