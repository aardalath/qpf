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
// Function: main
// Creates the application main class, parses the command line arguments
// and invokes the main window
//----------------------------------------------------------------------
int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    const QStringList & args = QCoreApplication::arguments();
    const int & numOfArgs = args.count();

    sayHello();
    
    if (numOfArgs > 4) {
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

        if (configStr.isEmpty() || sessionStr.isEmpty()) {
            usage();
        }
        
        QPF::MainWindow w(configStr, sessionStr);
        w.show();
        return a.exec();

    } else {
        usage();
    }

    return EXIT_SUCCESS;
}
