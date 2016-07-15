#ifndef DLGDATAING_H
#define DLGDATAING_H

#include <QDialog>

namespace Ui {
class DlgDataIngestion;
}

class DlgDataIngestion : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDataIngestion(QWidget *parent = 0);
    ~DlgDataIngestion();

private:
    Ui::DlgDataIngestion *ui;
};

#endif // DLGDATAING_H
