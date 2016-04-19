/******************************************************************************
 * File:    main.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.QPF.main
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Create the Application object and run it, and show the HMI if needed
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

#include "app.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    int exitCode = 0;

    QPF::App app(argc, argv);

    QPF::MainWindow * w = 0;
    if (app.mustLaunchHMI()) {
        if (w != 0) { delete w; }
        w = new QPF::MainWindow(0, app.getConfigHandler());
        w->show();
    }

    exitCode = app.exec();

    if (exitCode == QPF::MainWindow::EXIT_CODE_RESTART) {
        app.restart();// Spawn a new instance of application
    }

    return exitCode;
}
