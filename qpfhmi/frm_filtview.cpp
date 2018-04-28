/******************************************************************************
 * File:    frm_filtview.cpp
 *          Implementation of class FrmFiltView
 *
 * Domain:  QPF.qpfgui.frm_filtview
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
 *   Implementation of class FrmFiltView
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

#include "frm_filtview.h"
#include "ui_frm_filtview.h"

FrmFiltView::FrmFiltView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmFiltView)
{
    ui->setupUi(this);
}

FrmFiltView::~FrmFiltView()
{
    delete ui;
}

QTreeView * FrmFiltView::initialize(QString name, QString qry)
{
    ui->lblName->setText(name);
    ui->lblQry->setText(qry);
    return ui->treevwFiltView;
}

