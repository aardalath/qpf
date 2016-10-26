/******************************************************************************
 * File:    configtool.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfgui.configtool
 *
 * Version: 1.0
 *
 * Date:    2015/09/01
 *
 * Copyright (C) 2015, 2016 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare ConfigTool UI class
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

#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QDialog>

#include <QtWidgets>
#include <QCheckBox>

#include "exttooledit.h"

class ModelView : public QWidget {
    Q_OBJECT
public:
    enum Type { List, Table };

    ModelView(QVector<QStringList> & vdlist,
              QAbstractItemView  * v = 0,
              QAbstractItemModel * m = 0);
    ModelView(QStringList & dlist,
              QAbstractItemView  * v = 0,
              QAbstractItemModel * m = 0);

    QAbstractItemModel *  getModel() { return model; }
    QAbstractItemView *   getView()  { return view; }
    QItemSelectionModel * getSelectionModel() { return selectionModel; }

    void setHeader(QString h);
    void setHeaders(QStringList & hlist);

private:
    void setData(QStringList & dlist);
    void setData(QVector<QStringList> & vdlist);

    void setupModel();
    void setupViews();

    QAbstractItemView  *view;
    QAbstractItemModel *model;
    QItemSelectionModel *selectionModel;
    Type type;
};

namespace Ui {
class ConfigTool;
}

namespace QPF {

class ConfigTool : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigTool(QWidget *parent = 0);
    ~ConfigTool();

    enum PageIndex {
        PageGeneral,
        PageMachines,
        PageProdProc,
        PageNetwork,
        PageOrchestration,
        PageExtTools,
        PageStorage,
        PageFlags,
    };

    void readConfig();

    void initExtTools(MapOfUserDefTools & userTools, QStringList pts);
    void getExtTools(MapOfUserDefTools & userTools);

public slots:
    void save();
    void saveAs();
    void apply();
    void setWorkingPaths(QString newPath);

private slots:
    void addNewTool();
    void editTool(QModelIndex idx);
    void editTool();
    void editTool(int row);
    void removeTool();
    void cancelDlg();
    void changeToolWithItem(QTableWidgetItem * item);

private:
    ModelView * createListModelView(QAbstractItemView * v,
                                    QStringList & dlist,
                                    QString hdr);

    ModelView * createTableModelView(QAbstractItemView * v,
                                     QVector<QStringList> & vdlist,
                                     QStringList & hdr);

    void transferFlagsFromCfgToGUI();
    void transferFlagsFromGUIToCfg();

    struct FlagSt {
        std::string  msgName;
        QCheckBox  * chkDisk;
        QCheckBox  * chkDB;
    };

private:
    Ui::ConfigTool *ui;

    MapOfUserDefTools userDefTools;
    MapOfUserDefTools origDefTools;
    QStringList       prodTypes;

    static QVector<FlagSt> monitMsgFlags;
};

}

#endif // CONFIGTOOL_H
