/******************************************************************************
 * File:    dlguserpwd.cpp
 *          Implementation of class DlgUserPwd
 *
 * Domain:  QPF.qpfgui.dlguserpwd
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
 *   Implementation of class DlgUserPwd
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

#include "dlguserpwd.h"
#include "ui_dlguserpwd.h"

#include "keyevt.h"

using Configuration::cfg;

namespace QPF {
    
DlgUserPwd::DlgUserPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUserPwd)
{
    ui->setupUi(this);
    ui->lblCapsLockIcon->setVisible( checkCapsLock() );
    ui->edUser->setText(QString::fromStdString(cfg.connectivity.vospace.user()));
    ui->edPwd->setText(QString::fromStdString(cfg.connectivity.vospace.pwd()));  
}

DlgUserPwd::~DlgUserPwd()
{
    delete ui;
}

void DlgUserPwd::keyPressEvent(QKeyEvent *event)
{
    ui->lblCapsLockIcon->setVisible( checkCapsLock() );
    QWidget::keyPressEvent(event);
}
    
void DlgUserPwd::getValues(QString & u, QString & p)
{
    u = ui->edUser->text();
    p = ui->edPwd->text();
}

void DlgUserPwd::storeValues()
{
    cfg.connectivity.vospace["user"] = ui->edUser->text().toStdString();
    cfg.connectivity.vospace["pwd"]  = ui->edPwd->text().toStdString();
}

}
