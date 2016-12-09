/******************************************************************************
 * File:    verbleveldlg.cpp
 *          Implementation of class VerbLevelDlg
 *
 * Domain:  QPF.qpfgui.verbleveldlg
 *
 * Version: 1.1
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implementation of class VerbLevelDlg
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

#include "verbleveldlg.h"
#include "ui_verbleveldlg.h"

#include "log.h"
using namespace LibComm;

namespace QPF {

VerbLevelDlg::VerbLevelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerbLevelDlg)
{
    ui->setupUi(this);

    verbLevels << "TRACE"
               << "DEBUG"
               << "INFORMATIONAL"
               << "WARNING"
               << "ERROR"
               << "FATAL";

    int uidx = (int)(Log::getMinLogLevel());
    ui->listWidget->addItems(verbLevels);
    ui->listWidget->setCurrentRow(uidx);

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(setVerbosityLevel(int)));
}

void VerbLevelDlg::setVerbosityLevel(int lvl)
{
    Log::setMinLogLevel((Log::LogLevel)(lvl));
    currentLevel = ui->listWidget->currentItem()->text();
}

QString VerbLevelDlg::getVerbosityLevelName()
{
    return currentLevel;
}

VerbLevelDlg::~VerbLevelDlg()
{
    delete ui;
}

}
#include "verbleveldlg.h"
#include "ui_verbleveldlg.h"
