#ifndef DLGQDTFILTER_H
#define DLGQDTFILTER_H

#include <QDialog>
#include <QHash>

#include "qdtfilters.h"

namespace Ui {
class DlgQdtFilter;
}

class DlgQdtFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DlgQdtFilter(QVector<QStringList> chks, QDialog *parent = 0);
    ~DlgQdtFilter();

    void setProductsList(QStringList lst);
    void setCurrentSelection(QStringList l);
    
    void reset();

    void getQry(QString & name, QString & qry);

public slots:
    void addFilter();
    void removeFilter();
    void matchAllCriteria();
    void matchAnyCriteria();
    void setProductSelectionType(int);
    void setProductType(QString);
    void setDateFrom(QDateTime x);
    void setDateTo(QDateTime x);
    void setCustomSQL();
    void updateSQL();

    void setFilter(QDTSingleFilter flt);

    void selectDateTimeAndApply();

    void eval();
    void go();

    QString convertToSql(QString s);
    
signals:
    void newFilterToApply(QString);

private:
    void init();

private:
    Ui::DlgQdtFilter * ui;

    QVector<QStringList> checks;
    QStringList currentSelectionIDs;
    QDTFilter filter;

    QString filterEval;
    QString filterEvalSql;
};

#endif // DLGQDTFILTER_H
