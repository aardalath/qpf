#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QDialog>

namespace Ui {
class ConfigTool;
}

namespace QPF {

class ConfigTool : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigTool(QWidget *parent = 0);
    ~ConfigTool();

    void readConfig();

private:
    Ui::ConfigTool *ui;
};

}

#endif // CONFIGTOOL_H
