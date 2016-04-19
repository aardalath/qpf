#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QDialog>

#include <QtWidgets>

class ModelView : public QWidget {
    Q_OBJECT
public:
    enum Type { List, Table };

    ModelView(QVector<QStringList> & vdlist,
              QAbstractItemView  * v = 0,
              QAbstractItemModel * m = 0);
    ModelView(QStringList & dlist,
              QAbstractItemView  * v = 0,
              QAbstractItemModel * m = 0);

    QAbstractItemModel *  getModel() { return model; }
    QAbstractItemView *   getView()  { return view; }
    QItemSelectionModel * getSelectionModel() { return selectionModel; }

    void setHeader(QString h);
    void setHeaders(QStringList & hlist);

private:
    void setData(QStringList & dlist);
    void setData(QVector<QStringList> & vdlist);

    void setupModel();
    void setupViews();

    QAbstractItemView  *view;
    QAbstractItemModel *model;
    QItemSelectionModel *selectionModel;
    Type type;
};

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

    enum PageIndex {
        PageGeneral,
        PageMachines,
        PageProdProc,
        PageNetwork,
        PageOrchestration,
        PageStorage,
    };

    void readConfig();

public slots:
    void save();
    void saveAs();

private:
    ModelView * createListModelView(QAbstractItemView * v,
                                    QStringList & dlist,
                                    QString hdr);

    ModelView * createTableModelView(QAbstractItemView * v,
                                     QVector<QStringList> & vdlist,
                                     QStringList & hdr);

private:
    Ui::ConfigTool *ui;
};

}

#endif // CONFIGTOOL_H
