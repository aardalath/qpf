/******************************************************************************
 * File:    prodfiltmodel.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.prodfiltmodel
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of several ProductsFiltModel for QPF HMI
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
#ifndef PRODFILTMODEL_H
#define PRODFILTMODEL_H

#include "dbtreemodel.h"

namespace QPF {

class ProductsFilterModel : public DBTreeModel {

    Q_OBJECT

public:
    explicit ProductsFilterModel(QString qry);

};

}

#endif // PRODFILTMODEL_H
