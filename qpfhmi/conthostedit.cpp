/******************************************************************************
 * File:    conthostedit.cpp
 *          Implementation of class ContHostEdit
 *
 * Domain:  QPF.qpfgui.conthostedit
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
 *   Implementation of class ContHostEdit
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

#include "conthostedit.h"
#include "ui_conthostedit.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace QPF {

ContHostEdit::ContHostEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContHostEdit)
{
    ui->setupUi(this);
}

ContHostEdit::~ContHostEdit()
{
    delete ui;
}

void ContHostEdit::setContHost(QString ip, bool isMaster, int numAg)
{
    ui->edHostIP->setText(ip);
    ui->rbtnMaster->setChecked(isMaster);
    ui->rbtnProcessing->setChecked(!isMaster);
    ui->spbxNumAgents->setValue(numAg);
}

void ContHostEdit::getContHost(QString & ip, bool & isMaster, int & numAg)
{
    ip       = ui->edHostIP->text();
    isMaster = ui->rbtnMaster->isChecked();
    numAg    = ui->spbxNumAgents->value();
}    

}
