#ifndef EXTTOOLEDIT_H
#define EXTTOOLEDIT_H

#include <QDialog>

namespace Ui {
class ExtToolEdit;
}

class ExtToolEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ExtToolEdit(QWidget *parent = 0);
    ~ExtToolEdit();

private:
    Ui::ExtToolEdit *ui;
};

#endif // EXTTOOLEDIT_H
