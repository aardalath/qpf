/******************************************************************************
 * File:    anasession.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.anasession
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
 *   Declaration of several dataanasession for QPF HMI
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
#ifndef ANALYSISSESSION_H
#define ANALYSISSESSION_H

#include <QObject>

namespace QPF {

class AnalysisSession : public QObject {

    Q_OBJECT

public:
    explicit AnalysisSession(QObject *parent = 0);
    ~AnalysisSession();

signals:
    void activate();


private:
    int a;
};

}

#endif // ANALYSISSESSION_H
