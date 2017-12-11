/******************************************************************************
 * File:    dlgreproc.cpp
 *          Implementation of class DlgReproc
 *
 * Domain:  QPF.qpfgui.dlgreproc
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
 *   Implementation of class DlgReproc
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

#include "dlgreproc.h"
#include "ui_dlgreproc.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "config.h"
using Configuration::cfg;

#define QS(x) QString::fromStdString(x)

namespace QPF {

DlgReproc::DlgReproc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgReproc)
{
    ui->setupUi(this);

    ui->btngrpOutputsLocation->setId(ui->rbtnDefaultUserArea, DefUserArea);   
    ui->btngrpOutputsLocation->setId(ui->rbtnCfgLocal       , CfgLocalDir);  
    ui->btngrpOutputsLocation->setId(ui->rbtnCfgVOSpace     , CfgVOSpace);  
    ui->btngrpOutputsLocation->setId(ui->rbtnLocal          , LocalDir);     
    ui->btngrpOutputsLocation->setId(ui->rbtnVOSpace        , VOSpaceFolder);

    connect(ui->tbtnLocal, SIGNAL(clicked()), this, SLOT(selectLocalFolder()));
}

DlgReproc::~DlgReproc()
{
    delete ui;
}

void DlgReproc::setFields(QStringList inProds, OutputsLocation out,
                          int flags)
    
{
    ui->lstInputProducts->addItems(inProds);

    ui->edDefaultUserArea->setText(QS(cfg.general.workArea() + "/data/user"));
    ui->edCfgLocal->setText(QS(cfg.general.userArea()));
    ui->edCfgVOSpace->setText(QS("VOSpace://" + cfg.connectivity.vospace.user()
                               + "/" + cfg.connectivity.vospace.folder()));
    ui->edLocal->setText("");
    ui->edVOSpace->setText("");

    ui->btngrpOutputsLocation->button(out)->setChecked(true);

    ui->chkGenIntermProd->setChecked(flags & GenIntermProd);
    ui->chkIPython->setChecked(flags & OpenIPython);
    ui->chkJupLab->setChecked(flags & OpenJupyterLab);
    ui->chkBrowserVOSpace->setChecked(flags & OpenVOSpace);
}
    
void DlgReproc::getFields(QStringList & inProds, OutputsLocation & out,
                          QString & outDir, int & flags)
{
    out = static_cast<OutputsLocation>(ui->btngrpOutputsLocation->checkedId());
    switch (out) {
    case DefUserArea:
        outDir = ui->edDefaultUserArea->text();
        break;  
    case CfgLocalDir:
        outDir = ui->edCfgLocal->text();
        break;  
    case CfgVOSpace:
        outDir = ""; //ui->edCfgVOSpace->text();
        break;   
    case LocalDir:
        outDir = ui->edLocal->text();
        break;     
    case VOSpaceFolder:
        outDir = ui->edVOSpace->text();
        break;
    default:
        break;
    }

    flags = (int(ui->chkGenIntermProd->isChecked() ? GenIntermProd : NullFlags) |
             int(ui->chkIPython->isChecked() ? OpenIPython : NullFlags) |
             int(ui->chkJupLab->isChecked() ? OpenJupyterLab : NullFlags) |
             int(ui->chkBrowserVOSpace->isChecked() ? OpenVOSpace : NullFlags));
}

void DlgReproc::selectLocalFolder()
{
    QString pathName("/");
    pathName = QFileDialog::getExistingDirectory(this,
                                                 tr("Select local folder"),
                                                 pathName);
    QFileInfo fs(pathName);
    if (fs.exists()) {
        ui->edLocal->setText(pathName);
    }
}

}
