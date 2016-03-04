/******************************************************************************
 * File:    app.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.app
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
 *   Implement App class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   QApplication
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

App::App(int& argc, char** argv) :
    QApplication(argc, argv), deployer(new Deployer(argc, argv))
{
}

bool App::notify(QObject *receiver, QEvent *event)
{
    try {
        return QApplication::notify(receiver, event);
    } catch (std::exception &e) {
        qFatal("Error %s sending event %s to object %s (%s)",
               e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
               typeid(*receiver).name());
    } catch (...) {
        qFatal("Error <unknown> sending event %s to object %s (%s)",
               typeid(*event).name(), qPrintable(receiver->objectName()),
               typeid(*receiver).name());
    }
    // qFatal aborts, so this isn't really necessary
    // but you might continue if you use a different logging lib
    return false;
}
