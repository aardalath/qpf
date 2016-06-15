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

#include <QDebug>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QSqlQueryModel>

namespace QPF {

class ArchiveTreeItem {
public:
    explicit ArchiveTreeItem(const QList<QVariant> &data, ArchiveTreeItem *parent = 0);
    ~ArchiveTreeItem();
    void appendChild(ArchiveTreeItem *child);
    ArchiveTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ArchiveTreeItem *parentItem();

private:
    QList<ArchiveTreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    ArchiveTreeItem *m_parentItem;
};

class ArchiveTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ArchiveTreeModel(QList<QVariant> & rootData, QObject *parent = 0);
    ~ArchiveTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void setSourceModel(QSqlQueryModel * qmodel);

private:
    void populateModel(QSqlQueryModel * model, ArchiveTreeItem * parent);

private:
    int               numCols;
    ArchiveTreeItem * rootItem;
    int               numOfRecords     = -1;
    QString           lastExtSignature = "";
    ArchiveTreeItem * lastParent       = 0;
    QModelIndex       lastIndex;

};

}
