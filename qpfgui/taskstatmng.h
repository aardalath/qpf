/******************************************************************************
 * File:    taskstatmng.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskstatmng
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
 *   Declaration of several datataskstatmng for QPF HMI
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
#ifndef TASKSTATUSMANAGER_H
#define TASKSTATUSMANAGER_H

//namespace QPF {

class TaskStatusManager {

    Q_OBJECT

public:
    explicit TaskStatusManager(QObject *parent = 0);
    ~TaskStatusManager();

signals:
    void newTaskStatusInfo();

private:
    void checkTaskStatusInfo();

};

}

#endif // TASKSTATUSMANAGER_H