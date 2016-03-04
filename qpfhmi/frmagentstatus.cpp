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
    ui->lblRuningTasks->setText(QString("%1/%2/%3/%4/%5/%6")
                                .arg(d.running).arg(d.waiting)
                                .arg(d.paused).arg(d.stopped)
                                .arg(d.failed).arg(d.finished));
    ui->lblMaxTasks->setText(QString("%1").arg(d.maxnum));
    ui->lblClient->setText(QString::fromStdString(d.client));
    ui->lblServer->setText(QString::fromStdString(d.server));

    ui->lblLoadAvg1->setText(QString("%1").arg(((double)(d.load1min)) * 0.01, 0, 'f', 2));
    ui->lblLoadAvg2->setText(QString("%1").arg(((double)(d.load5min)) * 0.01, 0, 'f', 2));
    ui->lblLoadAvg3->setText(QString("%1").arg(((double)(d.load15min)) * 0.01, 0, 'f', 2));

    ui->lblUptime->setText(QString("%1").arg(d.uptimesecs));

    if (d.total > 0) {
        QHBoxLayout * hb = qobject_cast<QHBoxLayout*>(ui->frmStatusBar->layout());
        hb->setStretch(0, d.running * 1000);
        hb->setStretch(1, d.waiting * 1000);
        hb->setStretch(2, d.paused * 1000);
        hb->setStretch(3, d.stopped * 1000);
        hb->setStretch(4, d.failed * 1000);
        hb->setStretch(5, d.finished * 1000);
        ui->frmStatusBar->setVisible(true);
    } else {
        ui->frmStatusBar->setVisible(false);
    }
}

}
