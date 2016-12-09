/******************************************************************************
 * File:    dbbrowser.cpp
 *          Implementation of class DBBrowser
 *
 * Domain:  QPF.qpfgui.dbbrowser
 *
 * Version: 1.1
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implementation of class DBBrowser
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
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "dbbrowser.h"
#include "ui_dbbrowser.h"
#include "config.h"

DBBrowser::DBBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBBrowser)
{
    ui->setupUi(this);

    if (QSqlDatabase::connectionNames().isEmpty()) {
        QString databaseName ( QPF::Configuration::DBName.c_str() );
        QString userName     ( QPF::Configuration::DBUser.c_str() );
        QString password     ( QPF::Configuration::DBPwd.c_str() );
        QString hostName     ( QPF::Configuration::DBHost.c_str() );
        QString port         ( QPF::Configuration::DBPort.c_str() );

        Browser::DBConnection connection = { "QPSQL",        // .driverName
                                             databaseName,
                                             userName,
                                             password,
                                             hostName,
                                             port.toInt() };
        ui->browser->addConnection(connection);
    }
}

DBBrowser::~DBBrowser()
{
    delete ui;
}
