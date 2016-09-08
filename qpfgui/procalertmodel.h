/******************************************************************************
 * File:    procalertmodel.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.procalertmodel
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of several dataprocalertmodel for QPF HMI
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
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/
#ifndef PROCALERTMODEL_H
#define PROCALERTMODEL_H

#include "dbtblmodel.h"

namespace QPF {

class ProcAlertModel : public DBTableModel {

    Q_OBJECT

public:
    explicit ProcAlertModel();
};

}

#endif // PROCALERTMODEL_H