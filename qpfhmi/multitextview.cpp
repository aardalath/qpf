/******************************************************************************
 * File:    multitextview.cpp
 *          Implementation of class MultiTextView
 *
 * Domain:  QPF.qpfgui.multitextview
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
 *   Implementation of class MultiTextView
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

#include "multitextview.h"
#include "ui_multitextview.h"

namespace QPF {

MultiTextView::MultiTextView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiTextView)
{
    ui->setupUi(this);
}

MultiTextView::~MultiTextView()
{
    delete ui;
}

void MultiTextView::setItems(QString itemClass, QStringList & itemNames,
                             QVector<QWidget*> & items)
{
    ui->lblItem->setText(itemClass);
    ui->cboxItems->addItems(itemNames);
    foreach (QWidget * s, items) {
        ui->stckItems->addWidget(s);
    }
    connect(ui->cboxItems, SIGNAL(currentIndexChanged(int)),
            ui->stckItems, SLOT(setCurrentIndex(int)));
}
    
void MultiTextView::getItems(QString & itemClass, QStringList & itemNames,
                             QVector<QWidget*> & items)
{
    itemClass = ui->lblItem->text();
    itemNames.clear();
    for (int i = 0; i < ui->cboxItems->count(); ++i) {
        itemNames << ui->cboxItems->itemText(i); 
    }
    items.clear();
    for (int i = ui->cboxItems->count() - 1; i >= 0; --i) {
        QWidget * w = ui->stckItems->widget(i);
        items.append(w);        
        ui->stckItems->removeWidget(w);
    }
}

}
