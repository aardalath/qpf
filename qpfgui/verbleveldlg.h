#ifndef VERBLEVELDLG_H
#define VERBLEVELDLG_H

#include <QDialog>

namespace Ui {
class VerbLevelDlg;
}

namespace QPF {

class VerbLevelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit VerbLevelDlg(QWidget *parent = 0);
    ~VerbLevelDlg();
    QString getVerbosityLevelName();

public slots:
    void setVerbosityLevel(int lvl);

private:
    Ui::VerbLevelDlg *ui;
    QStringList verbLevels;
    QString currentLevel;
};

}

#endif // VERBLEVELDLG_H
