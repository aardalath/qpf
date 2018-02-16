/******************************************************************************
 * File:    swarmedit.h
 *          Declaration of class SwarmEdit
 *
 * Domain:  QPF.qpfgui.swarmedit
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
 *   Declaration of class SwarmEdit
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

#ifndef SWARMEDIT_H
#define SWARMEDIT_H

#include <QDialog>

#include "hmitypes.h"

namespace Ui {
class SwarmEdit;
}

namespace QPF {

class SwarmEdit : public QDialog
{
    Q_OBJECT

public:
    explicit SwarmEdit(QWidget *parent = 0);
    ~SwarmEdit();

public:
    void setSwarm(QString id, QString name,
                  QString ip, QStringList wips, int scale,
                  QString image, QString exe, QString args);
    void getSwarm(QString & id, QString & name,
                  QString & ip, QStringList & wips, int & scale,
                  QString & image, QString & exe, QString & args);

private:
    Ui::SwarmEdit *ui;
};

}

#endif // SWARMEDIT_H
