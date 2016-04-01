#ifndef EXTTOOLSDEF_H
#define EXTTOOLSDEF_H

#include <QDialog>

namespace Ui {
class ExtToolsDef;
}

class ExtToolsDef : public QDialog
{
    Q_OBJECT

public:
    explicit ExtToolsDef(QWidget *parent = 0);
    ~ExtToolsDef();

private:
    Ui::ExtToolsDef *ui;
};

#endif // EXTTOOLSDEF_H
