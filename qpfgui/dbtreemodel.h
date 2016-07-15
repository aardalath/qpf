/******************************************************************************
 * File:    dbtreemodel.h
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
 *   Declaration of several datadbtreemodel for QPF HMI
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
#ifndef DBTREEMODEL_H
#define DBTREEMODEL_H

#include <QStandardItemModel>
#include <QSqlQuery>

namespace QPF {

class DBTreeModel : public QStandardItemModel {

    Q_OBJECT

public:
    explicit DBTreeModel(QString q = QString(""),
                         QStringList hdr = QStringList());

    void refresh();

    void defineHeaders(QStringList hdr);
    void defineQuery(QString q);

protected:
    void setHeaders(QStringList & hdr);

    //virtual QList<QStandardItem *> prepareRow(QStringList & l) = 0;
    virtual void execQuery(QString & qry, QSqlDatabase & db);

    QString     queryString;
    QStringList headerLabels;
    int         rowsFromQuery;
};

}

#endif // DBTREEMODEL_H
