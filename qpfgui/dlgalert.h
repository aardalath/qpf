#ifndef DLGALERT_H
#define DLGALERT_H

#include <QDialog>
#include <QDateTime>

#include "alert.h"

namespace Ui {
class DlgAlert;
}

namespace QPF {

class DlgAlert : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAlert(QWidget *parent = 0);
    ~DlgAlert();

    void setAlert(Alert & alert);

private:
    Ui::DlgAlert *ui;
};
}

#endif // DLGALERT_H
