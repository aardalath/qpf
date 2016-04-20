#include "dlgalert.h"
#include "ui_dlgalert.h"

DlgAlert::DlgAlert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAlert)
{
    ui->setupUi(this);
}

DlgAlert::~DlgAlert()
{
    delete ui;
}

void DlgAlert::setAlert(Alert & alert)
{
    ui->edTimestamp->setText(alert.timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    ui->edID->setText(alert.id);
    ui->edSeverity->setText(alert.severity);
    ui->edComponent->setText(alert.component);
    ui->textedDescription->setPlainText(alert.description);
}
