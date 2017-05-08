/******************************************************************************
 * File:    procelem.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.procelem
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare ProcessingElement class
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

#ifndef PROCESSINGELEMENT_H
#define PROCESSINGELEMENT_H

#include "datatypes.h"
#include "cfg.h"
#include "urlhdl.h"

#include "dbg.h"

#include <atomic>
#include <thread>
#include <mutex>

namespace QPF {

class TaskAgent;

class ProcessingElement
{
public:
    ProcessingElement(TaskAgent * parent = 0);
    ~ProcessingElement();

    Property(ProcessingElement, std::string, agentName, AgentName);
    Property(ProcessingElement, std::string, workDir,   WorkDir);
    Property(ProcessingElement, std::string, sysDir,    SysDir);
    Property(ProcessingElement, int,         numTask,   NumTask);

public:
    //----------------------------------------------------------------------
    // Method: setTaskInfo
    // Sets the task information for the processing element execution
    //----------------------------------------------------------------------
    void setTaskInfo(TaskInfo & t);

    //----------------------------------------------------------------------
    // Method: getStatus
    // Returns the processing element status
    //----------------------------------------------------------------------
    TaskStatus getStatus();

    //----------------------------------------------------------------------
    // Method: getTask
    // Returns the TaskInfo structure
    //----------------------------------------------------------------------
    TaskInfo & getTask();

    //----------------------------------------------------------------------
    // Method: start
    // Flags the start of the underlying processor
    //----------------------------------------------------------------------
    void start();

    //----------------------------------------------------------------------
    // Method: end
    // Requests the end of the processor
    //----------------------------------------------------------------------
    void end();

    //----------------------------------------------------------------------
    // Method: exec
    // Executes a task
    //----------------------------------------------------------------------
    void exec();

protected:
    //----------------------------------------------------------------------
    // Method: initTaskInfo
    // Initializes task information
    //----------------------------------------------------------------------
    void initTaskInfo();

    //----------------------------------------------------------------------
    // Method: configureProcElem
    // Build config files and dir structure
    //----------------------------------------------------------------------
    void configureProcElem();

    //----------------------------------------------------------------------
    // Method: executeProcessorinternalTaskNameIdx
    // Forks process to run processor
    //----------------------------------------------------------------------
    void executeProcessor();

    //----------------------------------------------------------------------
    // Method: forkProcess
    // Forks process to run processor
    //----------------------------------------------------------------------
    void forkProcess();

    //----------------------------------------------------------------------
    // Method: obtainProcElemId
    // Obtain processing element identifier
    //----------------------------------------------------------------------
    void obtainProcElemId();

    //----------------------------------------------------------------------
    // Method: monitorProcElem
    // Monitorice running processing element
    //----------------------------------------------------------------------
    void monitorProcElemLoop();

    //----------------------------------------------------------------------
    // Method: prepareOutputFile
    // Prepare the output product file to be retrieved
    //----------------------------------------------------------------------
    std::string prepareOutputFile(std::string vd, std::string& dname);

    //----------------------------------------------------------------------
    // Method: retrieveOutputProducts
    // Gets information about output product files for archival purposes
    //----------------------------------------------------------------------
    void retrieveOutputProducts();

    //----------------------------------------------------------------------
    // Method: sendUpdatedInfo
    // Sends updated information about the running task
    //----------------------------------------------------------------------
    void sendUpdatedInfo();

    //----------------------------------------------------------------------
    // Method: cleanup
    // Finish processing element activities and mark as FINISHED
    //----------------------------------------------------------------------
    void cleanup();

    //----------------------------------------------------------------------
    // Method: getMonitoringInfo
    // Request monitoring information to the running docker container
    //----------------------------------------------------------------------
    std::string getMonitoringInfo(std::string id);

    //----------------------------------------------------------------------
    // Method: updateProgress
    // Update progress information
    //----------------------------------------------------------------------
    float updateProgress(float f = 100.);

    //----------------------------------------------------------------------
    // Method: getSimplifiedDateTime
    // Remove separators from Docker datatime string
    //----------------------------------------------------------------------
    std::string getSimplifiedDateTime(Json::Value v);

    //----------------------------------------------------------------------
    // Method: getExpandedDateTime
    // Add separators to timeTag date+time string
    //----------------------------------------------------------------------
    std::string getExpandedDateTime(std::string s);

    //----------------------------------------------------------------------
    // Method: goIdle
    // Sleep running thread for a number of microseconds
    //----------------------------------------------------------------------
    void goIdle(int us);

private:
    TaskAgent * super;

    TaskInfo task;
    std::mutex mutexTask;

    TaskStatus status;

    std::string pe;

    std::string exchangeDir;
    std::string internalTaskNameIdx;
    std::string nameFromOrchestrator;
    std::string exchgIn;
    std::string exchgOut;
    std::string exchgLog;

    std::string taskDriver;
    std::string cfgFile;

    std::string dockerIdFile;
    std::string dockerId;
    std::string inspectInfo;
    std::string originalRegKey;

    std::thread peThread;

    bool childEnded = false;

    int estimatedDuration;

    float progressValue;

    bool running, dead, paused, killed, finished;
    int excode;

    bool isRemote;

    std::atomic<bool> startProc;
    std::atomic<bool> endProc;

    const int checkStartSleepPeriod;
    const int threadLoopSleepPeriod;

    URLHandler * urlh;

#ifdef DEBUG_BUILD
    int chkLevel;
#endif
};

}

#endif // PROCESSINGELEMENT_H
