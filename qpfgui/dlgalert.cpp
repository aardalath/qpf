#include "dlgalert.h"
#include "ui_dlgalert.h"

namespace QPF {

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
    ui->edTimestamp->setText(QString::fromStdString(alert.timeStampString()));
    ui->edID->setText(QString::fromStdString(Alert::TypeName[alert.getType()]));
    ui->edSeverity->setText(QString::fromStdString(Alert::SeverityName[alert.getSeverity()]));
    ui->edComponent->setText(QString::fromStdString(alert.getOrigin()));
    ui->textedDescription->setPlainText(QString::fromStdString(alert.what()));
}

}
