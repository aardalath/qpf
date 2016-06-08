/******************************************************************************
 * File:    archivemodel.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.archivemodel
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
 *   Implement ArchiveModel class
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

#include "archivemodel.h"
#include "datatypes.h"
#include "types.h"
#include "dbhdlpostgre.h"
#include "except.h"

#include <QDebug>
#include <QTime>
#include <QTimer>

#include <QHeaderView>
#include <QFile>
#include <QMessageBox>

#include <iostream>
#include "config.h"


namespace QPF {

QSqlDatabase  ArchiveModel::db;
QString       ArchiveModel::dbId;
bool          ArchiveModel::dbInitialized = false;
QString       ArchiveModel::connectParams;

//----------------------------------------------------------------------
// Constructor: ArchiveModel
//----------------------------------------------------------------------
ArchiveModel::ArchiveModel()
    : tblvw(0), trvw(0), model(0), qmodel(0), atModel(0)
{
    if (! dbInitialized) { initializeDb(); }
}

//----------------------------------------------------------------------
// Method: setupModel
// Setup a model for a Table View
//----------------------------------------------------------------------
void ArchiveModel::setupModel(QTableView * t, QString q,
                              QList<QString> headers,
                              bool isTable)
{
    // DB connection is alive, assign and retrieve table
    mode = Table;
    tblvw = t;
    model = new QSqlTableModel(tblvw, db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (isTable) {
        model->setTable(db.driver()->escapeIdentifier(q, QSqlDriver::TableName));
    }

    int n = 0;
    foreach(QString s, headers) {
        rootData << QVariant(s);
        model->setHeaderData(n, Qt::Horizontal, s);
        ++n;
    }

    tblvw->setModel(model);
    tblvw->resizeColumnsToContents();

    updateView();
    startUpdateTimer();

    //tblvw->setEditTriggers(QAbstractItemView::DoubleClicked |
    //                       QAbstractItemView::EditKeyPressed);

    //connect(tblvw->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    //        this, SLOT(currentChanged()));
}

//----------------------------------------------------------------------
// Method: setupModel
// Setup a model for a Tree View
//----------------------------------------------------------------------
void ArchiveModel::setupModel(QTreeView * tt, QString q,
                              QList<QString> headers)
{
    // DB connection is alive, assign and retrieve table
    mode = Tree;
    trvw = tt;
    request = q;

    rootData.clear();
    foreach(QString s, headers) { rootData << QVariant(s); }

    atModel = new ArchiveTreeModel(rootData, this);

    updateView();
    startUpdateTimer();
}

//----------------------------------------------------------------------
// Method: updateView (SLOT)
// Slot to update the mode view in case of updates
//----------------------------------------------------------------------
void ArchiveModel::updateView()
{
    // Take data and show them in table
    if (mode == Table) {
        model->select();
        if (model->lastError().type() != QSqlError::NoError) {
            qDebug() << model->lastError().text();
        }
        tblvw->resizeColumnsToContents();
    } else {
        if (qmodel != 0) { delete qmodel; }
        QSqlQuery qry(request, db);
        qmodel = new QSqlQueryModel;
        qmodel->setQuery(qry);
        trvw->setModel(atModel);
        atModel->setSourceModel(qmodel);
    }
}

//----------------------------------------------------------------------
// Method: initializeDb
// Initialize the connection to the database
//----------------------------------------------------------------------
void ArchiveModel::initializeDb()
{
    QString dbName ( QPF::Configuration::DBName.c_str() );
    QString user   ( QPF::Configuration::DBUser.c_str() );
    QString passwd ( QPF::Configuration::DBPwd.c_str() );
    QString host   ( QPF::Configuration::DBHost.c_str() );
    QString port   ( QPF::Configuration::DBPort.c_str() );

    QSqlError err;
    dbId = "QPF";
    db = QSqlDatabase::addDatabase("QPSQL", dbId);
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port.toUInt());
    if (! db.open(user, passwd)) {
        if (db.lastError().isValid()) {
            qDebug() << db.lastError();
            abort();
        }
        // db = QSqlDatabase();
        // QSqlDatabase::removeDatabase(dbId);
    } else {
        dbInitialized = true;
    }
    connectParams = QString("host=%1 port=%2 dbname=%3 user=%4 password=%5")
        .arg(host).arg(port).arg(dbName).arg(user).arg(passwd);
}

//----------------------------------------------------------------------
// Method: startUpdateTimer
// Initialize the connection to the database
//----------------------------------------------------------------------
void ArchiveModel::startUpdateTimer()
{
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(3000);
}

}
