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
#include <QFileSystemWatcher>

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------

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
    // Constructor: Deployer
    //----------------------------------------------------------------------
    ArchiveModel(QTableView * t);

protected:
    //----------------------------------------------------------------------
    // Method: setupModel
    // Initialises the underlying data model
    //----------------------------------------------------------------------
    void setupModel();

    //----------------------------------------------------------------------
    // Method: setupView
    // Initialises the view to look at the model data
    //----------------------------------------------------------------------
    void setupView();

    //----------------------------------------------------------------------
    // Method: setupArchiveWatcher
    // Initialises the archive watcher to detect updates
    //----------------------------------------------------------------------
    void setupArchiveWatcher();

protected slots:
    //----------------------------------------------------------------------
    // Method: updateView (SLOT)
    // Slot to update the mode view in case of updates
    //----------------------------------------------------------------------
    void updateView();

private:
    QStandardItemModel * model;
    QTableView *         tblvw;
    QString              dbFileName;
    QFileSystemWatcher * fsWatcher;
};

}

#endif
