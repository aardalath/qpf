#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T10:08:15
#
#-------------------------------------------------

include(../defaults.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qpfhmi
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
        hmipxy.cpp \
        logwatcher.cpp \
    archivemodel.cpp \
    taskmonmodel.cpp \
    types.cpp \
    progbardlg.cpp \
    dlgshowtaskinfo.cpp \
    app.cpp \
    deployer.cpp \
    frmagentstatus.cpp \
    simindata.cpp \
    textview.cpp

HEADERS  += mainwindow.h \
         hmipxy.h \
         logwatcher.h \
    archivemodel.h \
    taskmonmodel.h \
    types.h \
    progbardlg.h \
    dlgshowtaskinfo.h \
    app.h \
    deployer.h \
    frmagentstatus.h \
    simindata.h \
    textview.h

FORMS    += mainwindow.ui \
    dlgshowtaskinfo.ui \
    frmagentstatus.ui \
    logframe.ui

INCLUDEPATH += . ../src $$LIBCOMMINC $$JSONCPPINC $$PSQLCPPINC

LIBS += -L../src -lQPF \
        -L$$LIBCOMMLIB -llibcomm \
        -L$$JSONCPPLIB -ljson \
        -L$$PSQLCPPLIB -l$$PSQLLIB \
        -L/opt/cots/lib -lzmq -liniparser

