#======================================================================
# QPF - Prototype of QLA Processing Framework
#======================================================================
# Project file
# Author: J C Gonzalez - 2015
#======================================================================

TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = \
    libcomm \
    json \
    sdc \
    infix \
    src \
    tests \
#    qpf \
    qpfhmi

tests.depends = libcomm sdc infix src
# qpf.depends = src json sdc
sdc.depends = json
qpfhmi.depends = libcomm json sdc infix src
tests.depends = libcomm sdc infix src

OTHER_FILES += \
    defaults.pri
