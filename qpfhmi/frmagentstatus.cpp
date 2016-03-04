#include "frmagentstatus.h"
#include "ui_frmagentstatus.h"

namespace QPF {

FrmAgentStatus::FrmAgentStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrmAgentStatus)
{
    ui->setupUi(this);
}

FrmAgentStatus::~FrmAgentStatus()
{
    delete ui;
}

void FrmAgentStatus::updateInfo(TaskAgentInfo & d)
{
    ui->lblTaskAgentName->setText(QString::fromStdString(d.name));
    ui->lblTotalTasks->setText(QString("%1").arg(d.total));
    ui->lblRuningTasks->setText(QString("%1").arg(d.running));
    ui->lblWaitingTasks->setText(QString("%1").arg(d.waiting));
    ui->lblFailedTasks->setText(QString("%1").arg(d.failed));
    ui->lblFinishedTasks->setText(QString("%1").arg(d.finished));
    ui->lblMaxTasks->setText(QString("%1").arg(d.maxnum));
    ui->lblClient->setText(QString::fromStdString(d.client));
    ui->lblServer->setText(QString::fromStdString(d.server));

    ui->lblLoadAvg1->setText(QString("%1").arg(((double)(d.load1min)) * 0.01, 0, 'f', 2));
    ui->lblLoadAvg2->setText(QString("%1").arg(((double)(d.load5min)) * 0.01, 0, 'f', 2));
    ui->lblLoadAvg3->setText(QString("%1").arg(((double)(d.load15min)) * 0.01, 0, 'f', 2));

    ui->lblUptime->setText(QString("%1").arg(d.uptimesecs));

    int percRunning  = 0;
    int percWaiting  = 0;
    int percFailed   = 0;
    int percFinished = 0;

    if (d.total > 0) {
        percRunning  = d.running * 100 / d.total;
        percWaiting  = d.waiting * 100 / d.total;
        percFailed   = d.failed * 100 / d.total;
        percFinished = d.finished * 100 / d.total;
    }

    ui->pbPercRunning->setValue(percRunning);
    ui->pbPercWaiting->setValue(percWaiting);
    ui->pbPercFailed->setValue(percFailed);
    ui->pbPercFinished->setValue(percFinished);

}

}
