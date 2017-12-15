/******************************************************************************
 * File:    exporthdl.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.exporthdl
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of several dataexporthdl for QPF HMI
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
#ifndef EXPORTHANDLER_H
#define EXPORTHANDLER_H

#include <QObject>

namespace QPF {

class ExportHandler : public QObject {

    Q_OBJECT

public:
    explicit ExportHandler(QObject *parent = 0);
    ~ExportHandler();

signals:
    void activate();

private:
    int a;
};

}

#endif // EXPORTHANDLER_H
