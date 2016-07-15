#ifndef DBBROWSER_H
#define DBBROWSER_H

#include <QDialog>

namespace Ui {
class DBBrowser;
}

class DBBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit DBBrowser(QWidget *parent = 0);
    ~DBBrowser();

private:
    Ui::DBBrowser *ui;
};

#endif // DBBROWSER_H
