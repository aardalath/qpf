/******************************************************************************
 * File:    configtool.cpp
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

#include "cfginfo.h"

#include "tools.h"
using LibComm::join;

#include "exttooledit.h"

#include <QHostInfo>
#include <QFileInfo>
#include <QCheckBox>

#define C(x) (x).c_str()
#define QS(x) QString::fromStdString(x)

ModelView::ModelView(QVector<QStringList> & vdlist,
                     QAbstractItemView  * v,
                     QAbstractItemModel * m)
    : view(v), model(m), type(Table)
{
    if (view  == 0) { view = new QTableView(0); }
    if (model == 0) { model = new QStandardItemModel(vdlist.count(),
                                                     vdlist.at(0).count()); }
    setData(vdlist);
    view->setModel(model);
}

ModelView::ModelView(QStringList & dlist,
                     QAbstractItemView  * v,
                     QAbstractItemModel * m)
    : view(v), model(m), type(List)
{
    if (view  == 0) { view  = new QListView(0); }
    if (model == 0) { model = new QStringListModel(); }
    dynamic_cast<QStringListModel*>(model)->setStringList(dlist);
    view->setModel(model);
}

void ModelView::setHeader(QString h)
{
    model->setHeaderData(0, Qt::Horizontal, h);
}

void ModelView::setHeaders(QStringList & hlist)
{
    for (int i = 0; i < hlist.count(); ++i) {
        model->setHeaderData(i, Qt::Horizontal, hlist.at(i));
    }
}

void ModelView::setData(QStringList & dlist)
{
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    for (int row = 0; row < dlist.count(); ++row) {
        model->insertRows(row, 1, QModelIndex());
        model->setData(model->index(row, 0, QModelIndex()), dlist.at(row));
    }
}
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

namespace QPF {

ConfigTool::ConfigTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigTool)
{
    ui->setupUi(this);

    monitMsgFlags.append(FlagSt({ "START"      , ui->chkMsgsSTARTToDisk     , ui->chkMsgsSTARTToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "INDATA"     , ui->chkMsgsINDATAToDisk    , ui->chkMsgsINDATAToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "DATA_RQST"  , ui->chkMsgsDATARQSTToDisk  , ui->chkMsgsDATARQSTToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "DATA_INFO"  , ui->chkMsgsDATAINFOToDisk  , ui->chkMsgsDATAINFOToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "MONIT_RQST" , ui->chkMsgsMONITRQSTToDisk , ui->chkMsgsMONITRQSTToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "MONIT_INFO" , ui->chkMsgsMONITINFOToDisk , ui->chkMsgsMONITINFOToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "TASK_PROC"  , ui->chkMsgsTASKPROCToDisk  , ui->chkMsgsTASKPROCToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "TASK_RES"   , ui->chkMsgsTASKRESToDisk   , ui->chkMsgsTASKRESToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "CMD"        , ui->chkMsgsCMDToDisk       , ui->chkMsgsCMDToDisk_2 }));
    monitMsgFlags.append(FlagSt({ "STOP"       , ui->chkMsgsSTOPToDisk      , ui->chkMsgsSTOPToDisk_2 }));


    // Hide non-yet-implemented widgets
    ui->gpboxInput->hide();
    ui->gpboxOutput->hide();

    ui->btngrpSection->setId(ui->tbtnGeneral       , PageGeneral);
    ui->btngrpSection->setId(ui->tbtnMachines      , PageMachines);
    ui->btngrpSection->setId(ui->tbtnProdProc      , PageProdProc);
    ui->btngrpSection->setId(ui->tbtnNetwork       , PageNetwork);
    ui->btngrpSection->setId(ui->tbtnOrchestration , PageOrchestration);
    ui->btngrpSection->setId(ui->tbtnExtTools      , PageExtTools);
    ui->btngrpSection->setId(ui->tbtnStorage       , PageStorage);
    ui->btngrpSection->setId(ui->tbtnFlags         , PageFlags);

    connect(ui->edBasePath, SIGNAL(textChanged(QString)), this, SLOT(setWorkingPaths(QString)));
}

ConfigTool::~ConfigTool()
{
    delete ui;
}

void ConfigTool::readConfig()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // Generate values for Config. display
    QVector<QStringList> netTable;
    QVector<QStringList> hostsTable;
    QVector<QStringList> rulesTable;
    QStringList hostsList;
    QStringList data;
    QStringList hdr;
    int masterIndex = 0;

    QMap<QString, QString> ipToMachine;
    QMap<QString, int> machineAgents;

    int i = 0;
    for (std::string & m : cfgInfo.machines) {
        std::string ip("0.0.0.0");
        if (cfgInfo.machineNodes.find(m) != cfgInfo.machineNodes.end()) {
            std::vector<std::string> & mnodes = cfgInfo.machineNodes[m];
            std::string pname = mnodes.at(0);
            Peer * peer = cfgInfo.peersCfgByName[pname];
            ip = peer->clientAddr.substr(6);
            ip = ip.substr(0, ip.find(':'));
        }
        if (m == cfgInfo.masterMachine) { masterIndex = i; }
        QString machine = QS(m);
        QString iP = QS(ip);
        QStringList column;
        column << machine << iP
               << ((masterIndex == i) ? tr("Master Host") : tr("Processing Host"));
        hostsTable.append(column);
        hostsList << machine;
        ipToMachine[iP] = machine;
        ++i;
    }

    int numNodes = cfgInfo.peersCfg.size();
    for (i = 0; i < numNodes; ++i) {
        Peer & peer = cfgInfo.peersCfg.at(i);
        QString s = QS(peer.clientAddr);
        s = s.mid(s.lastIndexOf("/") + 1);
        s = s.left(s.indexOf(":"));
        if (peer.type == "taskagent") {
            QString m = ipToMachine[s];
            if (machineAgents.find(m) == machineAgents.end()) {
                machineAgents[m] = 1;
            } else {
                machineAgents[m] = machineAgents[m] + 1;
            }
        }

        data.clear();
        data << ipToMachine[s] + " (" + s + ")"
             << QS(peer.name) << QS(peer.type)
             << QS(peer.clientAddr) << QS(peer.serverAddr);

        netTable.append(data);
    }

    for (i = 0; i < hostsTable.count(); ++i) {
        QStringList & column = hostsTable[i];
        column << QString("%1").arg(machineAgents[column.at(0)]);
    }

    Peer * peer = cfgInfo.peersCfgByName[std::string("QPFHMI")];
    QString qpfhmiServerAddr = QS(peer->serverAddr);
    int basePort = qpfhmiServerAddr.mid(qpfhmiServerAddr.lastIndexOf(":") + 1).toInt();

    int numRules = cfgInfo.orcParams.rules.size();
    for (i = 0; i < numRules; ++i) {
        Rule * const rule = cfgInfo.orcParams.rules.at(i);
        data.clear();
        data << QS(rule->name)
             << QS(join(rule->inputs, ", "))
             << QS(rule->condition)
             << QS(rule->processingElement)
             << QS(join(rule->outputs, ", "));
        rulesTable.append(data);
    }

    // Now, put data in the GUI

    ui->lblConfigName->setText(C("Source: " + cfgInfo.cfgFileName));

    // 1. GENERAL
    // 1.1 General
    ui->edAppName->setText(C(cfgInfo.appName));
    ui->edAppVersion->setText(C(cfgInfo.appVersion));
    ui->edLastAccess->setText(C(cfgInfo.lastAccess));
    ui->edWorkArea->setText(C(cfgInfo.storage.base));

    // 1.2 Environment
    ui->edUser->setText(qgetenv("USER"));
    ui->edHost->setText(QHostInfo::localHostName());

    ui->edExec->setText(QCoreApplication::applicationFilePath());
    ui->edPATH->setText(qgetenv("PATH"));
    ui->edLDPATH->setText(qgetenv("LD_LIBRARY_PATH"));

    // 2. MACHINES
    hdr.clear();
    hdr << "Host name" << "IP" << "Host Type" << "Num.Agents";
    ModelView * mvHosts = createTableModelView(ui->tblviewHosts, hostsTable, hdr);
    (void)(mvHosts);

    ui->cboxMasterHost->addItems(hostsList);
    ui->cboxMasterHost->setCurrentIndex(masterIndex);

    ui->spboxBasePort->setValue(basePort);

    // 3. PRODUCTS & PROCESSORS
    // 3.1 Products
    data.clear();
    for (auto & s : cfgInfo.orcParams.productTypes) data << QS(s);
    ModelView * mvPT = createListModelView(ui->listProductTypes, data, "Product Type");
    (void)(mvPT);

    // 3.2 Processors
    data.clear();
    for (auto & kv : cfgInfo.orcParams.processors) data << QS(kv.first);
    ModelView * mvProcs = createListModelView(ui->listProcs, data, "Processors");
    (void)(mvProcs);

    // 4. NETWORK
    hdr.clear();
    hdr << "Host"
        << "Node name" << "Node Type"
        << "Client Addr" << "Server Addr";
    ModelView * mvNet = createTableModelView(ui->tblviewNet, netTable, hdr);
    (void)(mvNet);

    // 4. RULES
    hdr.clear();
    hdr << "Rule name" << "Inputs" << "Condition" << "Processor" << "Outputs";
    ModelView * mvRules = createTableModelView(ui->tblviewRules, rulesTable, hdr);
    (void)(mvRules);

    // 5. USER DEFINED TOOLS

    // Already set

    // 6. STORAGE
    ui->edBasePath->setText(C(cfgInfo.storage.base));
    ui->nedLocalArchiveFolder->setText(C(cfgInfo.storage.local_archive.path));
    ui->nedInbox->setText(C(cfgInfo.storage.inbox.path));
    ui->nedOutbox->setText(C(cfgInfo.storage.outbox.path));

    // 7. FLAGS
    transferFlagsFromCfgToGUI();
}

ModelView * ConfigTool::createListModelView(QAbstractItemView * v,
                                            QStringList & dlist,
                                            QString hdr)
{
    ModelView * mv = new ModelView(dlist, v);
    mv->setHeader(hdr);

    return mv;
}


ModelView * ConfigTool::createTableModelView(QAbstractItemView * v,
                                             QVector<QStringList> & vdlist,
                                             QStringList & hdr)
{
    ModelView * mv = new ModelView(vdlist, v);
    mv->setHeaders(hdr);

    return mv;
}

void ConfigTool::save()
{
    (void)QMessageBox::warning(this, tr("Save"),
                               tr("Sorry, but this functionality is not yet implemented."),
                               QMessageBox::Ok);
}

void ConfigTool::saveAs()
{
    (void)QMessageBox::warning(this, tr("Save as..."),
                               tr("Sorry, but this functionality is not yet implemented."),
                               QMessageBox::Ok);
}

void ConfigTool::apply()
{
    transferFlagsFromGUIToCfg();
    accept();
}

void ConfigTool::setWorkingPaths(QString newPath)
{
    QFileInfo fs(newPath);

    QString base = fs.path();
    ui->nedLocalArchiveFolder->setText(base + "/data/archive");
    ui->nedInbox->setText(base + "/data/inbox");
    ui->nedOutbox->setText(base + "/data/outbox");
}

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
    }
    userDefTools = userTools;
    origDefTools = userTools;
    prodTypes = pts;
    connect(ui->tblwdgUserDefTools, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(changeToolWithItem(QTableWidgetItem*)));
}

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

void ConfigTool::editTool(QModelIndex idx)
{
    int row = idx.row();
    editTool(row);
}

void ConfigTool::editTool()
{
    QList<QTableWidgetItem*> items = ui->tblwdgUserDefTools->selectedItems();
    int row = items.first()->row();
    editTool(row);
}

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

void ConfigTool::removeTool()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Remove tool from list");
    msgBox.setText("You requested to remove selected user defined tool from the list.");
    msgBox.setInformativeText("Do you really want to remove this tool?");
    QPushButton *removeButton = msgBox.addButton(tr("Remove tool"), QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Abort);

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

void ConfigTool::cancelDlg()
{
    userDefTools = origDefTools;
}

void ConfigTool::getExtTools(MapOfUserDefTools & userTools)
{
    userTools = userDefTools;
}

void ConfigTool::transferFlagsFromCfgToGUI()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    std::string msgName;

    std::map<std::string, bool> & fmapDsk = cfgInfo.flags.monit.msgsToDisk;
    std::map<std::string, bool> & fmapDB  = cfgInfo.flags.monit.msgsToDB;

    for (int i = (int)(MSG_START_IDX); i < (int)(MSG_UNKNOWN_IDX); ++i) {
        msgName = monitMsgFlags.at(i).msgName;
        monitMsgFlags.at(i).chkDisk->setChecked(!(fmapDsk.find(msgName) == fmapDsk.end()));
        monitMsgFlags.at(i).chkDB->setChecked(!(fmapDB.find(msgName) == fmapDB.end()));
    }

    ui->chkMsgsIncommingInLog->setChecked(cfgInfo.flags.monit.notifyMsgArrival);
    ui->chkGroupTskAgentLogs->setChecked(cfgInfo.flags.monit.groupTaskAgentLogs);

    ui->chkAllowReproc->setChecked(cfgInfo.flags.proc.allowReprocessing);
    ui->chkGenerateIntermedProd->setChecked(cfgInfo.flags.proc.intermedProducts);

    ui->chkSendOutputsToArchive->setChecked(cfgInfo.flags.arch.sendOutputsToMainArchive);
}

void ConfigTool::transferFlagsFromGUIToCfg()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    std::string msgName;

    std::map<std::string, bool> & fmapDsk = cfgInfo.flags.monit.msgsToDisk;
    std::map<std::string, bool> & fmapDB = cfgInfo.flags.monit.msgsToDB;
    fmapDsk.clear();
    fmapDB.clear();

    for (int i = (int)(MSG_START_IDX); i < (int)(MSG_UNKNOWN_IDX); ++i) {
        msgName = monitMsgFlags.at(i).msgName;
        if (monitMsgFlags.at(i).chkDisk->isChecked()) { fmapDsk[msgName] = true; }
        if (monitMsgFlags.at(i).chkDB->isChecked()) { fmapDB[msgName] = true; }
    }

    cfgInfo.flags.monit.notifyMsgArrival   = ui->chkMsgsIncommingInLog->isChecked();
    cfgInfo.flags.monit.groupTaskAgentLogs = ui->chkGroupTskAgentLogs->isChecked();

    cfgInfo.flags.proc.allowReprocessing = ui->chkAllowReproc->isChecked();
    cfgInfo.flags.proc.intermedProducts  = ui->chkGenerateIntermedProd->isChecked();

    cfgInfo.flags.arch.sendOutputsToMainArchive = ui->chkSendOutputsToArchive->isChecked();
}

QVector<ConfigTool::FlagSt> ConfigTool::monitMsgFlags;

}
