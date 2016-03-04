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

private:

    //----------------------------------------------------------------------
    // Method: executeProcessingElement
    //----------------------------------------------------------------------
    void executeProcessingElement(TaskInfo task);

    //----------------------------------------------------------------------
    // Method: runProcessor
    // Prepares a task processor environment, and launches it
    //----------------------------------------------------------------------
    bool runProcessor(std::string exchangeDir, std::string pe);

    //----------------------------------------------------------------------
    // Method: executeProcessingElement
    // repares a task processor environment and folders
    //----------------------------------------------------------------------
    bool prepareFolders(std::string exchangeDir,
                        std::string & taskDriver, std::string & cfgFile);

    //----------------------------------------------------------------------
    // Method: executeTaskDriver
    // Executes a task's taskDriver
    //----------------------------------------------------------------------
    void executeTaskDriver(std::string pe,
                           std::string taskDriver, std::string cfgFile);

    //----------------------------------------------------------------------
    // Method: getDockerId
    // Returns the docker id of a docker task
    //----------------------------------------------------------------------
    std::string getDockerId(std::string exchangeDir);

    //----------------------------------------------------------------------
    // Method: monitorDockerTask
    // Continuously monitor Docker executing task
    //----------------------------------------------------------------------
    bool monitorDockerTask(std::string & dockerId,
                           TaskInfo & task, Json::Value & taskData);

    //----------------------------------------------------------------------
    // Method: reportEndOfTask
    // Send final TASK_RES message after a Docker task is finished
    //----------------------------------------------------------------------
    void reportEndOfTask(TaskInfo & task, Json::Value & taskData);

    //----------------------------------------------------------------------
    // Method: executeFakeProcessingElement
    //----------------------------------------------------------------------
    void executeFakeProcessingElement(std::string pe);

    //----------------------------------------------------------------------
    // Method: sendTaskResMsg
    //----------------------------------------------------------------------
    bool sendTaskResMsg(TaskInfo & task);

    //----------------------------------------------------------------------
    // Method: inspectDockerRunningTask
    //----------------------------------------------------------------------
    void inspectDockerRunningTask(std::string id, std::string & result);

    //----------------------------------------------------------------------
    // Method: timeTag
    //----------------------------------------------------------------------
    std::string timeTag();

    //----------------------------------------------------------------------
    // Method: createNewTaskInfo
    // Add basic information about processing to task
    //----------------------------------------------------------------------
    void createNewTaskInfo(TaskInfo & task);


private:
    std::vector<std::thread *> procTasks;
    std::atomic<bool> stopTasks;
    std::atomic<int> numTasks;
    std::atomic<int> numRunningTasks;
    std::atomic<int> maxRunningTasks;
};

}

#endif  /* TASKAGENT_H */
