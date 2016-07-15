#include "logframe.h"
#include "ui_logframe.h"

#include "datatypes.h"

#include <QTimer>

LogFrame::LogFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LogFrame)
{
    ui->setupUi(this);
}

LogFrame::~LogFrame()
{
    delete ui;
}

void LogFrame::setMsg(QString msg)
{
    ui->lblMsg->setText(msg);
    QTimer::singleShot(100, this, SLOT(clearMsg()));
}

void LogFrame::clearMsg()
{
    ui->lblMsg->clear();
}

void LogFrame::displayLed(QString msgType)
{
    std::string msgt(msgType.toStdString());
    if (msgt == QPF::MSG_INDATA) {
        ui->lblMsgData->setStyleSheet("{bgcolor: #white;}");
    } else if (msgt == QPF::MSG_DATA_INFO) {
        ui->lblMsgData->setStyleSheet("{bgcolor: #cyan;}");
    } else if (msgt == QPF::MSG_DATA_RQST) {
        ui->lblMsgData->setStyleSheet("{bgcolor: #purple;}");
    } else if (msgt == QPF::MSG_CMD) {
        ui->lblMsgCmd->setStyleSheet("{bgcolor: #blue;}");
    } else if (msgt == QPF::MSG_MONIT_INFO) {
        ui->lblMsgMonit->setStyleSheet("{bgcolor: #pink;}");
    } else if (msgt == QPF::MSG_MONIT_RQST) {
        ui->lblMsgMonit->setStyleSheet("{bgcolor: #magenta;}");
    } else if (msgt == QPF::MSG_TASK_PROC) {
        ui->lblMsgTsk->setStyleSheet("{bgcolor: #green;}");
    } else if (msgt == QPF::MSG_TASK_RES) {
        ui->lblMsgTsk->setStyleSheet("{bgcolor: #red;}");
    }
    QTimer::singleShot(10, this, SLOT(clearLed()));
}

void LogFrame::clearLed()
{
    ui->lblMsgData->setStyleSheet("");
    ui->lblMsgCmd->setStyleSheet("");
    ui->lblMsgMonit->setStyleSheet("");
    ui->lblMsgTsk->setStyleSheet("");
}

QPlainTextEdit * LogFrame::getTextEditor()
{
    return ui->pltxtLog;
}
