/******************************************************************************
 * File:    configtool.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.qpfgui.configtool
 *
 * Version:  1.2
 *
 * Date:    2015/09/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement ConfigTool UI class
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

#include "configtool.h"
#include "ui_configtool.h"

#include "tools.h"

#include "exttooledit.h"
#include "conthostedit.h"
#include "swarmedit.h"
#include "ruleedit.h"

#include "tskorc.h"

#include <QHostInfo>
#include <QFileInfo>
#include <QCheckBox>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QtCore/qnamespace.h>

#include "config.h"
using Configuration::cfg;

#define C(x) (x).c_str()
#define QS(x) QString::fromStdString(x)

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Method: flags
// Overwrites QAbstractItemModel::flags
//----------------------------------------------------------------------
Qt::ItemFlags StandardItemModel::flags(const QModelIndex&index) const
{
    Qt::ItemFlags flags;
    if (index.isValid()) {
        flags =  Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled; // | Qt::ItemIsEditable;
    } else {
        flags =  Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled  | Qt::ItemIsEnabled;
    }
    return flags;
}

//----------------------------------------------------------------------
// Method: flags
// Overwrites QAbstractItemModel::flags
//----------------------------------------------------------------------
Qt::ItemFlags StringListModel::flags(const QModelIndex&index) const
{
    Qt::ItemFlags flags;
    if (index.isValid()) {
        flags =  Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        flags =  Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled  | Qt::ItemIsEnabled;
    }
    return flags;
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ModelView::ModelView(QVector<QStringList> & vdlist,
                     QAbstractItemView  * v,
                     QAbstractItemModel * m)
    : view(v), model(m), type(Table)
{
    if (view  == 0) { view = new QTableView(0); }
    if (model == 0) { model = new StandardItemModel(vdlist.count(),
                                                     vdlist.at(0).count()); }
    setData(vdlist);
    view->setModel(model);
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ModelView::ModelView(QStringList & dlist,
                     QAbstractItemView  * v,
                     QAbstractItemModel * m)
    : view(v), model(m), type(List)
{
    if (view  == 0) { view  = new QListView(0); }
    if (model == 0) { model = new StringListModel(); }
    dynamic_cast<StringListModel*>(model)->setStringList(dlist);
    view->setModel(model);
}

//----------------------------------------------------------------------
// Method: setHeader
// Sets header in section 0
//----------------------------------------------------------------------
void ModelView::setHeader(QString h)
{
    model->setHeaderData(0, Qt::Horizontal, h);
}

//----------------------------------------------------------------------
// Method: setHeaders
// Sets column headers for the model
//----------------------------------------------------------------------
void ModelView::setHeaders(QStringList & hlist)
{
    for (int i = 0; i < hlist.count(); ++i) {
        model->setHeaderData(i, Qt::Horizontal, hlist.at(i));
    }
}

//----------------------------------------------------------------------
// Method: setData
// Sets the data for a list view
//----------------------------------------------------------------------
void ModelView::setData(QStringList & dlist)
{
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    for (int row = 0; row < dlist.count(); ++row) {
        model->insertRows(row, 1, QModelIndex());
        model->setData(model->index(row, 0, QModelIndex()), dlist.at(row));
    }
}

//----------------------------------------------------------------------
// Method: setData
// Sets the data for a table view
//----------------------------------------------------------------------
void ModelView::setData(QVector<QStringList> & vdlist)
{
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    for (int row = 0; row < vdlist.count(); ++row) {
        model->insertRows(row, 1, QModelIndex());
        const QStringList & rowdata = vdlist.at(row);
        for (int col = 0; col < rowdata.count(); ++col) {
            model->setData(model->index(row, col, QModelIndex()), rowdata.at(col));
        }
    }
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ConfigTool::ConfigTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigTool)
{
    ui->setupUi(this);

    monitMsgFlags.append(FlagSt({ "CMD",      ui->chkCmd }));    
    monitMsgFlags.append(FlagSt({ "EVTMNG",   ui->chkEvtMngCmd })); 
    monitMsgFlags.append(FlagSt({ "HMICMD",   ui->chkHmiCmd })); 
    monitMsgFlags.append(FlagSt({ "INDATA",   ui->chkInData }));                                        
    monitMsgFlags.append(FlagSt({ "TSKSCHED", ui->chkTskSched }));
    monitMsgFlags.append(FlagSt({ "TSKREG",   ui->chkTskReg })); 
    monitMsgFlags.append(FlagSt({ "TSKRQST",  ui->chkTskRqst }));                                       
    monitMsgFlags.append(FlagSt({ "TSKPROC",  ui->chkTskProc }));
    monitMsgFlags.append(FlagSt({ "TSKREP",   ui->chkTskRep })); 
    monitMsgFlags.append(FlagSt({ "FMKMON",   ui->chkFmkMon })); 
    monitMsgFlags.append(FlagSt({ "HOSTMON",  ui->chkHostMon }));
                                       
    ui->btngrpUserWA->setId(ui->rbtnUserWAAuto    , Auto);
    ui->btngrpUserWA->setId(ui->rbtnUserWALocal   , LocalFolder);
    ui->btngrpUserWA->setId(ui->rbtnUserWAVOSpace , VOSpaceFolder);

    connect(ui->tblwdgUserDefTools, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTool(QModelIndex)));
    connect(ui->tblviewHosts, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editHost(QModelIndex)));
    connect(ui->tblviewSwarms, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editSwarm(QModelIndex)));
    connect(ui->tblviewRules, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editRule(QModelIndex)));
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
ConfigTool::~ConfigTool()
{
    //delete ui;
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads config data from internals and places them into dialog
//----------------------------------------------------------------------
void ConfigTool::prepare(MapOfUserDefTools & userTools, QStringList pts)
{
    // Save current config data string
    cfgDataBackup = cfg.str();
    std::cerr << cfgDataBackup << std::endl;

    // Place config data in dialog
    transferCfgToGUI();
    initExtTools(userTools, pts);
}

//----------------------------------------------------------------------
// Method: initExtTools
// Init table of user defined / external tools
//----------------------------------------------------------------------
void ConfigTool::initExtTools(MapOfUserDefTools & userTools, QStringList pts)
{
    ui->tblwdgUserDefTools->clear();
    ui->tblwdgUserDefTools->setRowCount(userTools.count());
    ui->tblwdgUserDefTools->setColumnCount(5);
    ui->tblwdgUserDefTools->setHorizontalHeaderLabels(QStringList()
                                                      << "Name"
                                                      << "Description"
                                                      << "Executable"
                                                      << "Arguments"
                                                      << "Product types");
    int row = 0;
    foreach (QString key, userTools.keys()) {
        const QUserDefTool & udt = userTools.value(key);
        ui->tblwdgUserDefTools->setItem(row, 0, new QTableWidgetItem(udt.name));
        ui->tblwdgUserDefTools->setItem(row, 1, new QTableWidgetItem(udt.desc));
        ui->tblwdgUserDefTools->setItem(row, 2, new QTableWidgetItem(udt.exe));
        ui->tblwdgUserDefTools->setItem(row, 3, new QTableWidgetItem(udt.args));
        ui->tblwdgUserDefTools->setItem(row, 4, new QTableWidgetItem(udt.prod_types.join(QString("|"))));
        ++row;
        std::cerr << row << ": " << udt.name.toStdString() << '\n';
    }
    userDefTools = userTools;
    origDefTools = userTools;
    prodTypes = pts;
    connect(ui->tblwdgUserDefTools, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(changeToolWithItem(QTableWidgetItem*)));
}

//----------------------------------------------------------------------
// Method: getExtTools
// Allows external access to user defined tools
//----------------------------------------------------------------------
void ConfigTool::getExtTools(MapOfUserDefTools & userTools)
{
    userTools = userDefTools;
    userTools.detach();
    int row = 0;
    foreach (QString key, userTools.keys()) {
        const QUserDefTool & udt = userTools.value(key);
        ++row;
        std::cerr << row << ": " << udt.name.toStdString() << '\n';
    }
}

//----------------------------------------------------------------------
// Slot: save
// Saves the configuration data into the original file loaded
//----------------------------------------------------------------------
void ConfigTool::save()
{
    if (!cfg.isActualFile) {
        QString fileName(QString::fromStdString(cfg.cfgFileName));
        int ret = QMessageBox::question(this, tr("Save"),
                    tr("Do you want to overwrite the configuration file \"") +
                    fileName + QString("\"?"),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            saveAsFilename(fileName);
        }
    } else {
        saveAs();
    }
}

//----------------------------------------------------------------------
// Slot: saveAs
// Saves the configuration data into a new config file
//----------------------------------------------------------------------
void ConfigTool::saveAs()
{
    QFileInfo fs(QString::fromStdString(cfg.cfgFileName));
    QString filter("*.json");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
            fs.absolutePath(), filter);
    if (! fileName.isEmpty()) {
        saveAsFilename(fileName);
    }
}

//----------------------------------------------------------------------
// Slot: saveAsFilename
// Saves the configuration data into a new config file
//----------------------------------------------------------------------
void ConfigTool::saveAsFilename(QString & fName)
{
    if (! fName.isEmpty()) {
        if (!transferGUIToCfg()) { return; }
        QFile file( fName );
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << QS(cfg.str(true)) << endl;
        }
    }
}

//----------------------------------------------------------------------
// Slot: apply
// Moves the new config. info into the internals for immediate use
//----------------------------------------------------------------------
void ConfigTool::apply()
{
    if (transferGUIToCfg()) {
        accept();
    }
}

//----------------------------------------------------------------------
// Slot: closeDoNothing
// Closes the dialog, without modifying the configuration
//----------------------------------------------------------------------
void ConfigTool::closeDoNothing()
{
    reject();
}

//----------------------------------------------------------------------
// Slot: selectBasePath
// Select QPF base path
//----------------------------------------------------------------------
void ConfigTool::selectBasePath()
{
    QString pathName(QS(cfg.general.workArea()));
    pathName = QFileDialog::getExistingDirectory(this,
                                                 tr("Select QPF base path"),
                                                 pathName);
    QFileInfo fs(pathName);
    if (fs.exists()) {
        ui->edBasePath->setText(pathName);
        setWorkingPaths(pathName);
    }
}

//----------------------------------------------------------------------
// Slot: setWorkingPaths
// Sets working paths for a given base path
//----------------------------------------------------------------------
void ConfigTool::setWorkingPaths(QString newPath)
{
    ui->nedLocalArchiveFolder->setText(newPath + "/data/archive");
    ui->nedInbox->setText(newPath + "/data/inbox");
    ui->nedOutbox->setText(newPath + "/data/outbox");
    ui->nedUserReprocArea->setText(newPath + "/data/user");
}

//----------------------------------------------------------------------
// Slot: setWorkingPaths
// Sets working paths for a given base path
//----------------------------------------------------------------------
void ConfigTool::selectUserDefAreaPath()
{
    QString pathName(QS(cfg.general.workArea()));
    pathName = QFileDialog::getExistingDirectory(this,
                                                 tr("Select folder to be set as user-defined area"),
                                                 pathName);
    QFileInfo fs(pathName);
    if (fs.exists()) {
        ui->nedUserReprocArea->setText(pathName);
    }
}    

//----------------------------------------------------------------------
// Slot: setWorkingPaths
// Sets working paths for a given base path
//----------------------------------------------------------------------
void ConfigTool::defineUserWA(int btn)
{
    switch (btn) {
    case Auto:
        ui->tbtnUserDefAreaPath->setEnabled(false);
        ui->nedUserReprocArea->setReadOnly(true);
        break;
    case LocalFolder:
        ui->tbtnUserDefAreaPath->setEnabled(true);
        ui->nedUserReprocArea->setReadOnly(false);
        break;
    case VOSpaceFolder:
        ui->tbtnUserDefAreaPath->setEnabled(false);
        ui->nedUserReprocArea->setReadOnly(false);
        break;
    default:
        break;
    }

}

//----------------------------------------------------------------------
// Slot: selectIPythonCmd
// Select executable in local host corresponding to IPython binary
//----------------------------------------------------------------------
void ConfigTool::selectIPythonCmd()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Select IPython executable"),
                                            fileName);
    QFileInfo fs(fileName);
    if (fs.exists()) {
        ui->edIPythonCmd->setText(fileName);
    }
}    

//----------------------------------------------------------------------
// Slot: selectIPythonWorkingDir
// Select executable in local host corresponding to IPython binary
//----------------------------------------------------------------------
void ConfigTool::selectIPythonWorkingDir()
{
    QString pathName;

    pathName = QFileDialog::getExistingDirectory(this,
                                                 tr("Select IPython working directory"));
    QFileInfo fs(pathName);
    if (fs.exists()) {
        ui->edIPythonPath->setText(pathName);
    }
}    

//==[ HOST ]===================================================================

//----------------------------------------------------------------------
// Slot: addHost
// Opens a dialog to add a new used defined host
//----------------------------------------------------------------------
void ConfigTool::addHost()
{
    ContHostEdit dlg;
    QString ip;
    bool isMaster;
    int numAg;
    QTableView * vw = ui->tblviewHosts;
    QAbstractItemModel * model = vw->model();
    int row = model->rowCount();
    if (dlg.exec()) {
        // Create new tool and append to list in table
        model->insertRows(row, 1, QModelIndex());
        dlg.getContHost(ip, isMaster, numAg);
        model->setData(model->index(row, 0, QModelIndex()), ip);
        model->setData(model->index(row, 1, QModelIndex()), (isMaster ?
                                                           "Master Host" :
                                                           "Processing Host"));
        model->setData(model->index(row, 2, QModelIndex()), numAg);
    }
}

//----------------------------------------------------------------------
// Slot: removeHost
// Removes the selected row with information of a configured host
//----------------------------------------------------------------------
void ConfigTool::removeHost()
{
    removeFromTable(ui->tblviewHosts, "host");
}

//----------------------------------------------------------------------
// Slot: editHost
// Edits the host in the table with a given model index
//----------------------------------------------------------------------
void ConfigTool::editHost(QModelIndex idx)
{
    ContHostEdit dlg;
    QTableView * vw = ui->tblviewHosts;
    QAbstractItemModel * model = vw->model();
    int row = idx.row();
    QString ip = model->index(row,0).data().toString();
    bool isMaster = model->index(row,1).data().toString() == "Master Host";
    int numAg = model->index(row,2).data().toInt();
    dlg.setContHost(ip, isMaster, numAg);
    if (dlg.exec()) {
        // Create new tool and append to list in table
        dlg.getContHost(ip, isMaster, numAg);
        model->setData(model->index(row, 0, QModelIndex()), ip);
        model->setData(model->index(row, 1, QModelIndex()), (isMaster ?
                                                           "Master Host" :
                                                           "Processing Host"));
        model->setData(model->index(row, 2, QModelIndex()), numAg);
    }
}
                               
//==[ SWARM ]===================================================================

//----------------------------------------------------------------------
// Slot: addSwarm
// Adds a new row to the swarms table to allow the addition of a new swarm
//----------------------------------------------------------------------
void ConfigTool::addSwarm()
{
    SwarmEdit dlg;
    QString id;
    QString name;
    QString ip;
    QStringList wips;
    int scale;
    QString image;
    QString exe;
    QString args;        
    QTableView * vw = ui->tblviewHosts;
    QAbstractItemModel * model = vw->model();
    int row = model->rowCount();
    if (dlg.exec()) {
        // Create new tool and append to list in table
        model->insertRows(row, 1, QModelIndex());
        dlg.getSwarm(id, name, ip, wips, scale, image, exe, args);
        model->setData(model->index(row, 0, QModelIndex()), id);
        model->setData(model->index(row, 1, QModelIndex()), name);
        model->setData(model->index(row, 2, QModelIndex()), ip);
        model->setData(model->index(row, 3, QModelIndex()), wips.join(" "));
        model->setData(model->index(row, 4, QModelIndex()), scale);
        model->setData(model->index(row, 5, QModelIndex()), image);
        model->setData(model->index(row, 6, QModelIndex()), exe);
        model->setData(model->index(row, 7, QModelIndex()), args);
    }
}

//----------------------------------------------------------------------
// Slot: removeSwarm
// Removes the selected row with information of a configured swarm
//----------------------------------------------------------------------
void ConfigTool::removeSwarm()
{
    removeFromTable(ui->tblviewSwarms, "swarm");
}

//----------------------------------------------------------------------
// Slot: editSwarm
// Edits the swarm in the table with a given model index
//----------------------------------------------------------------------
void ConfigTool::editSwarm(QModelIndex idx)
{
    SwarmEdit dlg;
    QTableView * vw = ui->tblviewSwarms;
    QAbstractItemModel * model = vw->model();
    int row = idx.row();
    QString id = model->index(row,0).data().toString();
    QString name = model->index(row,1).data().toString();
    QString ip = model->index(row,2).data().toString();
    QStringList wips = model->index(row,3).data().toString().split(QRegExp("\\W+"), QString::SkipEmptyParts);
    int scale = model->index(row,4).data().toInt();
    QString image = model->index(row,5).data().toString();
    QString exe = model->index(row,6).data().toString();
    QString args = model->index(row,7).data().toString();
    dlg.setSwarm(id, name, ip, wips, scale, image, exe, args);
    if (dlg.exec()) {
        // Replace swarm values
        dlg.getSwarm(id, name, ip, wips, scale, image, exe, args);
        model->setData(model->index(row, 0, QModelIndex()), id);
        model->setData(model->index(row, 1, QModelIndex()), name);
        model->setData(model->index(row, 2, QModelIndex()), ip);
        model->setData(model->index(row, 3, QModelIndex()), wips.join(" "));
        model->setData(model->index(row, 4, QModelIndex()), scale);
        model->setData(model->index(row, 5, QModelIndex()), image);
        model->setData(model->index(row, 6, QModelIndex()), exe);
        model->setData(model->index(row, 7, QModelIndex()), args);
    }
}

//==[ PRODUCTS ]=============================================================

//----------------------------------------------------------------------
// Slot: addProduct
// Adds a new row to the product types list to allow the addition of a new one
//----------------------------------------------------------------------
void ConfigTool::addProduct()
{
    QListView * vw = ui->listProductTypes;
    vw->model()->insertRow(vw->model()->rowCount());
}

//----------------------------------------------------------------------
// Slot: removeProduct
// Removes the selected row with information of a configured product type
//----------------------------------------------------------------------
void ConfigTool::removeProduct()
{
    removeFromTable(ui->listProductTypes, "product type");
}

//==[ PROCESSORS ]=============================================================

//----------------------------------------------------------------------
// Slot: addProcessor
// Adds a new row to the processors list to allow the addition of a new one
//----------------------------------------------------------------------
void ConfigTool::addProcessor()
{
    QListView * vw = ui->listProcs;
    vw->model()->insertRow(vw->model()->rowCount());
}

//----------------------------------------------------------------------
// Slot: removeProcessor
// Removes the selected row with information of a configured processor
//----------------------------------------------------------------------
void ConfigTool::removeProcessor()
{
    removeFromTable(ui->listProcs, "processor");
}

//==[ RULES ]=================================================================

//----------------------------------------------------------------------
// Slot: addRule
// Adds a new row to the rules table to allow the addition of a new rule
//----------------------------------------------------------------------
void ConfigTool::addRule()
{
    RuleEdit dlg;
    QTableView * vw = ui->tblviewRules;
    QAbstractItemModel * model = vw->model();
    int row = model->rowCount();
    QString name;
    QStringList ins;
    QString cond;
    QString proc;
    QStringList outs;

    QStringList products;
    for (auto & s : cfg.products.productTypes()) { products << QS(s); }

    QStringList processors;
    for (auto & kv : cfg.orchestration.processors()) { processors << QS(kv.first); }

    QVector<int> inputs;
    QVector<int> outputs;
    
    dlg.setRule(name, products, inputs, outputs, cond, processors, proc);
    if (dlg.exec()) {
        // Create new tool and append to list in table
        model->insertRows(row, 1, QModelIndex());
        dlg.getRule(name, ins, outs, cond, proc);
        model->setData(model->index(row, 0, QModelIndex()), name);
        model->setData(model->index(row, 1, QModelIndex()), ins.join(","));
        model->setData(model->index(row, 2, QModelIndex()), cond);
        model->setData(model->index(row, 3, QModelIndex()), proc);
        model->setData(model->index(row, 4, QModelIndex()), outs.join(","));
    }
}

//----------------------------------------------------------------------
// Slot: removeRule
// Removes the selected row with information of a configured rule
//----------------------------------------------------------------------
void ConfigTool::removeRule()
{
    removeFromTable(ui->tblviewRules, "rule");
}

//----------------------------------------------------------------------
// Slot: editRule
// Edits the rule in the table with a given model index
//----------------------------------------------------------------------
void ConfigTool::editRule(QModelIndex idx)
{
    RuleEdit dlg;
    QTableView * vw = ui->tblviewRules;
    QAbstractItemModel * model = vw->model();
    int row = idx.row();
    QString name = model->index(row,0).data().toString();
    QStringList ins = model->index(row,1).data().toString().split(",");
    QString cond = model->index(row,2).data().toString();
    QString proc = model->index(row,3).data().toString();
    QStringList outs = model->index(row,4).data().toString().split(",");

    QStringList products;
    for (auto & s : cfg.products.productTypes()) { products << QS(s); }

    QStringList processors;
    for (auto & kv : cfg.orchestration.processors()) { processors << QS(kv.first); }

    QVector<int> inputs;
    QVector<int> outputs;
    for (int i = 0; i < products.size(); ++i) {
        QString prod = products.at(i);
        foreach (const QString & s, ins) { if (s == prod) { inputs << i; } }
        foreach (const QString & s, outs) { if (s == prod) { outputs << i; } }
    }
    
    dlg.setRule(name, products, inputs, outputs, cond, processors, proc);
    if (dlg.exec()) {
        // Create new tool and append to list in table
        dlg.getRule(name, ins, outs, cond, proc);
        model->setData(model->index(row, 0, QModelIndex()), name);
        model->setData(model->index(row, 1, QModelIndex()), ins.join(","));
        model->setData(model->index(row, 2, QModelIndex()), cond);
        model->setData(model->index(row, 3, QModelIndex()), proc);
        model->setData(model->index(row, 4, QModelIndex()), outs.join(","));
    }
}

//----------------------------------------------------------------------
// Slot: removeFromTable
// Removes the selected row from a table/list view, if confirmed by user
//----------------------------------------------------------------------
void ConfigTool::removeFromTable(QAbstractItemView * vw, QString item)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Yoy requested to remove a %1 from the list.").arg(item));
    msgBox.setInformativeText(QString("Do you really want to delete the selected %1? "
                                      " Its information will be lost.").arg(item));
    QPushButton *removeButton = msgBox.addButton(QString("Remove %1").arg(item), QMessageBox::ActionRole);
    QPushButton *discardtButton = msgBox.addButton(QMessageBox::Discard);
    msgBox.setDefaultButton(discardtButton);
    msgBox.exec();
    if (msgBox.clickedButton() == removeButton) {
        vw->model()->removeRow(vw->currentIndex().row());
    }
}

//----------------------------------------------------------------------
// Slot: addNewTool
// Opens a dialog to add a new used defined tool
//----------------------------------------------------------------------
void ConfigTool::addNewTool()
{
    ExtToolEdit dlg;
    dlg.setProdTypes(prodTypes);
    if (dlg.exec()) {
        // Create new tool and append to list in table
        QUserDefTool udt;
        dlg.getToolInfo(udt);
        int row = ui->tblwdgUserDefTools->rowCount();
        ui->tblwdgUserDefTools->insertRow(row);

        ui->tblwdgUserDefTools->setItem(row, 0, new QTableWidgetItem(udt.name));
        ui->tblwdgUserDefTools->setItem(row, 2, new QTableWidgetItem(udt.desc));
        ui->tblwdgUserDefTools->setItem(row, 1, new QTableWidgetItem(udt.exe));
        ui->tblwdgUserDefTools->setItem(row, 3, new QTableWidgetItem(udt.args));
        ui->tblwdgUserDefTools->setItem(row, 4, new QTableWidgetItem(udt.prod_types.join(QString("|"))));

        userDefTools[udt.name] = udt;
    }
}

//----------------------------------------------------------------------
// Slot: editTool
// Edits the tool in the table with a given model index
//----------------------------------------------------------------------
void ConfigTool::editTool(QModelIndex idx)
{
    int row = idx.row();
    editTool(row);
}

//----------------------------------------------------------------------
// Slot: editTool
// Edits the tool selected in the table
//----------------------------------------------------------------------
void ConfigTool::editTool()
{
    QList<QTableWidgetItem*> items = ui->tblwdgUserDefTools->selectedItems();
    int row = items.first()->row();
    editTool(row);
}

//----------------------------------------------------------------------
// Slot: editTool
// Edits the row-th tool in the table
//----------------------------------------------------------------------
void ConfigTool::editTool(int row)
{
    QString name = ui->tblwdgUserDefTools->item(row, 0)->data(0).toString();
    QUserDefTool udt = userDefTools[name];
    ExtToolEdit dlg;
    dlg.setProdTypes(prodTypes);
    dlg.editTool(udt);
    if (dlg.exec()) {
        // Create new tool and append to list in table
        dlg.getToolInfo(udt);

        ui->tblwdgUserDefTools->item(row, 0)->setData(0, udt.name);
        ui->tblwdgUserDefTools->item(row, 1)->setData(0, udt.desc);
        ui->tblwdgUserDefTools->item(row, 2)->setData(0, udt.exe);
        ui->tblwdgUserDefTools->item(row, 3)->setData(0, udt.args);
        ui->tblwdgUserDefTools->item(row, 4)->setData(0, udt.prod_types.join(QString("|")));

        userDefTools[udt.name] = udt;
    }
}

//----------------------------------------------------------------------
// Slot: changeToolWithItem
// Puts tool info into table item
//----------------------------------------------------------------------
void ConfigTool::changeToolWithItem(QTableWidgetItem * item)
{
    QString content = item->data(0).toString();
    QString name = ui->tblwdgUserDefTools->item(item->row(), 0)->data(0).toString();
    QUserDefTool & udt = const_cast<QUserDefTool&>(userDefTools[name]);
    switch (item->column()) {
    case 0: udt.name       = content; break;
    case 1: udt.desc       = content; break;
    case 2: udt.exe        = content; break;
    case 3: udt.args       = content; break;
    case 4: udt.prod_types = content.split("|"); break;
    default: break;
    }
}

//----------------------------------------------------------------------
// Slot: removeTool
// Removes the selected tool from table
//----------------------------------------------------------------------
void ConfigTool::removeTool()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Remove tool from list");
    msgBox.setText("You requested to remove selected user defined tool from the list.");
    msgBox.setInformativeText("Do you really want to remove this tool?");
    QPushButton * removeButton = msgBox.addButton(tr("Remove tool"), QMessageBox::ActionRole);
    QPushButton * cancelButton = msgBox.addButton(QMessageBox::Abort);

    msgBox.exec();
    if (msgBox.clickedButton() == removeButton) {
        QList<QTableWidgetItem*> items = ui->tblwdgUserDefTools->selectedItems();
        int row = items.first()->row();
        QString name = ui->tblwdgUserDefTools->item(row, 0)->data(0).toString();
        ui->tblwdgUserDefTools->removeRow(row);
        userDefTools.remove(name);
    } else if (msgBox.clickedButton() == cancelButton) {
        return;
    }
}

//----------------------------------------------------------------------
// Slot: cancelDlg
// Cancel dialog (all edited info is forgotten)
//----------------------------------------------------------------------
void ConfigTool::cancelDlg()
{
    userDefTools = origDefTools;
}

//----------------------------------------------------------------------
// Method: transferCfgToGUI
// Transfer internal configuration info to GUI widgets
//----------------------------------------------------------------------
void ConfigTool::transferCfgToGUI()
{
    // Generate values for Config. display
    QVector<QStringList> netTable;
    QVector<QStringList> hostsTable;
    QVector<QStringList> swarmsTable;
    QVector<QStringList> rulesTable;
    QStringList hostsList;
    QStringList data;
    QStringList hdr;
    int masterIndex = 0;

    QMap<QString, QString> ipToMachine;
    QMap<QString, int> machineAgents;

    // Create Agents table
    std::string masterHost = cfg.network.masterNode();
    for (auto & kv : cfg.network.processingNodes()) {
        data.clear();
        data << QS(kv.first)
             << tr((kv.first == masterHost) ? "Master Host" : "Processing Host")
             << QString("%1").arg(kv.second);
        hostsTable.append(data);
    }

    for (auto & kv : cfg.network.swarms()) {
        CfgGrpSwarm & swrm = kv.second;
        std::string manager("");
        std::string workers("");
        for (auto & nd : swrm.serviceNodes()) {
            if (manager.empty()) {
                manager = nd;
            } else {
                workers += nd + " ";
            }
        }
        std::string args;
        for (auto & s : swrm.args()) {
            args += s + " ";
        }

        data.clear();
        data << QS(kv.first)
             << QS(swrm.name())
             << QS(manager) << QS(workers)
             << QS(std::to_string(swrm.scale()))
             << QS(swrm.image())
             << QS(swrm.exec())
             << QS(args);
        swarmsTable.append(data);          
    }

    json rules = cfg.orchestration.rules.val();
    for (int i = 0; i < rules.size(); ++i) {
        json & rule = rules[i]; 
        data.clear();
        data << QS(rule["name"].asString())
             << QS(rule["inputs"].asString())
             << QS(rule["condition"].asString())
             << QS(rule["processing"].asString())
             << QS(rule["outputs"].asString());
        rulesTable.append(data);
    }

    // Now, put data in the GUI

    ui->lblConfigName->setText(C("Source: " + cfg.cfgFileName));

    // 1. GENERAL
    // 1.1 General
    ui->edAppName->setText(C(cfg.general.appName()));
    ui->edAppVersion->setText(C(cfg.general.appVersion()));
    ui->edLastAccess->setText(C(cfg.general.lastAccess()));

    ui->edBasePath->setText(C(cfg.general.workArea()));
    ui->nedRunFolder->setText(C(Config::PATHRun));
    ui->nedLocalArchiveFolder->setText(C(cfg.storage.archive));
    ui->nedInbox->setText(C(cfg.storage.inbox));
    ui->nedOutbox->setText(C(cfg.storage.gateway));

    std::string userWAType = cfg.general.userAreaType();
    if (userWAType == UserAreaName[UA_USER]) {
        ui->btngrpUserWA->button(Auto)->setChecked(true);
    } else if (userWAType == UserAreaName[UA_LOCAL]) {
        ui->btngrpUserWA->button(LocalFolder)->setChecked(true);
    } else if (userWAType == UserAreaName[UA_VOSPACE]) {
        ui->btngrpUserWA->button(VOSpaceFolder)->setChecked(true);
    } else {
        // Nothing
    }
    
    // 1.2 Environment
    ui->edUser->setText(qgetenv("USER"));
    ui->edHost->setText(QHostInfo::localHostName());

    ui->edExec->setText(QCoreApplication::applicationFilePath());
    ui->edPATH->setText(qgetenv("PATH"));
    ui->edLDPATH->setText(qgetenv("LD_LIBRARY_PATH"));

    // 2. MACHINES
    hdr.clear();
    hdr << "Host IP" << "Host Type" << "Num.Agents";
    ModelView * mvHosts = createTableModelView(ui->tblviewHosts, hostsTable, hdr);
    (void)(mvHosts);

    hdr.clear();
    hdr << "Swarm Id" << "Srv. name" << "Manager" << "Workers" << "Scale" << "Image" << "Exec" << "Args";
    ModelView * mvSwarms = createTableModelView(ui->tblviewSwarms, swarmsTable, hdr);
    (void)(mvSwarms);

    ui->spboxStartingPort->setValue(cfg.network["startingPort"].asInt());

    // 3. DATABASE
    ui->edDBHost->setText(C(cfg.db.host()));
    ui->edDBPort->setText(C(cfg.db.port()));
    ui->edDBName->setText(C(cfg.db.name()));
    ui->edDBUser->setText(C(cfg.db.user()));
    ui->edDBPwd->setText(C(cfg.db.pwd()));

    // 4. PRODUCTS & PROCESSORS
    
    // 4.1 Products
    data.clear();
    for (auto & s : cfg.products.productTypes()) { data << QS(s); }
    ModelView * mvPT = createListModelView(ui->listProductTypes, data, "Product Type");
    (void)(mvPT);

    // 4.2 Processors
    data.clear();
    for (auto & kv : cfg.orchestration.processors()) { data << QS(kv.first); }
    ModelView * mvProcs = createListModelView(ui->listProcs, data, "Processor Name");
    (void)(mvProcs);
    
    // 5. RULES
    hdr.clear();
    hdr << "Rule name" << "Inputs" << "Condition" << "Processor" << "Outputs";
    ModelView * mvRules = createTableModelView(ui->tblviewRules, rulesTable, hdr);
    (void)(mvRules);

    // 6. USER DEFINED TOOLS
    // Already set

    // 7. CONNECTIVITY

    // VOSpace
    ui->edVOSpaceURL->setText(C(cfg.connectivity.vospace.url()));
    ui->edVOSpaceUser->setText(C(cfg.connectivity.vospace.user()));
    ui->edVOSpacePwd->setText(C(cfg.connectivity.vospace.pwd()));
    ui->edVOSpaceFolder->setText(C(cfg.connectivity.vospace.folder()));

    // Jupyter (Lab)
    ui->edJupyterLabHost->setText(C(cfg.connectivity.jupyter.host()));
    ui->edJupyterLabHostUser->setText(C(cfg.connectivity.jupyter.user()));
    ui->edJupyterLabHostPwd->setText(C(cfg.connectivity.jupyter.pwd()));
    ui->edJupyterLabURL->setText(C(cfg.connectivity.jupyter.url()));

    // IPython
    ui->edIPythonCmd->setText(C(cfg.connectivity.ipython.cmd()));
    ui->edIPythonPath->setText(C(cfg.connectivity.ipython.path()));
    
    // 8. FLAGS
    transferFlagsFromCfgToGUI();

    QStringList levels;
    for (int lvl = (int)(Log::TRACE); lvl <= (int)(Log::FATAL); lvl++) {
        levels << QString::fromStdString(Log::LogLevelName[lvl]);
    }    
    ui->cboxMinLogLevel->addItems(levels);
    ui->cboxMinLogLevel->setCurrentIndex((int)(Log::getMinLogLevel()));
}

//----------------------------------------------------------------------
// Method: transferGUIToCfg
// Transfer config settings in GUI to internal configuration info
//----------------------------------------------------------------------
bool ConfigTool::transferGUIToCfg()
{
    // 1. GENERAL
    // 1.1 General
    cfg.general["workArea"] = ui->edBasePath->text().toStdString();
    switch (ui->btngrpUserWA->checkedId()) {
    case Auto:
        cfg.general["userAreaType"] = UserAreaName[UA_USER];
        cfg.general["userArea"]     = ui->edBasePath->text().toStdString() + "/data/user";
        break;
    case LocalFolder:
        cfg.general["userAreaType"] = UserAreaName[UA_LOCAL];
        cfg.general["userArea"]     = ui->nedUserReprocArea->text().toStdString();
        break;
    case VOSpaceFolder:
        cfg.general["userAreaType"] = UserAreaName[UA_VOSPACE];
        cfg.general["userArea"]     = ui->edVOSpaceFolder->text().toStdString();
        break;
    default:
        break;
    }
            
    // 1.2 Environment

    // 2. MACHINES

    cfg.network["processingNodes"].clear();
    cfg.network["swarms"].clear();

    int numOfMasterNodes = 0;
    QAbstractItemModel * model = ui->tblviewHosts->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        std::string ip = model->index(i,0).data().toString().toStdString();
        bool isMaster = model->index(i,1).data().toString() == "Master Host";
        int numAg = model->index(i,2).data().toInt();
        if (isMaster) {
            cfg.network["masterNode"] = ip;
            numOfMasterNodes++;
        }
        cfg.network["processingNodes"][ip] = numAg;
    }
    cfg.network["startingPort"] = ui->spboxStartingPort->value();

    if (numOfMasterNodes != 1) {
        int ret = QMessageBox::question(this, tr("Error in processing nodes"),
                                        tr("There is an error in the definition of the processing nodes.\n\n"
                                           "There must be exactly 1 processing node declared as MASTER, \n"
                                           "but in the current configuration there are %1 declared.\n\n"
                                           "The configuration will not be saved.").arg(numOfMasterNodes),
                                        QMessageBox::Ok);
        return false;
    }
        
    model = ui->tblviewSwarms->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        std::string id = model->index(i,0).data().toString().toStdString();
        std::string name = model->index(i,1).data().toString().toStdString();
        QStringList nodes;
        nodes << model->index(i,2).data().toString();
        nodes << model->index(i,3).data().toString()
            .split(QRegExp("\\W+"), QString::SkipEmptyParts);
        int scale = model->index(i,4).data().toInt();
        std::string image = model->index(i,5).data().toString().toStdString();;
        std::string exe  = model->index(i,6).data().toString().toStdString();;
        std::string args  = model->index(i,7).data().toString().toStdString();;
        
        CfgGrpSwarm swrm;
        swrm["name"] = name;
        swrm["scale"] = scale;
        swrm["image"] = image;
        swrm["exec"] = exe;
        swrm["args"] = args;
        foreach (QString s, nodes) { swrm["serviceNodes"].append(s.toStdString()); }
        
        cfg.network["swarms"][id] = swrm.val();
    }
    cfg.network["startingPort"] = ui->spboxStartingPort->value();

    // 3. DATABASE
    cfg.db["host"] = ui->edDBHost->text().toStdString();
    cfg.db["port"] = ui->edDBPort->text().toStdString();
    cfg.db["name"] = ui->edDBName->text().toStdString();
    cfg.db["user"] = ui->edDBUser->text().toStdString();
    cfg.db["pwd"]  = ui->edDBPwd->text().toStdString();

    // 4. PRODUCTS & PROCESSORS
    // 4.1 Products
    model = ui->listProductTypes->model();
    json & prds = cfg.products["productTypes"];
    prds.clear();
    for (int i = 0; i < model->rowCount(); ++i) {
        prds.append(model->index(i,0).data().toString().toStdString());
    }
    
    // 4.2 Processors
    model = ui->listProcs->model();
    json & procs = cfg.orchestration["processors"];
    procs.clear();
    for (int i = 0; i < model->rowCount(); ++i) {
        std::string name = model->index(i,0).data().toString().toStdString();
        procs[name] = name;
    }
        
    // 5. RULES
    model = ui->tblviewRules->model();
    json & rules = cfg.orchestration.rules.val();
    rules = nullJson;
    for (int i = 0; i < model->rowCount(); ++i) {
        std::string tag = model->index(i,0).data().toString().toStdString();
        std::string inputs = model->index(i,1).data().toString().toStdString();
        std::string outputs = model->index(i,4).data().toString().toStdString();;
        std::string condition  = model->index(i,2).data().toString().toStdString();;
        std::string processing  = model->index(i,3).data().toString().toStdString();;
        rules["tag"].append(tag);
        rules["inputs"].append(inputs);
        rules["outputs"].append(outputs);
        rules["processing"].append(processing);
        rules["condition"].append(condition);
    }
    
    // 6. USER DEFINED TOOLS

    json uts;

    QMap<QString, QUserDefTool>::const_iterator it  = userDefTools.constBegin();
    auto end = userDefTools.constEnd();
    int i = 0;
    while (it != end) {
        const QUserDefTool & t = it.value();

        uts[i]["name"]         = t.name.toStdString();
        uts[i]["description"]  = t.desc.toStdString();
        uts[i]["executable"]   = t.exe.toStdString();
        uts[i]["arguments"]    = t.args.toStdString();
        uts[i]["productTypes"] = t.prod_types.join(",").toStdString();

        ++it;
        ++i;
    }

    cfg.userDefTools.fromStr(JValue(uts).str());

    // 7. CONNECTIVITY

    // VOSpace
    cfg.connectivity.vospace["url"]    = ui->edVOSpaceURL->text().toStdString();
    cfg.connectivity.vospace["user"]   = ui->edVOSpaceUser->text().toStdString();
    cfg.connectivity.vospace["pwd"]    = ui->edVOSpacePwd->text().toStdString();
    cfg.connectivity.vospace["folder"] = ui->edVOSpaceFolder->text().toStdString();

    // Jupyter (Lab)
    cfg.connectivity.jupyter["host"]   = ui->edJupyterLabHost->text().toStdString();
    cfg.connectivity.jupyter["user"]   = ui->edJupyterLabHostUser->text().toStdString();
    cfg.connectivity.jupyter["pwd"]    = ui->edJupyterLabHostPwd->text().toStdString();
    cfg.connectivity.jupyter["url"]    = ui->edJupyterLabURL->text().toStdString();

    // IPython
    cfg.connectivity.ipython["cmd"] = ui->edIPythonCmd->text().toStdString();
    cfg.connectivity.ipython["path"] = ui->edIPythonPath->text().toStdString();

    // 8. FLAGS
    transferFlagsFromGUIToCfg();
    cfg.general["logLevel"] = Log::LogLevelName[ui->cboxMinLogLevel->currentIndex()];


    //=== CONSOLIDATE ===
    cfg.consolidate();
    cfg.processConfig();

    return true;
}

//----------------------------------------------------------------------
// Method: createListModelView
// Uses ModelView class to create a model for a list view
//----------------------------------------------------------------------
ModelView * ConfigTool::createListModelView(QAbstractItemView * v,
                                            QStringList & dlist,
                                            QString hdr)
{
    ModelView * mv = new ModelView(dlist, v);
    mv->setHeader(hdr);

    return mv;
}


//----------------------------------------------------------------------
// Method: createListModelView
// Uses ModelView class to create a model for a table view
//----------------------------------------------------------------------
ModelView * ConfigTool::createTableModelView(QAbstractItemView * v,
                                             QVector<QStringList> & vdlist,
                                             QStringList & hdr)
{
    ModelView * mv = new ModelView(vdlist, v);
    mv->setHeaders(hdr);

    return mv;
}

//----------------------------------------------------------------------
// Method: transferFlagsFromCfgToGUI
// Transfer flags settings from internal cfg structure to dialog
//----------------------------------------------------------------------
void ConfigTool::transferFlagsFromCfgToGUI()
{
    json & msgsToDisk = cfg.flags["msgsToDisk"];

    for (auto & v : monitMsgFlags) {
        std::string & msgName = v.msgName;
        v.chkDisk->setChecked(false);
        for (const json & msg : msgsToDisk) {
            if (msg.asString() == msgName) {
                v.chkDisk->setChecked(true);
                break;
            }
        }
    }

    ui->grpboxWriteMsgsDisk->setChecked(cfg.flags.writeMsgsToDisk());
    
    ui->chkMsgsIncommingInLog->setChecked(cfg.flags.notifyMsgArrival());
    ui->chkGroupTskAgentLogs->setChecked(cfg.flags.groupTaskAgentLogs());

    ui->chkAllowReproc->setChecked(cfg.flags.allowReprocessing());
    ui->chkGenerateIntermedProd->setChecked(cfg.flags.intermediateProducts());

    ui->chkSendOutputsToArchive->setChecked(cfg.flags.sendOutputsToMainArchive());

    ui->edProgressString->setText(QS(cfg.flags.progressString()));
}

//----------------------------------------------------------------------
// Method: transferFlagsFromGUIToCfg
// Transfer flags settings from dialog to internal cfg structure
//----------------------------------------------------------------------
void ConfigTool::transferFlagsFromGUIToCfg()
{
    cfg.flags["msgsToDisk"].clear();
    for (auto & v : monitMsgFlags) {
        if (v.chkDisk->isChecked()) { cfg.flags["msgsToDisk"].append(v.msgName); }
    }

    cfg.flags["writeMsgsToDisk"] = ui->grpboxWriteMsgsDisk->isChecked();
    
    cfg.flags["notifyMsgArrival"]   = ui->chkMsgsIncommingInLog->isChecked();
    cfg.flags["groupTaskAgentLogs"] = ui->chkGroupTskAgentLogs->isChecked();

    cfg.flags["allowReprocessing"] = ui->chkAllowReproc->isChecked();
    cfg.flags["intermediateProducts"]  = ui->chkGenerateIntermedProd->isChecked();

    cfg.flags["sendOutputsToMainArchive"] = ui->chkSendOutputsToArchive->isChecked();

    cfg.flags["progressString"] = ui->edProgressString->text().toStdString();
}

// Auxiliary vector with information about config. flags
QVector<ConfigTool::FlagSt> ConfigTool::monitMsgFlags;

}
