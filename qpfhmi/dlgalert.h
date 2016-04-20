#ifndef DLGALERT_H
#define DLGALERT_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class DlgAlert;
}

struct Alert {
    QDateTime timeStamp;
    QString id;
    QString severity;
    QString component;
    QString description;
};

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

#endif // DLGALERT_H
