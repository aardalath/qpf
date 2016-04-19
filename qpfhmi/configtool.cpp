#include "configtool.h"
#include "ui_configtool.h"

#include "cfginfo.h"

#include "tools.h"
using LibComm::join;

#include <QHostInfo>
#include <QFileInfo>

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

    // Hide non-yet-implemented widgets
    ui->gpboxInput->hide();
    ui->gpboxOutput->hide();

    ui->btngrpSection->setId(ui->tbtnGeneral       , PageGeneral);
    ui->btngrpSection->setId(ui->tbtnMachines      , PageMachines);
    ui->btngrpSection->setId(ui->tbtnProdProc      , PageProdProc);
    ui->btngrpSection->setId(ui->tbtnNetwork       , PageNetwork);
    ui->btngrpSection->setId(ui->tbtnOrchestration , PageOrchestration);
    ui->btngrpSection->setId(ui->tbtnStorage       , PageStorage);

    connect(ui->edBasePath, SIGNAL(textChanged(QString)), this, SLOT(setWorkingPaths(QString)));
}

ConfigTool::~ConfigTool()
{
    delete ui;
}

void ConfigTool::readConfig()
{
    ConfigurationInfo cfgInfo = ConfigurationInfo::data();

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

    // 5. STORAGE
    ui->edBasePath->setText(C(cfgInfo.storage.base));
    ui->nedLocalArchiveFolder->setText(C(cfgInfo.storage.local_archive.path));
    ui->nedInbox->setText(C(cfgInfo.storage.inbox.path));
    ui->nedOutbox->setText(C(cfgInfo.storage.outbox.path));
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

void ConfigTool::setWorkingPaths(QString newPath)
{
    QFileInfo fs(newPath);

    QString base = fs.path();
    ui->nedLocalArchiveFolder->setText(base + "/data/archive");
    ui->nedInbox->setText(base + "/data/inbox");
    ui->nedOutbox->setText(base + "/data/outbox");
}


}
