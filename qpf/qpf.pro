include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

HEADERS += deployer.h

SOURCES += deployer.cpp main.cpp

INCLUDEPATH += . ../src $$LIBCOMMINC $$JSONCPPINC

LIBS += -L../src -lQPF \
        -L$$LIBCOMMLIB -llibcomm \
        -L$$JSONCPPLIB -ljson \
        -L/opt/cots/lib -lzmq
