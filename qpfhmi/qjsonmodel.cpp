/***********************************************
    Copyright (C) 2014  Schutz Sacha
    This file is part of QJsonModel (https://github.com/dridk/QJsonmodel).

    QJsonModel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QJsonModel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QJsonModel.  If not, see <http://www.gnu.org/licenses/>.

**********************************************/

#include "qjsonmodel.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIcon>
#include <QFont>
#include <iostream>

QJsonModel::QJsonModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    mRootItem = new QJsonTreeItem;
    mHeaders.append("key");
    mHeaders.append("value");


}

bool QJsonModel::load(const QString &fileName)
{
    QFile file(fileName);
    bool success = false;
    if (file.open(QIODevice::ReadOnly)) {
        success = load(&file);
        file.close();
    }
    else success = false;

    return success;
}

bool QJsonModel::load(QIODevice *device)
{
    return loadJson(device->readAll());
}

bool QJsonModel::loadJson(const QByteArray &json)
{
    mDocument = QJsonDocument::fromJson(json);

    if (!mDocument.isNull())
    {
        beginResetModel();
        if (mDocument.isArray()) {
            mRootItem = QJsonTreeItem::load(QJsonValue(mDocument.array()));
        } else {
            mRootItem = QJsonTreeItem::load(QJsonValue(mDocument.object()));
        }
        endResetModel();
        return true;
    }
    return false;
}


QVariant QJsonModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();


    QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());


    if ((role == Qt::DecorationRole) && (index.column() == 0)){

        return mTypeIcons.value(item->type());
    }


    if (role == Qt::DisplayRole) {

        if (index.column() == 0)
            return QString("%1").arg(item->key());

        if (index.column() == 1)
            return QString("%1").arg(item->value());
    }



    return QVariant();

}

QVariant QJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {

        return mHeaders.value(section);
    }
    else
        return QVariant();
}

QModelIndex QJsonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QJsonTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    QJsonTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex QJsonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QJsonTreeItem *childItem = static_cast<QJsonTreeItem*>(index.internalPointer());
    QJsonTreeItem *parentItem = childItem->parent();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QJsonModel::rowCount(const QModelIndex &parent) const
{
    QJsonTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int QJsonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

void QJsonModel::setIcon(const QJsonValue::Type &type, const QIcon &icon)
{
    mTypeIcons.insert(type,icon);
}

QModelIndex QJsonModel::find(QAbstractItemModel * model, QString content,
                             QModelIndex startNode = QModelIndex())
{
    QModelIndex finalIdx = QModelIndex();
    
    for (int r = 0; r < model->rowCount(startNode); ++r) {
        QModelIndex idx = model->index(r, 0, startNode);
        QVariant value = idx.data();
        
        // Compare content
        if (value.toString() == content) { return model->index(idx.row(),
                                                             idx.column(),
                                                             startNode); }
        
        if (model->hasChildren(idx)) {
            finalIdx = find(model, content, idx);
            if (finalIdx != QModelIndex()) { return model->index(finalIdx.row(),
                                                               finalIdx.column(),
                                                               startNode); }
        }
    }
    
    return finalIdx;
}

bool QJsonModel::findSequence(QAbstractItemModel * model, QStringList & seq,
                              QList<QModelIndex> & idxs)
{
    QModelIndex idx = QModelIndex();
    foreach (QString s, seq) {
        idx = find(model, s, idx);
        if (idx == QModelIndex()) { return false; }
        idxs.append(idx);
    }
    return idxs.count() > 0;
}
