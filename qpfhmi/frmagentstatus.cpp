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

    int percRunning  = 0;
    int percWaiting  = 0;
    int percPaused   = 0;
    int percStopped  = 0;
    int percFailed   = 0;
    int percFinished = 0;

    if (d.total > 0) {
        int w = ui->frmStatusBar->width();
        percWaiting  = d.waiting * w / d.total;
        percPaused   = d.paused * w / d.total;
        percStopped  = d.stopped * w / d.total;
        percFailed   = d.failed * w / d.total;
        percFinished = d.finished * w / d.total;
        percRunning  = w - (percWaiting + percPaused + percStopped +
                            percFailed + percFinished);
    }
    ui->frmRunning->setGeometry(ui->frmRunning->x(), ui->frmRunning->y(),
                                percRunning, ui->frmRunning->height());
    ui->frmWaiting->setGeometry(ui->frmWaiting->x(), ui->frmWaiting->y(),
                                percWaiting, ui->frmWaiting->height());
    ui->frmPaused->setGeometry(ui->frmPaused->x(), ui->frmPaused->y(),
                               percPaused, ui->frmPaused->height());
    ui->frmStopped->setGeometry(ui->frmStopped->x(), ui->frmStopped->y(),
                                percStopped, ui->frmStopped->height());
    ui->frmFailed->setGeometry(ui->frmFailed->x(), ui->frmFailed->y(),
                               percFailed, ui->frmFailed->height());
    ui->frmFinished->setGeometry(ui->frmFinished->x(), ui->frmFinished->y(),
                                 percFinished, ui->frmFinished->height());

        /*
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
*/
}

}
