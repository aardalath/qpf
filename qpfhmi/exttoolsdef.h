#ifndef EXTTOOLSDEF_H
#define EXTTOOLSDEF_H

#include <QDialog>
#include <QModelIndex>
#include <QTableWidgetItem>

#include "exttooledit.h"

namespace Ui {
class ExtToolsDef;
}

namespace QPF {

class ExtToolsDef : public QDialog
{
    Q_OBJECT

public:
    explicit ExtToolsDef(QWidget *parent = 0);
    ~ExtToolsDef();

    void initialize(MapOfUserDefTools & userTools, QStringList pts);
    void getTools(MapOfUserDefTools & userTools);

private slots:
    void addNewTool();
    void editTool(QModelIndex idx);
    void editTool();
    void editTool(int row);
    void removeTool();
    void cancelDlg();
    void changeToolWithItem(QTableWidgetItem * item);

private:
    Ui::ExtToolsDef *ui;
    MapOfUserDefTools userDefTools;
    MapOfUserDefTools origDefTools;
    QStringList       prodTypes;
};

}

#endif // EXTTOOLSDEF_H
