#include "configtool.h"
#include "ui_configtool.h"

#include "cfginfo.h"

namespace QPF {

ConfigTool::ConfigTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigTool)
{
    ui->setupUi(this);

    ui->btngrpSection->setId(ui->tbtnGeneral,0);
    ui->btngrpSection->setId(ui->tbtnMachines,1);
    ui->btngrpSection->setId(ui->tbtnNetwork,2);
    ui->btngrpSection->setId(ui->tbtnProducts,3);
    ui->btngrpSection->setId(ui->tbtnProcessing,4);
    ui->btngrpSection->setId(ui->tbtnOrchestration,5);
    ui->btngrpSection->setId(ui->tbtnStorage,6);
}

ConfigTool::~ConfigTool()
{
    delete ui;
}

void ConfigTool::readConfig()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // General
    ui->applicationNameLineEdit->setText(cfgInfo.appName.c_str());
    ui->applicationVersionLineEdit->setText(cfgInfo.appVersion.c_str());
    ui->lastAccessLineEdit->setText(cfgInfo.lastAccess.c_str());
    ui->runAreaPathLineEdit->setText("/var/run/qpf");

    // Hosts
    ui->twHosts->clear();
    for (std::string & m : cfgInfo.machines) {
        std::string ip("0.0.0.0");
        if (cfgInfo.machineNodes.find(m) != cfgInfo.machineNodes.end()) {
            std::vector<std::string> & mnodes = cfgInfo.machineNodes[m];
            std::string pname = mnodes.at(0);
            Peer * peer = cfgInfo.peersCfgByName[pname];
            ip = peer->clientAddr.substr(6);
            ip = ip.substr(0, ip.find(':'));
        }
        QTreeWidgetItem * tw = new QTreeWidgetItem;
        tw->setData(0, Qt::DisplayRole, QString::fromStdString(m));
        tw->setData(1, Qt::DisplayRole, QString::fromStdString(ip));
        ui->twHosts->addTopLevelItem(tw);
    }

    // Products
    ui->listProductTypes->clear();
    for (std::string & s : cfgInfo.orcParams.productTypes) {
        ui->listProductTypes->addItem(QString::fromStdString(s));
    }

    // Processors
    ui->listProcs->clear();
    for (auto & pr : cfgInfo.orcParams.processors) {
        Processor * pe = pr.second;
        ui->listProcs->addItem(QString::fromStdString(pe->name));
    }
}



}
