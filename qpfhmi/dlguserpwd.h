/******************************************************************************
 * File:    dlguserpwd.h
 *          Declaration of class DlgUserPwd
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
 *   Declaration of class DlgUserPwd
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

#ifndef DLGUSERPWD_H
#define DLGUSERPWD_H

#include <QDialog>

#include "config.h"

namespace Ui {
class DlgUserPwd;
}

namespace QPF {

class DlgUserPwd : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUserPwd(QWidget *parent = 0);
    ~DlgUserPwd();

public:
    void getValues(QString & u, QString & p);
    void storeValues();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::DlgUserPwd *ui;
};

}

#endif // DLGUSERPWD_H
