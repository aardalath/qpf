#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T10:08:15
#
#-------------------------------------------------

include(../defaults.pri)

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qpfhmi
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    hmipxy.cpp \
    logwatcher.cpp \
    archivemodel.cpp \
    browser.cpp \
    connectionwidget.cpp \
    taskmonmodel.cpp \
    types.cpp \
    progbardlg.cpp \
    dlgshowtaskinfo.cpp \
    app.cpp \
    deployer.cpp \
    frmagentstatus.cpp \
    simindata.cpp \
    textview.cpp \
    logframe.cpp \
    configtool.cpp \
    dbbrowser.cpp \
    exttoolsdef.cpp \
    verbleveldlg.cpp \
    testrundlg.cpp

HEADERS  += \
    mainwindow.h \
    hmipxy.h \
    logwatcher.h \
    archivemodel.h \
    browser.h \
    connectionwidget.h \
    taskmonmodel.h \
    types.h \
    progbardlg.h \
    dlgshowtaskinfo.h \
    app.h \
    deployer.h \
    frmagentstatus.h \
    simindata.h \
    textview.h \
    logframe.h \
    configtool.h \
    dbbrowser.h \
    exttoolsdef.h \
    verbleveldlg.h \
    testrundlg.h

FORMS    += \
    mainwindow.ui \
    browserwidget.ui \
    dlgshowtaskinfo.ui \
    frmagentstatus.ui \
    logframe.ui \
    configtool.ui \
    dbbrowser.ui \
    exttoolsdef.ui \
    verbleveldlg.ui \
    testrundlg.ui

INCLUDEPATH += . ../src $$LIBCOMMINC $$SDCINC $$INFIXINC $$JSONCPPINC $$PSQLCPPINC

LIBS += -L../src -lQPF \
        -L$$LIBCOMMLIB -llibcomm \
        -L$$JSONCPPLIB -ljson \
        -L$$SDCLIB -lsdc \
        -L$$INFIXLIB -linfix \
        -L$$PSQLCPPLIB -l$$PSQLLIB \
        -L/opt/cots/lib -lzmq -lcurl -luuid

RESOURCES += \
    configtool.qrc
