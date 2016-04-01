#include "exttoolsdef.h"
#include "ui_exttoolsdef.h"

ExtToolsDef::ExtToolsDef(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtToolsDef)
{
    ui->setupUi(this);
}

ExtToolsDef::~ExtToolsDef()
{
    delete ui;
}
