#include "dbbrowser.h"
#include "ui_dbbrowser.h"

DBBrowser::DBBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBBrowser)
{
    ui->setupUi(this);
}

DBBrowser::~DBBrowser()
{
    delete ui;
}
