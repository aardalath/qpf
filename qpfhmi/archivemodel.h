/******************************************************************************
 * File:    archivemodel.h
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

#ifndef ARCHIVEMODEL_H
#define ARCHIVEMODEL_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
// Qt Packages
//------------------------------------------------------------
#include <QStandardItemModel>
#include <QStringList>
#include <QTableView>
#include <QTreeView>
#include <QFileSystemWatcher>
#include <QtSql>

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------
#include "archivetree.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: Deployer
//==========================================================================
class ArchiveModel : public QObject
{
  Q_OBJECT

public:
    //----------------------------------------------------------------------
    // Constructor: ArchiveModel
    //----------------------------------------------------------------------
    ArchiveModel();

public:
    //----------------------------------------------------------------------
    // Method: setupModel
    // Setup a model for a Table View
    //----------------------------------------------------------------------
    void setupModel(QTableView * t, QString q,
                    QList<QString> headers = QList<QString>(),
                    bool isTable = true);

    //----------------------------------------------------------------------
    // Method: setupModel
    // Setup a model for a Tree View
    //----------------------------------------------------------------------
    void setupModel(QTreeView * tt, QString q,
                    QList<QString> headers = QList<QString>());

protected slots:
    //----------------------------------------------------------------------
    // Method: updateView (SLOT)
    // Slot to update the mode view in case of updates
    //----------------------------------------------------------------------
    void updateView();

private:
    //----------------------------------------------------------------------
    // Method: initializeDb
    // Initialize the connection to the database
    //----------------------------------------------------------------------
    void initializeDb();

    //----------------------------------------------------------------------
    // Method: startUpdateTimer
    // Initialize the connection to the database
    //----------------------------------------------------------------------
    void startUpdateTimer();

private:
    enum ModelMode { Table, Tree };

    //QStandardItemModel * model;
    QTableView *         tblvw;
    QTreeView *          trvw;
    QSqlTableModel *     model;
    QSqlQueryModel *     qmodel;
    ModelMode            mode;
    ArchiveTreeModel *   atModel;
    QList<QVariant>      rootData;
    QString              request;

    static QSqlDatabase  db;
    static QString       dbId;
    static bool          dbInitialized;
    static QString       connectParams;
};

}

#endif
