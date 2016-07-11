#include "dlgdataing.h"
#include "ui_dlgdataing.h"

DlgDataIngestion::DlgDataIngestion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDataIngestion)
{
    ui->setupUi(this);
}

DlgDataIngestion::~DlgDataIngestion()
{
    delete ui;
}
