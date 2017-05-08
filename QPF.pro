#======================================================================
# QPF - Prototype of QLA Processing Framework
#======================================================================
# Project file
# Author: J C Gonzalez - 2015,2016,2017 
#======================================================================

TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = \
    libcomm \
    json \
    sdc \
    infix \
    src \
    qpf 

hmi { SUBDIRS += qpfgui }

#    qpfhmi \

#SUBDIRS += tests

libcomm.depends = json
sdc.depends = json
qpfhmi.depends = libcomm json sdc infix src
qpf.depends = libcomm json sdc infix src
tests.depends = libcomm sdc infix src
qpfgui.depends = src

OTHER_FILES += \
    defaults.pri
