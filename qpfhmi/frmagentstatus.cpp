#include "frmagentstatus.h"
#include "ui_frmagentstatus.h"

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
