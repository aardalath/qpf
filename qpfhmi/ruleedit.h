/******************************************************************************
 * File:    ruleedit.h
 *          Declaration of class RuleEdit
 *
 * Domain:  QPF.qpfgui.ruleedit
 *
 * Version:  1.2
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
 *   Declaration of class RuleEdit
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

#ifndef RULEEDIT_H
#define RULEEDIT_H

#include <QDialog>

#include "hmitypes.h"

namespace Ui {
class RuleEdit;
}

namespace QPF {

class RuleEdit : public QDialog
{
    Q_OBJECT

public:
    explicit RuleEdit(QWidget *parent = 0);
    ~RuleEdit();

public:
    void setRule(QString name,
                 QStringList products,
                 QVector<int> selInputs, QVector<int> selOutputs,
                 QString condition,
                 QStringList processors, QString processor);
    void getRule(QString & name, QStringList & inputs, QStringList & outputs,
                 QString & condition, QString & processor);

private:
    Ui::RuleEdit *ui;
};

}

#endif // RULEEDIT_H
