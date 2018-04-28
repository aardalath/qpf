#include "dlgqdtfilter.h"
#include "ui_dlgqdtfilter.h"

#include <QCalendarWidget>
#include <QMessageBox>
#include <QRegExp>

DlgQdtFilter::DlgQdtFilter(QVector<QStringList> chks, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::DlgQdtFilter),
    checks(chks)
{
    ui->setupUi(this);
    ui->wdgDiagFlt->setChecks(chks);
    QDTSingleFilter filt;
    filter.filts[ui->wdgDiagFlt] = filt;

    filter.prodSelType = ALL;
    
    connect(ui->wdgDiagFlt, SIGNAL(filterIsUpdated(QDTSingleFilter)),
            this, SLOT(setFilter(QDTSingleFilter)));
    connect(ui->wdgDiagFlt, SIGNAL(rqstAddFilter()),
            this, SLOT(addFilter()));
    connect(ui->wdgDiagFlt, SIGNAL(rqstRemoveFilter()),
            this, SLOT(removeFilter()));
    
    init();
}

DlgQdtFilter::~DlgQdtFilter()
{
    delete ui;
}

void DlgQdtFilter::init()
{
}

void DlgQdtFilter::setProductsList(QStringList lst)
{
    ui->cboxProdTypes->addItems(lst);
}

void DlgQdtFilter::reset()
{
}

void DlgQdtFilter::getQry(QString & name, QString & qry)
{
    name = ui->edQryName->text();
    qry  = filterEvalSql;
}

void DlgQdtFilter::addFilter()
{
    FrmDiagFlt * o = qobject_cast<FrmDiagFlt*>(sender());
    QVBoxLayout * vly = qobject_cast<QVBoxLayout *>(ui->scrollAreaFilters->widget()->layout());
    int idx = 0;
    do {
        FrmDiagFlt * chld = qobject_cast<FrmDiagFlt*>(vly->itemAt(idx)->widget());
        if (chld == o) {
            FrmDiagFlt * oo = new FrmDiagFlt;
            connect(oo, SIGNAL(filterIsUpdated(QDTSingleFilter)),
                    this, SLOT(setFilter(QDTSingleFilter)));    
            connect(oo, SIGNAL(rqstAddFilter()), this, SLOT(addFilter()));
            connect(oo, SIGNAL(rqstRemoveFilter()), this, SLOT(removeFilter()));
            QDTSingleFilter flt;
            filter.filts[oo] = flt;
            vly->insertWidget(idx + 1, oo);
            oo->show();
            break;
        }
        ++idx;
    } while (true);
}

void DlgQdtFilter::removeFilter()
{
    FrmDiagFlt * o = qobject_cast<FrmDiagFlt*>(sender());
    if (o != ui->wdgDiagFlt) {
        filter.filts.erase(filter.filts.find(o));
        delete o;
    } else {
        o->reset();
    }
}

void DlgQdtFilter::setFilter(QDTSingleFilter flt)
{
    FrmDiagFlt * o = qobject_cast<FrmDiagFlt*>(sender());
    foreach (FrmDiagFlt* f, filter.filts.keys()) {
        if (f == o) { filter.filts[f] = flt; }
    }
}

void DlgQdtFilter::matchAllCriteria()
{
    filter.criteria = ALL_OF_THEM;
}

void DlgQdtFilter::matchAnyCriteria()
{
    filter.criteria = ANY_OF_THEM;
}

void DlgQdtFilter::setProductSelectionType(int x)
{
    filter.prodSelType = (ProductSelType)(x);
}

void DlgQdtFilter::setProductType(QString x)
{
    filter.prodType = x;
}

void DlgQdtFilter::setDateFrom(QDateTime x)
{
    filter.dtFrom = x;
}

void DlgQdtFilter::setDateTo(QDateTime x)
{
    filter.dtTo = x;
}

void DlgQdtFilter::setCustomSQL()
{
    filter.sqlQry = ui->pltxtSQL->toPlainText();
}

void DlgQdtFilter::updateSQL()
{
}

