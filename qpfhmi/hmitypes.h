/******************************************************************************
 * File:    hmitypes.h
 *          Declaration of class
 *
 * Domain:  QPF.qpfgui.hmitypes
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
 *   Declaration of class
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

#ifndef HMITYPES_H
#define HMITYPES_H

#include <QMap>
#include <QString>

namespace QPF {

struct QUserDefTool {
    QString name;
    QString desc;
    QString exe;
    QString args;
    QStringList prod_types;
};

typedef QMap<QString, QUserDefTool> MapOfUserDefTools;

 enum Flags {
    NullFlags      = 0x00,
    GenIntermProd  = 0x01,
    OpenIPython    = 0x02,
    OpenJupyterLab = 0x04,
    OpenVOSpace    = 0x08,
};

enum OutputsLocation {
    LocalArch      = 0x100,
    LocalDir       = 0x200,
    VOSpaceFolder  = 0x400,
};
        
}

#endif // HMITYPES_H
