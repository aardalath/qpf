/******************************************************************************
 * File:    main.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfgui.main
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015,2016 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Version constants
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "version.h"

#ifndef BUILD_ID
#define BUILD_ID ""
#endif

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSplashScreen>

#define TIME_LAPSE 400000

//----------------------------------------------------------------------
// Function: usage
// Shows the command line to be used to call this application
//----------------------------------------------------------------------
void usage()
{
    QString execName = QCoreApplication::arguments().at(0);
    std::cerr << "Usage: "
              << execName.toStdString()
              << " -c db://user:passwd@host:port/dbname"
              << std::endl;
    exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------
// Function: sayHello
// Shows a minimal title and build id for the application
//----------------------------------------------------------------------
void sayHello()
{
    std::string buildId(BUILD_ID);
    if (buildId.empty()) {
        char buf[20];
        sprintf(buf, "%ld", (long)(time(0)));
        buildId = std::string(buf);
    }

    std::string hline("----------------------------------------"
                      "--------------------------------------");
    std::cout << hline << std::endl
            << " " << APP_NAME << " - " << APP_LONG_NAME << std::endl
            << " " << APP_DATE << " - "
            << APP_RELEASE << " Build " << buildId << std::endl
            << hline << std::endl << std::endl;
}

//----------------------------------------------------------------------
// Function: ctrlMsg
// Shows a minimal title and build id for the application
//----------------------------------------------------------------------
void ctrlMsg(QSplashScreen & splash, int k = 0)
{
    QString msgs[] = {"Loading system configuration",
                      "Connecting to database",
                      "Checking components running state",
                      "Preparing graphical user interface",
                      "Launching application window",
                      ""};
    static int i = 0;

    static QString appMsg(APP_NAME " - " APP_DATE " - "
                          APP_RELEASE " " BUILD_ID);

    QString msgToShow = "<font color=\"yellow\"><b>" + appMsg + "</b></font><br>" +
                        "<font color=\"white\">" + msgs[i++] + "</font>";
    splash.showMessage(msgToShow, Qt::AlignLeft, Qt::white);
    qApp->processEvents(QEventLoop::AllEvents);
    QThread::usleep(TIME_LAPSE);
}

//----------------------------------------------------------------------
// Function: main
// Creates the application main class, parses the command line arguments
// and invokes the main window
//----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/img/EuclidQPF.png");
    QSplashScreen splash(pixmap);
    splash.show();

    const QStringList & args = QCoreApplication::arguments();
    const int & numOfArgs = args.count();

    sayHello();

    ctrlMsg(splash);

    QString configStr("");
    QString sessionStr("");

    for (int i = 1; i < numOfArgs; ++i) {
        const QString & tok = args.at(i);
        if (tok == "-c") {
            configStr = args.at(i + 1);
            ++i;
        } else if (tok == "-s") {
            sessionStr = args.at(i + 1);
            ++i;
        }
    }

    ctrlMsg(splash);

    if (configStr.isEmpty()) {
        usage();
    }

    ctrlMsg(splash);

    QPF::MainWindow w(configStr, sessionStr);

    ctrlMsg(splash);

    w.show();

    ctrlMsg(splash);
    splash.finish(&w);

    return a.exec();
}
