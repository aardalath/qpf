/******************************************************************************
 * File:    dbtreemodel.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.dbtreemodel
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
 *   Provides object implementation for some declarations
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

#include "dbtreemodel.h"

#include "dbmng.h"
#include <QDebug>

namespace QPF {

DBTreeModel::DBTreeModel(QString q, QStringList hdr) :
    queryString(q),
    headerLabels(hdr),
    rowsFromQuery(-1)
{
    refresh();
}

void DBTreeModel::setHeaders(QStringList & hdr)
{
    int i = 0;
    for (QString & s: hdr) {
        setHeaderData(i, Qt::Horizontal, s);
        ++i;
    }
}

void DBTreeModel::defineHeaders(QStringList hdr)
{
    headerLabels = hdr;
}

void DBTreeModel::defineQuery(QString q)
{
    queryString = q;
}

void DBTreeModel::refresh()
{
    if (! queryString.isEmpty()) {
        execQuery(queryString, DBManager::getDB());
    }
}

void DBTreeModel::execQuery(QString & qry, QSqlDatabase & db)
{
    // Perform query
    QSqlQuery q(qry, db);
    QSqlRecord rec = q.record();
    int fldCount = rec.count();

    if (q.size() == rowsFromQuery) { return; }

    clear();

    // Create root item
    QStandardItem * root = invisibleRootItem();
    QStandardItem * parent;
    QList<QStandardItem *> row;
    QString prevGrp("");

    int children = 0;
    rowsFromQuery = 0;
    while (q.next()) {
        QString grp = q.value(0).toString();
        if (prevGrp != grp) {
            parent = new QStandardItem(grp);
            row.clear();
            row << parent;
            for (int i = 1; i < fldCount; ++i) { row << 0; }
            root->appendRow(row);
            children = 0;
            prevGrp = grp;
        }
        row.clear();
        for (int i = 0; i < fldCount; ++i) { row << new QStandardItem(q.value(i).toString()); }
        parent->appendRow(row);
        ++rowsFromQuery;
        ++children;
    }

    if (headerLabels.count() > 0) {
        setHeaders(headerLabels);
    }
}

}
