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
#include <QtSql>

#include <iostream>
#include "config.h"


namespace QPF {

inline int randInt(int low, int high) {
    return qrand() % ((high + 1) - low) + low;
}

ArchiveModel::ArchiveModel(QTableView * t) : tblvw(t)
{
    //setupModel();
    //setupView();
    //setupArchiveWatcher();
}

void ArchiveModel::setupModel(QString t)
{
    static QSqlDatabase db;
    static QString dbId;
    static bool dbInitialized = false;
    static QString connectParams;

    if (! dbInitialized) {
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

    // DB connection is alive, assign and retrieve table
    QSqlTableModel * model = new QSqlTableModel(tblvw, db);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(db.driver()->escapeIdentifier(t, QSqlDriver::TableName));
    model->select();
    if (model->lastError().type() != QSqlError::NoError) {
        qDebug() << model->lastError().text();
    }
    tblvw->setModel(model);
    tblvw->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
    tblvw->resizeColumnsToContents();

    //connect(tblvw->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    //        this, SLOT(currentChanged()));
}

/*
void ArchiveModel::setupModel()
{
    model = new QStandardItemModel(0, NumOfMdataFieldNames);
    for (int i = 0; i < NumOfMdataFieldNames; ++i) {
        model->setHeaderData(i, Qt::Horizontal,
                             tr(MdataFieldNames[i]).toUpper());
    }
}

void ArchiveModel::setupView()
{
    tblvw->setModel(model);

    QItemSelectionModel * selModel = new QItemSelectionModel(model);
    tblvw->setSelectionModel(selModel);

    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    QHeaderView * hdrView = tblvw->horizontalHeader();
    hdrView->setStretchLastSection(true);
}

void ArchiveModel::setupArchiveWatcher()
{
    //dbFileName = "/home/jcgonzalez/qpfDB.ini";
    //fsWatcher = new QFileSystemWatcher;
    //connect(fsWatcher, SIGNAL(fileChanged(const QString &)),
    //    this, SLOT(updateView(const QString &)));
    //fsWatcher->addPath(dbFileName);

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(10000);

}

void ArchiveModel::updateView()
{
    static int nRows = 0;

    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    DBHandler * db = dbHdl.get();

    // Check that connection with the DB is possible
    try {
        db->openConnection();
    } catch (RuntimeException & e) {
        QMessageBox::warning(0, "Cannot open DB connection",
                             e.what());
        return;
    }

    // Get number of records in products table in DB
    int nNewRows = db->getNumRowsInTable("products_info");

    ProductList prodList;
    int numOfProductsInDB = 0;

    if (nNewRows > nRows) {
        nRows = nNewRows;

        // Try to retrieve all the products from the DB
        try {
            numOfProductsInDB = db->retrieveProducts(prodList, "");
        } catch (RuntimeException & e) {
            QMessageBox::warning(0, "Cannot retrieve product list from DB",
                                 e.what());
            numOfProductsInDB = 0;
        }
    }

    // Close connection
    db->closeConnection();

    int numOfProductsInView = model->rowCount();

    for (int k = numOfProductsInView; k < numOfProductsInDB; ++k) {
        model->insertRows(k, 1, QModelIndex());
        try {
        ProductMetadata & m = prodList.productList.at(k);

        model->setData(model->index(k, 0, QModelIndex()), QString::fromStdString(m.productId));
        model->setData(model->index(k, 1, QModelIndex()), QString::fromStdString(m.productType));
        model->setData(model->index(k, 2, QModelIndex()), QString::fromStdString(m.productStatus));
        model->setData(model->index(k, 3, QModelIndex()), QString::fromStdString(m.productVersion));
        model->setData(model->index(k, 4, QModelIndex()), QString("%1").arg(m.productSize));
        model->setData(model->index(k, 5, QModelIndex()), QString::fromStdString(m.creator));
        model->setData(model->index(k, 6, QModelIndex()), QString::fromStdString(m.instrument));
        model->setData(model->index(k, 7, QModelIndex()), QString::fromStdString(m.signature));
        model->setData(model->index(k, 8, QModelIndex()), QString::fromStdString(m.startTime));
        model->setData(model->index(k, 9, QModelIndex()), QString::fromStdString(m.endTime));
        model->setData(model->index(k, 10, QModelIndex()), QString::fromStdString(m.regTime));
        model->setData(model->index(k, 11, QModelIndex()), QString::fromStdString(m.url));
        } catch (RuntimeException & e) {
            QMessageBox::warning(0,
                               "ERROR",
                               QString("ERROR: %6\nnRows = %1, numOfProductsInDB = %2, "
                                       "numOfProductsInView = %3, k = %4, size = %5")
                               .arg(nRows).arg(numOfProductsInDB)
                               .arg(numOfProductsInView).arg(k)
                               .arg(prodList.productList.size())
                               .arg(e.what()));
        } catch(...) {
            QMessageBox::warning(0,
                               "ERROR",
                               QString("UNKNOWN ERROR!\nnRows = %1, numOfProductsInDB = %2, "
                                       "numOfProductsInView = %3, k = %4, size = %5")
                               .arg(nRows).arg(numOfProductsInDB)
                               .arg(numOfProductsInView).arg(k)
                               .arg(prodList.productList.size()));
        }
    }
}
*/

}
