#======================================================================
# SDC - Structured Data Container
#======================================================================
# Library project file
# Author: J C Gonzalez - 2015
#======================================================================

include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

TEMPLATE = lib

TARGET = sdc

INCLUDEPATH += . ../json

HEADERS += \
    sdc.h \
    array.h \
    dict.h \
    value.h \
    valueio.h \
    valueiojsoncpp.h
SOURCES += \
    array.cpp \
    dict.cpp \
    value.cpp \
    valueiojsoncpp.cpp
