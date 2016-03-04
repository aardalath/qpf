/******************************************************************************
 * File:    taskmonmodel.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskmonmodel
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
 *   Implement TaskMonModel class
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

#ifndef TASKMONMODEL_H
#define TASKMONMODEL_H

#include <QStandardItemModel>
#include <QStringList>
#include <QTableView>
#include <QFileSystemWatcher>

namespace QPF {
/*
class TaskMonModel : public QObject
{
  Q_OBJECT

public:
  TaskMonModel(QTableView * t);

public:
  void populateModel(DPFDemo::ExternalFiles * list = 0);
  // QList<QStandardItem *> prepareRow(const QStringList & data,
  // 				    bool highlight = false);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
  bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
  DPFDemo::ExternalFiles getExtFiles();
  QStringList getExtFields();

private:
  DPFDemo::ExternalFiles extFiles;
  static const char * Headers [];
  static const char * metadataFields [];
  static int NumOfFields;



protected:
  void setupModel();
  void setupView();
  void setupArchiveWatcher();

protected slots:
  void updateView(const QString & path);

private:
  QStandardItemModel * model;
  QTableView *         tblvw;
  QString              dbFileName;
  QFileSystemWatcher * fsWatcher;
};
*/
}

#endif
