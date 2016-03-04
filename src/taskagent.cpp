/******************************************************************************
 * File:    taskagent.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskagent
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
 *   Implement TaskAgent class
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

#include "taskagent.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <random>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TaskAgent::TaskAgent(const char * name) :
    Component(name), workDir("/home/jcgonzalez/ws/docker-data/tools/")

{
    canProcessMessage(MSG_TASK_PROC_IDX);
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TaskAgent::fromRunningToOperational()
{
    stopTasks = false;
    numTasks = 0;
    numRunningTasks = 0;
    maxRunningTasks = 3;
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void TaskAgent::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
    flushLog();

    stopTasks = true;
    for (unsigned int i = 0; i < procTasks.size(); ++i) {
        procTasks.at(i)->join();
    }
}

//----------------------------------------------------------------------
// Method: processTASK_PROC
//----------------------------------------------------------------------
void TaskAgent::processTASK_PROC()
{
    // Check the product type as input for any rule
    Message_TASK_PROC * msg = dynamic_cast<Message_TASK_PROC *>(msgData.msg);
    std::string procElem = msg->task.taskPath;
    InfoMsg("Proc.element to be launched: " + procElem);

    std::thread * t = new std::thread(&TaskAgent::executeProcessingElement,
                                      this, msg->task);
    procTasks.push_back(t);
}

//----------------------------------------------------------------------
// Method: executeProcessingElement
// Executes a task
//----------------------------------------------------------------------
void TaskAgent::executeProcessingElement(TaskInfo t)
{
    std::string pe = t.taskPath;

    // Update number of tasks managed by this agent
    numTasks++;

    // Define task name and exchange dir.
    std::string internalTaskNameIdx = (selfPeer()->name + "-" +
                                       LibComm::timeTag() +
                                       "-" +
                                       LibComm::toStr<int>(numTasks));
    std::string exchangeDir = workDir + internalTaskNameIdx;

    //-------------------------------------------------------------------
    // 1. Send initial Task information (for storage)
    //-------------------------------------------------------------------
    TaskStatus status = TASK_WAITING;

    TaskInfo task;
    task.setData(t.getData());
    Json::Value & taskData = task.taskData;

    taskData["TaskAgent"] = self.load(std::memory_order_relaxed)->name;
    taskData["Name"] = "/UNDEFINED";
    taskData["NameExtended"] = (taskData["TaskAgent"].asString() +
                                ":/" + taskData["Name"].asString());
    taskData["State"]["Dead"] = false;
    taskData["State"]["Error"] = "";
    taskData["State"]["ExitCode"] = 0;
    taskData["State"]["FinishedAt"] = "0001-01-01T00:00:00Z";
    taskData["State"]["OOMKilled"] = false;
    taskData["State"]["Paused"] = true;
    taskData["State"]["Pid"] = 0;
    taskData["State"]["Progress"] = "0";
    taskData["State"]["Restarting"] = false;
    taskData["State"]["Running"] = false;
    taskData["State"]["StartedAt"] = "0001-01-01T00:00:00Z";
    task.taskEnd = "";
    task.taskExitCode = 0;
    task.taskName = internalTaskNameIdx;
    task.taskPath = "";
    task.taskStart = "";
    task.taskStatus = status;

    //-------------------------------------------------------------------
    // 2. Loop waiting for execution slot
    //-------------------------------------------------------------------
    if (numRunningTasks >= maxRunningTasks) {
        sendTaskResMsg(task);
        // Loop waiting until numTasks is below threshold
        while (numRunningTasks >= maxRunningTasks) {
            LibComm::waitForHeartBeat(0, 200000);
        }
    }

    //-------------------------------------------------------------------
    // 3. Execute processor
    //-------------------------------------------------------------------

    // Prepare folders
    std::string exchgIn     = exchangeDir + "/in";
    std::string exchgOut    = exchangeDir + "/out";

    std::string taskDriver  = workDir + std::string("runTask.sh");
    std::string cfgFile     = exchangeDir + std::string("/dummy.cfg");

    // Create exchange area
    mkdir(exchangeDir.c_str(), 0755);
    mkdir(exchgIn.c_str(), 0755);
    mkdir(exchgOut.c_str(), 0755);

    // Create input files
    int mn = 60;
    int mx = 180;
    std::ofstream oInFile1((exchgIn + "/1.in").c_str());
    if (oInFile1.good()) {
        oInFile1 << mn << " " << mx << std::endl;
        oInFile1.close();
    } else {
        WarnMsg("Cannot create " + (exchgIn + "/1.in"));
        return;
    }

    // Execute task driver
    pid_t pid = fork();

    if (pid < 0) {
        // Error, failed to fork()
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // We are the child
        char *procTaskCmdLine[] = { (char*)(taskDriver.c_str()),
                                    (char*)(pe.c_str()),
                                    (char*)(cfgFile.c_str()), NULL };
        std::string cmdLine =
                taskDriver + " " + pe + " " + cfgFile;
        InfoMsg("CMDLINE: " + cmdLine);
        execv(procTaskCmdLine[0], procTaskCmdLine);
        ErrMsg("EXECV: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    // Update number of running tasks
    numRunningTasks++;

    //-------------------------------------------------------------------
    // Start monitoring
    //-------------------------------------------------------------------

    // Get dockerId
    std::string dockerIdFile = exchangeDir + "/docker.id";
    std::string dockerId("");
    do {
        std::ifstream ifDockerId(dockerIdFile, std::ifstream::in);
        if (ifDockerId.good()) { ifDockerId >> dockerId; }
        ifDockerId.close();
    } while (dockerId.empty());

    // Monitor executing task
    bool childEnded = false;

    Json::Reader jsonReader;
    std::string inspectInfo;

    float progressValue = 0.;
    float monitStep = 5.; //seconds
    float estimatedMonitSteps = 180 / monitStep;
    float estimatedProgressStep = 100. / estimatedMonitSteps;
    int sleepPeriod = (int)(monitStep * 1000000.);

    bool running, dead, paused, killed, finished;
    int excode;

    do {

        // Sleep for some time
        std::this_thread::sleep_for(std::chrono::microseconds(sleepPeriod));

        // Get monitoring information from docker task
        std::string cmd = "docker inspect " + dockerId;
        inspectInfo = "";

        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) { return; }
        char buffer[128];
        while (!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL) { inspectInfo += buffer; }
        }
        pclose(pipe);

        // Parse information
        if (inspectInfo != "ERROR") {
            Json::Value td(Json::arrayValue);
            if  (jsonReader.parse(inspectInfo, td)) {
                taskData = td[0u];
            } else {
                childEnded = true;
            }
        } else {
            childEnded = true;
        }

        running  = taskData["State"]["Running"].asBool();
        dead     = taskData["State"]["Dead"].asBool();
        paused   = taskData["State"]["Paused"].asBool();
        killed   = taskData["State"]["OOMKilled"].asBool();
        excode   = taskData["State"]["ExitCode"].asInt();
        finished = (taskData["State"]["FinishedAt"].asString()
                    != "0001-01-01T00:00:00Z");        
        task.taskExitCode = excode;
        if (finished) {
            status = (excode == 0) ? TASK_FINISHED : TASK_FAILED;
        } else if (dead || killed) {
            status = TASK_FAILED;
        } else if (paused) {
            status = TASK_WAITING;
        } else {
            status = TASK_RUNNING;
            assert(running);
        }
        childEnded = (status == TASK_FAILED) || (status == TASK_FINISHED);

        // Additional info
        taskData["TaskAgent"] = selfPeer()->name;
        taskData["NameExtended"] = selfPeer()->name + ":/" + taskData["Name"].asString();

        // Update progress
        if (status != TASK_FINISHED) {
            progressValue += estimatedProgressStep;
            if (progressValue > 100.) { progressValue = 100.; }
            taskData["State"]["Progress"] = LibComm::toStr<int>(floor(progressValue));
        } else {
            taskData["State"]["Progress"] = "100";
        }

        // Incorporate taskData to task data structure
        task.taskStatus = status;

        // Send message
        sendTaskResMsg(task);

    } while ((!childEnded) && (!stopTasks));

    //-------------------------------------------------------------------
    // Report end of task
    //-------------------------------------------------------------------

    // Set end time
    std::string endTime = taskData["State"]["FinishedAt"].asString();
    endTime = LibComm::replaceAll(endTime, "-", "");
    endTime = LibComm::replaceAll(endTime, ":", "");
    endTime = LibComm::replaceAll(endTime, "Z", "");
    task.taskEnd = endTime;

    // Send message
    sendTaskResMsg(task);

    std::cerr << "After end: "
              << task.taskName << " - " << task.taskStatus << "\n";

    //-------------------------------------------------------------------
    // Task finished
    //-------------------------------------------------------------------
    numRunningTasks--;
}

//----------------------------------------------------------------------
// Method: sendTaskResMsg
// Send a TaskResultMsg to the Task Manager, with information on the
// processing task at hand
//----------------------------------------------------------------------
bool TaskAgent::sendTaskResMsg(TaskInfo & task)
{
    sendMsgsMutex.lock();
    Message_TASK_RES msg;
    buildMsgHeader(MSG_TASK_RES_IDX, selfPeer()->name, "TskMng", msg.header);
    msg.task.setData(task.getData());
    PeerMessage * taskResMsg = buildPeerMsg("TskMng", msg.getDataString(), MSG_TASK_RES);
    registerMsg(selfPeer()->name, *taskResMsg);
    setTransmissionToPeer("TskMng", taskResMsg);
    sendMsgsMutex.unlock();

    return true;
}

}
