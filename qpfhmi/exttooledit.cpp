#include "exttooledit.h"
#include "ui_exttooledit.h"

ExtToolEdit::ExtToolEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtToolEdit)
{
    ui->setupUi(this);
}

ExtToolEdit::~ExtToolEdit()
{
    delete ui;
}
