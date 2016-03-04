/******************************************************************************
 * File:    taskagent.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.TaskAgent
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
 *   Declare TaskAgent class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   Component
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

#ifndef TASKAGENT_H
#define TASKAGENT_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - thread
//------------------------------------------------------------

#include <thread>
#include <mutex>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - component.h
//------------------------------------------------------------

#include "component.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: TaskAgent
//==========================================================================
class TaskAgent : public Component {

public:
    TaskAgent(const char * name = 0);

    Property(TaskAgent, std::string, workDir, WorkDir);

protected:

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

protected:

    //----------------------------------------------------------------------
    // Method: processTASK_PROC
    //----------------------------------------------------------------------
    virtual void processTASK_PROC();

    //----------------------------------------------------------------------
    // Method: execAdditonalLoopTasks
    //----------------------------------------------------------------------
    virtual void execAdditonalLoopTasks();

private:

    //----------------------------------------------------------------------
    // Method: executeProcessingElement
    //----------------------------------------------------------------------
    void executeProcessingElement(TaskInfo t);

    //----------------------------------------------------------------------
    // Method: executeFakeProcessingElement
    //----------------------------------------------------------------------
    void executeFakeProcessingElement(std::string pe);

    //----------------------------------------------------------------------
    // Method: sendTaskResMsg
    //----------------------------------------------------------------------
    bool sendTaskResMsg(TaskInfo & task);

    //----------------------------------------------------------------------
    // Method: timeTag
    //----------------------------------------------------------------------
    std::string timeTag();


private:
    std::vector<std::thread *> procTasks;
    std::atomic<bool> stopTasks;
    std::atomic<int> numTasks;
    std::atomic<int> numRunningTasks;
    std::atomic<int> numWaitingTasks;
    std::atomic<int> maxRunningTasks;
    std::mutex sendMsgsMutex;
};

}

#endif  /* TASKAGENT_H */
