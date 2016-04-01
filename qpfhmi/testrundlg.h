#ifndef TESTRUNDLG_H
#define TESTRUNDLG_H

#include <QDialog>

namespace Ui {
class TestRunDlg;
}

namespace QPF {

class TestRunDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TestRunDlg(QWidget *parent = 0);
    ~TestRunDlg();

    void readConfig();

private:
    Ui::TestRunDlg *ui;
};

}

#endif // TESTRUNDLG_H
