#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T10:08:15
#
#-------------------------------------------------

include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

TEMPLATE = app

TARGET = qpf

SOURCES += \
    main.cpp \
    deployer.cpp \

HEADERS  += \
    deployer.h \
    init.h

INCLUDEPATH += . ../src $$LIBCOMMINC $$SDCINC $$INFIXINC $$JSONCPPINC $$PSQLCPPINC

LIBS += -L../src -lQPF \
        -L$$LIBCOMMLIB -llibcomm \
        -L$$JSONCPPLIB -ljson \
        -L$$SDCLIB -lsdc \
        -L$$INFIXLIB -linfix \
        -L$$PSQLCPPLIB -l$$PSQLLIB \
        -L/opt/cots/lib -lzmq -lcurl -luuid