void DlgQdtFilter::selectDateTimeAndApply()
{
    QToolButton * tbtn = qobject_cast<QToolButton*>(sender());
    QCalendarWidget * newCal = new QCalendarWidget;
    newCal->setWindowFlags(Qt::Popup);
    QDateTimeEdit * dt = (tbtn == ui->tbtnFrom) ? ui->datetimeFrom : ui->datetimeTo;
    connect(newCal, &QCalendarWidget::selectionChanged, dt,
            [newCal, dt]() {dt->setDate(newCal->selectedDate());});
    newCal->move(tbtn->mapToGlobal(QPoint(0,tbtn->height())));
    newCal->show();
}

void DlgQdtFilter::setCurrentSelection(QStringList l)
{
    currentSelectionIDs = l;
}

void DlgQdtFilter::eval()
{
    filterEval = filter.evaluate();
    filterEvalSql = convertToSql(filterEval);
    ui->pltxtSQL->setPlainText(filterEval);
    ui->pltxtTrueSQL->setPlainText(filterEvalSql);
}

void DlgQdtFilter::go()
{
    if (ui->edQryName->text().isEmpty()) {
        int ret = QMessageBox::warning(this, tr("Execute query"),
                      tr("Please, enter a name for this query"),
                      QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    
    eval();
    accept();
}

QString DlgQdtFilter::convertToSql(QString s)
{
    QString f(s);
    QString prodSelTypeStr(ProductSelToken[(int)(filter.prodSelType)]);
    QStringList ids;
    
    // Replace product selection section
    switch (filter.prodSelType) {
    case ALL:
        f.replace(prodSelTypeStr, "products_info_filter");
        break;
    case CURRENT:
        ids = currentSelectionIDs;
        f.replace(prodSelTypeStr + " WHERE ",
                  QString("products_info_filter WHERE "
                          "id IN (%1) AND (")
                  .arg(ids.join(", ")));
        f.insert(f.length() - 1, ")");
        break;
    case PROD_TYPE:
        f.replace(QRegExp(prodSelTypeStr + "\\(.*\\) WHERE "),
                  QString("products_info_filter WHERE "
                          "product_type LIKE '%1' AND (")
                  .arg(filter.prodType));
        f.insert(f.length() - 1, ")");
        break;
    case DATE_RANGE:
        f.replace(QRegExp(prodSelTypeStr + "\\(('.*')\\.\\.('.*')\\) WHERE "),
                  "products_info_filter WHERE "
                  "(start_time BETWEEN \\1 AND \\2) AND (");
        f.insert(f.length() - 1, ")");
        break;
    default:
        break;
    }

    // Replace filter specifications
    QRegExp rxFlt("\\{(.+)\\}");
    rxFlt.setMinimal(true);

    QRegExp rxElem("\\[(.+)\\]");
    rxElem.setMinimal(true);

    int pos = 0;
    while ((pos = rxFlt.indexIn(f, pos)) != -1) {
        QString aFlt(rxFlt.cap(1));
        QString newFlt(aFlt);
        bool isLike = (aFlt.indexOf(" LIKE ") > -1);
        int epos = 0;
        if ((epos = rxElem.indexIn(newFlt, epos)) != -1) {
            QString elem(rxElem.cap(1));
            QString newElem;
            
            QStringList elemTokens(elem.split('.'));

            if (elemTokens.at(0) == "diagnostics") {
                newElem = QString("(diag LIKE '%1') AND "
                                  "((value->>'%2')%3")
                    .arg(elemTokens.at(1)).arg(elemTokens.at(3))
                    .arg(isLike ? "" : "::float");
            } else {
                newElem = QString("(diag LIKE '%1') AND "
                                  "((value->>'value')::float")
                    .arg(elemTokens.at(1));
            }

            // Substitute pseudo-code expression with true condition
            newFlt.replace("[" + elem + "]", newElem);
        }

        f.replace("{" + aFlt + "}", "(" + newFlt + "))");
    }

    return f;
}
