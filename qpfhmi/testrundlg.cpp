#include "testrundlg.h"
#include "ui_testrundlg.h"

namespace QPF {

TestRunDlg::TestRunDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestRunDlg)
{
    ui->setupUi(this);
}

TestRunDlg::~TestRunDlg()
{
    delete ui;
}

}
