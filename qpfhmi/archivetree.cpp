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

#include "archivetree.h"

#include <QDebug>
#include <QTime>
#include <QTimer>

#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QSqlRecord>

#include <iostream>
#include "config.h"


namespace QPF {

ArchiveTreeItem::ArchiveTreeItem(const QList<QVariant> &data, ArchiveTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

ArchiveTreeItem::~ArchiveTreeItem()
{
    qDeleteAll(m_childItems);
}

void ArchiveTreeItem::appendChild(ArchiveTreeItem *child)
{
    m_childItems.append(child);
}

ArchiveTreeItem * ArchiveTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int ArchiveTreeItem::childCount() const
{
    return m_childItems.count();
}

int ArchiveTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant ArchiveTreeItem::data(int column) const
{
        return m_itemData.value(column);
}

ArchiveTreeItem * ArchiveTreeItem::parentItem()
{
    return m_parentItem;
}

int ArchiveTreeItem::row() const
{
    return (m_parentItem ?
            m_parentItem->m_childItems.indexOf(const_cast<ArchiveTreeItem*>(this)) :
            0);
}

//////////////////////////////////////////////////////////////////////////////

ArchiveTreeModel::ArchiveTreeModel(QList<QVariant> & rootData,
                                   QObject *parent)
    : QAbstractItemModel(parent), numCols(0), rootItem(0),
      numOfRecords(-1), lastExtSignature(""), lastParent(0)

{
    numCols = rootData.count();
    rootItem = new ArchiveTreeItem(rootData);
}

ArchiveTreeModel::~ArchiveTreeModel()
{
    delete rootItem;
}

int ArchiveTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<ArchiveTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant ArchiveTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    ArchiveTreeItem *item = static_cast<ArchiveTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags ArchiveTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ArchiveTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ArchiveTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ArchiveTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ArchiveTreeItem*>(parent.internalPointer());

    ArchiveTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ArchiveTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ArchiveTreeItem *childItem = static_cast<ArchiveTreeItem*>(index.internalPointer());
    ArchiveTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ArchiveTreeModel::rowCount(const QModelIndex &parent) const
{
    ArchiveTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ArchiveTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void ArchiveTreeModel::setSourceModel(QSqlQueryModel * qmodel)
{
    populateModel(qmodel, rootItem);
}

void ArchiveTreeModel::populateModel(QSqlQueryModel * qmodel, ArchiveTreeItem * parent)
{
    QList<QVariant> itemData;
    int rows = qmodel->rowCount();

    //if (parent != 0) { lastIndex = index(0, 0); }

    if (numOfRecords < rows) {
        beginResetModel();
        for (int i = numOfRecords + 1; i < rows; ++i) {
            QString extSignature = qmodel->data(qmodel->index(i, 0)).toString();
            if (extSignature != lastExtSignature) {
                // Create new branch
                itemData.clear();
                itemData << extSignature;
                for (int j = 1; j < numCols; ++j) { itemData << QVariant(); }
                lastParent = new ArchiveTreeItem(itemData, parent);
                parent->appendChild(lastParent);
                lastExtSignature = extSignature;
            }
            itemData.clear();
            // Add new leaf
            for (int j = 0; j < numCols; ++j) {
                itemData << qmodel->data(qmodel->index(i, j));
            }
            lastParent->appendChild(new ArchiveTreeItem(itemData, lastParent));
        }
        numOfRecords = rows;
        endResetModel();
    }

}

}
