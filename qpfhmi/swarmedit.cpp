/******************************************************************************
 * File:    swarmedit.cpp
 *          Implementation of class SwarmEdit
 *
 * Domain:  QPF.qpfgui.swarmedit
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
 *   Implementation of class SwarmEdit
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

#include "swarmedit.h"
#include "ui_swarmedit.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace QPF {

SwarmEdit::SwarmEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SwarmEdit)
{
    ui->setupUi(this);
}

SwarmEdit::~SwarmEdit()
{
    delete ui;
}

void SwarmEdit::setSwarm(QString id, QString name,
                         QString ip, QStringList wips, int scale,
                         QString image, QString exe, QString args)
{
    ui->edSwarmId->setText(id);
    ui->edSrvName->setText(name);
    ui->edManagerIP->setText(ip);
    ui->edWorkerIPs->setText(wips.join(" "));
    ui->spbxScale->setValue(scale);
    ui->edImage->setText(image);
    ui->edExe->setText(exe);
    ui->edArgs->setText(args);
}
    
void SwarmEdit::getSwarm(QString & id, QString & name,
                         QString & ip, QStringList & wips, int & scale,
                         QString & image, QString & exe, QString & args)
{
    id    = ui->edSwarmId->text();
    name  = ui->edSrvName->text();
    ip    = ui->edManagerIP->text();
    wips  = ui->edWorkerIPs->text().split(QRegExp("\\W+"), QString::SkipEmptyParts);
    scale = ui->spbxScale->value();
    image = ui->edImage->text();
    exe   = ui->edExe->text();
    args  = ui->edArgs->text();
}

}
