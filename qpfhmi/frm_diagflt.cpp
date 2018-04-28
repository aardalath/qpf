/******************************************************************************
 * File:    frm_diagflt.cpp
 *          Implementation of class FrmDiagFlt
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
 *   Implementation of class FrmDiagFlt
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

#include "frm_diagflt.h"
#include "ui_frm_diagflt.h"

FrmDiagFlt::FrmDiagFlt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmDiagFlt)
{
    ui->setupUi(this);

    init();
}

FrmDiagFlt::~FrmDiagFlt()
{
    delete ui;
}

void FrmDiagFlt::reset()
{
    changedSection(0);
    ui->cboxComparison->setCurrentIndex(0);
    ui->edValue->setText("");
}

void FrmDiagFlt::init()
{
    if (checks.size() > 0) { updateChecks(); }
    updateFilter();    
}

void FrmDiagFlt::setChecks(QVector<QStringList> chks)
{
    checks = chks;
    updateChecks();
}

void FrmDiagFlt::updateChecks()
{   
    ui->cboxSection->clear();
    ui->cboxSection->addItems(getAllSections());

    ui->cboxDiagnostic->clear();
    ui->cboxDiagnostic->addItem("");

    ui->cboxOutput->clear();
    ui->cboxOutput->addItem("");

    ui->cboxItem->clear();
    ui->cboxItem->addItem("");
}

QStringList FrmDiagFlt::getAllSections()
{
    QStringList lst;
    QString v("");
    lst << "";
    foreach (const QStringList & l, checks) {
        if (l.at(0) != v) {
            v = l.at(0);
            lst << v;
        }
    }
    return lst;
}

QStringList FrmDiagFlt::getAllDiagnostics(QString sec)
{
    filter.section = sec;

    QStringList lst;
    QString v("");
    lst << "";
    foreach (const QStringList & l, checks) {
        if (l.size() > 1) {
            if (l.at(0) == filter.section) {
                if (l.at(1) != v) {
                    v = l.at(1);
                    lst << v;
                }
            }
        }
    }
    return lst;
}

QStringList FrmDiagFlt::getAllOutputs(QString diag)
{
    filter.diag = diag;

    QStringList lst;
    QString v("");
    lst << "";
    foreach (const QStringList & l, checks) {
        if (l.size() > 2) {
            if ((l.at(0) == filter.section) &&
                (l.at(1) == filter.diag)) {
                if (l.at(2) != v) {
                    v = l.at(2);
                    lst << v;
                }
            }
        }
    }
    return lst;
}

QStringList FrmDiagFlt::getAllItems(QString outp)
{
    filter.outGrp = outp;
    QStringList lst;
    QString v("");
    lst << "";
    foreach (const QStringList & l, checks) {
        if (l.size() > 3) {
            if ((l.at(0) == filter.section) &&
                (l.at(1) == filter.diag) &&
                (l.at(2) == filter.outGrp)) {
                if (l.at(3) != v) {
                    v = l.at(3);
                    lst << v;
                }
            }
        }
    }
    return lst;
}

void FrmDiagFlt::changedSection(int x)
{
    ui->cboxDiagnostic->clear();
    ui->cboxDiagnostic->addItems(getAllDiagnostics(ui->cboxSection->itemText(x)));

    changedDiagnostic(0);
}

void FrmDiagFlt::changedDiagnostic(int x)
{
    ui->cboxOutput->clear();
    ui->cboxOutput->addItems(getAllOutputs(ui->cboxDiagnostic->itemText(x)));

    changedOutput(0);
}

void FrmDiagFlt::changedOutput(int x)
{
    ui->cboxItem->clear();
    ui->cboxItem->addItems(getAllItems(ui->cboxOutput->itemText(x)));
    
    changedItem(0);
}

void FrmDiagFlt::changedItem(int x)
{
    filter.item = ui->cboxItem->itemText(x);
    updateFilter();
}

void FrmDiagFlt::changedComparison(int x)
{
    filter.cmp = static_cast<Comparison>(x);
    //ui->edValue->setVisible((x < TOP10) || (x > BELOW_AVG));
    updateFilter();
}

void FrmDiagFlt::changedCompareTo(QString cmps)
{
    filter.cmpString = cmps;
    updateFilter();
}

void FrmDiagFlt::updateFilter()
{
    emit filterIsUpdated(filter);
}

void FrmDiagFlt::emitAddFilter()
{
    emit rqstAddFilter();
}

void FrmDiagFlt::emitRemoveFilter()
{
    emit rqstRemoveFilter();
}

QVector<QStringList> FrmDiagFlt::checks;
