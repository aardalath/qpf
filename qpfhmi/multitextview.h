/******************************************************************************
 * File:    multitextview.h
 *          Declaration of class MultiTextView
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
 *   Declaration of class MultiTextView
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

#ifndef MULTITEXTVIEW_H
#define MULTITEXTVIEW_H

#include <QDialog>

namespace Ui {
class MultiTextView;
}

namespace QPF {

class MultiTextView : public QDialog
{
    Q_OBJECT

public:
    explicit MultiTextView(QWidget *parent = 0);
    ~MultiTextView();
    void setItems(QString itemClass, QStringList & itemNames,
                  QVector<QWidget*> & items);
    void getItems(QString & itemClass, QStringList & itemNames,
                  QVector<QWidget*> & items);

private:
    Ui::MultiTextView *ui;
};

}

#endif // MULTITEXTVIEW_H
