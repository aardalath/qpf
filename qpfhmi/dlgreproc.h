/******************************************************************************
 * File:    dlgreproc.h
 *          Declaration of class DlgReproc
 *
 * Domain:  QPF.qpfgui.dlgreproc
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
 *   Declaration of class DlgReproc
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

#ifndef DLGREPROC_H
#define DLGREPROC_H

#include <QDialog>

#include "hmitypes.h"

namespace Ui {
class DlgReproc;
}

namespace QPF {

class DlgReproc : public QDialog
{
    Q_OBJECT

public:
    explicit DlgReproc(QWidget *parent = 0);
    ~DlgReproc();

    enum Flags {
        NullFlags      = 0x00,
        GenIntermProd  = 0x01,
        OpenIPython    = 0x02,
        OpenJupyterLab = 0x04,
        OpenVOSpace    = 0x08,
    };

    enum OutputsLocation {
        DefUserArea,
        CfgLocalDir,
        CfgVOSpace,
        LocalDir,
        VOSpaceFolder,
    };
        
public:
    void setLabels(QString aTitle, QString desc, QString aHeading,
                   bool showAddOpt);
    void setFields(QStringList inProds, OutputsLocation out,
                   int flags);
    void getFields(QStringList & inProds, OutputsLocation & out,
                   QString & outDir, int & flags);

public slots:
    void selectLocalFolder();

private:
    Ui::DlgReproc *ui;
};

}

#endif // DLGREPROC_H
