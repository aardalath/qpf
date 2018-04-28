/******************************************************************************
 * File:    frm_diagflt.h
 *          Declaration of class Frm_DiagFlt
 *
 * Domain:  QPF.qpfgui.frm_diagflt
 *
 * Version:  2.0
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of class Frm_DiagFlt
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef FRM_DIAGFLT_H
#define FRM_DIAGFLT_H

#include <QWidget>

#include "qdtfilters.h"

namespace Ui {
class FrmDiagFlt;
}

class FrmDiagFlt : public QWidget
{
    Q_OBJECT

public:
    explicit FrmDiagFlt(QWidget *parent = 0);
    ~FrmDiagFlt();

public slots:
    void reset();
    
    void setChecks(QVector<QStringList> chks);
    void updateChecks();

    void changedSection(int);
    void changedDiagnostic(int);
    void changedOutput(int);
    void changedItem(int);
    void changedComparison(int);
    void changedCompareTo(QString);

    QStringList getAllSections();
    QStringList getAllDiagnostics(QString sec);
    QStringList getAllOutputs(QString diag);
    QStringList getAllItems(QString outp);

    void emitAddFilter();
    void emitRemoveFilter();
    
signals:
    void filterIsUpdated(QDTSingleFilter);
    void rqstAddFilter();
    void rqstRemoveFilter();
    
private:
    void init();
    void updateFilter();

private:
    Ui::FrmDiagFlt * ui;

    static QVector<QStringList> checks;
    QString section;
    QString diagnostic;
    QString output;
    QString item;

    QDTSingleFilter filter;
};

#endif // FRM_DIAGFLT_H
