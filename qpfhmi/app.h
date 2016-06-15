/******************************************************************************
 * File:    app.h
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
 *   Declares App application wrapper class
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

#ifndef APP_H
#define APP_H

#include <QApplication>
#include "deployer.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: App
//==========================================================================
class App Q_DECL_FINAL : public QApplication {
    Q_OBJECT
public:
    App(int& argc, char** argv);
    virtual bool notify(QObject *receiver, QEvent *event) Q_DECL_OVERRIDE;
    virtual char * getConfigFileName();
    virtual Configuration * getConfigHandler();
    virtual void restart();

private:
    Deployer * deployer;
};

}

#endif // APP_H
