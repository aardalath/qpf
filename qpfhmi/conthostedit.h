/******************************************************************************
 * File:    conthostedit.h
 *          Declaration of class ContHostEdit
 *
 * Domain:  QPF.qpfgui.conthostedit
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
 *   Declaration of class ContHost
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

#ifndef CONTHOSTEDIT_H
#define CONTHOSTEDIT_H

#include <QDialog>

#include "hmitypes.h"

namespace Ui {
class ContHostEdit;
}

namespace QPF {

class ContHostEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ContHostEdit(QWidget *parent = 0);
    ~ContHostEdit();

public:
    void setContHost(QString ip, bool isMaster, int numAg);
    void getContHost(QString & ip, bool & isMaster, int & numAg);

private:
    Ui::ContHostEdit *ui;
};

}

#endif // CONTHOSTEDIT_H
