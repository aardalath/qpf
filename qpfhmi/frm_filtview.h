/******************************************************************************
 * File:    frm_filtview.h
 *          Declaration of class FrmFiltView
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
 *   Declaration of class Frm_DiagFlt
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

#ifndef FRM_FILTVIEW_H
#define FRM_FILTVIEW_H

#include <QTreeView>

namespace Ui {
class FrmFiltView;
}

class FrmFiltView : public QWidget
{
    Q_OBJECT

public:
    explicit FrmFiltView(QWidget *parent = 0);
    ~FrmFiltView();

    QTreeView * initialize(QString name, QString qry);

private:
    Ui::FrmFiltView * ui;
};

#endif // FRM_FILTVIEW_H
