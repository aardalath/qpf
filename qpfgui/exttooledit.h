#ifndef EXTTOOLEDIT_H
#define EXTTOOLEDIT_H

#include <QDialog>

#include "hmitypes.h"

namespace Ui {
class ExtToolEdit;
}

namespace QPF {

class ExtToolEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ExtToolEdit(QWidget *parent = 0);
    ~ExtToolEdit();

public:
    void setProdTypes(QStringList pts);
    void editTool(QUserDefTool & udt);
    void getToolInfo(QUserDefTool & udt);

private slots:
    void showHelp();
    void selectApp();
    void selectProdTypes();

private:
    Ui::ExtToolEdit *ui;
    QStringList prodTypes;
};

}

#endif // EXTTOOLEDIT_H
