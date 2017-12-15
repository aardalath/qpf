/******************************************************************************
 * File:    ruleedit.cpp
 *          Implementation of class RuleEdit
 *
 * Domain:  QPF.qpfgui.ruleedit
 *
 * Version:  1.2
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implementation of class RuleEdit
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

#include "ruleedit.h"
#include "ui_ruleedit.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace QPF {

RuleEdit::RuleEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleEdit)
{
    ui->setupUi(this);
}

RuleEdit::~RuleEdit()
{
    delete ui;
}

void RuleEdit::setRule(QString name,
                       QStringList products,
                       QVector<int> selInputs, QVector<int> selOutputs,
                       QString condition,
                       QStringList processors, QString processor)
{
    ui->edName->setText(name);
    ui->lstwdgInputs->addItems(products);
    ui->lstwdgOutputs->addItems(products);
    ui->edCondition->setText(condition);
    ui->cboxProcessor->addItems(processors);

    ui->lstwdgInputs->setSelectionMode(QAbstractItemView::MultiSelection);
    for (auto & i : selInputs) { ui->lstwdgInputs->item(i)->setSelected(true); }

    ui->lstwdgOutputs->setSelectionMode(QAbstractItemView::MultiSelection);
    for (auto & i : selOutputs) { ui->lstwdgOutputs->item(i)->setSelected(true); }
    
    ui->cboxProcessor->setCurrentText(processor);
}
    
void RuleEdit::getRule(QString & name, QStringList & inputs, QStringList & outputs,
                       QString & condition, QString & processor)
{
    QStringList l;
    
    name = ui->edName->text();

    inputs.clear();
    foreach (QListWidgetItem * it, ui->lstwdgInputs->selectedItems()) { inputs << it->text(); }
    
    outputs.clear();
    foreach (QListWidgetItem * it, ui->lstwdgOutputs->selectedItems()) { outputs << it->text(); }

    condition = ui->edCondition->text();
    processor = ui->cboxProcessor->currentText();
}

}
