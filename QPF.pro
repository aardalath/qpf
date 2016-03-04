#======================================================================
# QPF - small toy QLA Processing Framework
#======================================================================
# Project file
# Author: J C Gonzalez - 2015
#======================================================================

TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = \
    src \
    json \
    tests \
    qpf \
    qpfhmi

tests.depends = src
qpf.depends = src json
qpfhmi.depends = src json

OTHER_FILES += \
    defaults.pri

    
