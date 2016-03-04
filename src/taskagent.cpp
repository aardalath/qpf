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
    Component(name)
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
void TaskAgent::executeProcessingElement(TaskInfo task)
{
    std::string workDir("/home/jcgonzalez/ws/docker-data/tools");

    std::string pe = task.taskPath;

    // Update number of tasks managed by this agent
    numTasks++;
    std::string internalTaskNameIdx = (selfPeer()->name + "-" +
                                       LibComm::timeTag() +
                                       "-" +
                                       LibComm::toStr<int>(numTasks));

    // Send initial Task information (for storage)
    if (numRunningTasks >= maxRunningTasks) {
        task.taskStatus = TASK_PAUSED;
        task.taskData["NameExtended"] = selfPeer()->name + ":/undefined";
        task.taskData["State"]["Paused"] = true;
        sendTaskResMsg(task);

        // Loop waiting until numTasks is below threshold
        while (numRunningTasks >= maxRunningTasks) {
            LibComm::waitForHeartBeat(0, 200000);
        }
    }

    // Update number of running tasks
    numRunningTasks++;

    //-------------------------------------------------------------------
    // Execute processor
    //-------------------------------------------------------------------
    workDir += "/";
    std::string taskDriver  = workDir + std::string("runTask.sh");
    std::string exchangeDir = workDir + internalTaskNameIdx;
    std::string exchgIn     = exchangeDir + "/in";
    std::string exchgOut    = exchangeDir + "/out";
    std::string cfgFile     = exchangeDir + std::string("/dummy.cfg");

    // Create exchange area
    mkdir(exchangeDir.c_str(), 0755);
    mkdir(exchgIn.c_str(), 0755);
    mkdir(exchgOut.c_str(), 0755);

    // Create input files
    int mn = 300;
    int mx = 600;
    std::ofstream oInFile1((exchgIn + "/1.in").c_str());
    if (oInFile1.good()) {
        oInFile1 << mn << " " << mx << std::endl;
        oInFile1.close();
    } else {
        WarnMsg("Cannot create " + (exchgIn + "/1.in"));
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        // Error, failed to fork()
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // We are the child
        char * procTaskCmdLine[] = { (char*)(taskDriver.c_str()),
                                     (char*)(pe.c_str()),
                                     (char*)(cfgFile.c_str()), NULL };
        std::string cmdLine =
                taskDriver + " " + pe + " " + cfgFile;
        InfoMsg("CMDLINE: " + cmdLine);
        execv(procTaskCmdLine[0], procTaskCmdLine);
        ErrMsg("EXECV: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    // We are still the parent...

    //-------------------------------------------------------------------
    // Start monitoring
    //-------------------------------------------------------------------
    bool childEnded = false;

    // Get dockerId
    std::string dockerIdFile = exchangeDir + "/docker.id";
    std::string dockerId("");
    do {
        std::ifstream ifDockerId(dockerIdFile, std::ifstream::in);
        if (ifDockerId.good()) { ifDockerId >> dockerId; }
        ifDockerId.close();
    } while (dockerId.empty());

    Json::Value taskData(Json::arrayValue);
    Json::Reader jsonReader;
    Json::StyledWriter writer;
    std::string inspectInfo;

    float progressValue = 0.;
    std::string progress;
    float monitStep = 5.; //seconds
    float estimatedMonitSteps = mx / monitStep;
    float estimatedProgressStep = 100. / estimatedMonitSteps;
    int sleepPeriod = (int)(monitStep * 1000000.);

    do {

        // Sleep for some time
        std::this_thread::sleep_for(std::chrono::microseconds(sleepPeriod));

        // Get monitoring information from docker task
        inspectInfo = inspectDockerRunningTask(dockerId);

        // Parse information
        if  (! jsonReader.parse(inspectInfo, taskData)) {
            taskData[0] = task.taskData;
            //taskData.clear();
            //taskData[0]["State"]["ExitCode"] = 0;
            childEnded = true;
            break;
        }

        // Update progress
        progressValue += estimatedProgressStep;
        if (progressValue > 100.) { progressValue = 100.; }
        progress = LibComm::toStr<int>(floor(progressValue));

        childEnded =  ! taskData[0]["State"]["Running"].asBool();

        // Additional info
        taskData[0]["State"]["Progress"] = progress;
        taskData[0]["TaskAgent"] = selfPeer()->name;
        taskData[0]["NameExtended"] = selfPeer()->name + ":/" + taskData[0]["Name"].asString();

        task.taskStatus = TASK_RUNNING;

        // Incorporate taskData to task data structure
        task.setData("taskData", taskData[0]);

        // Send message
        sendTaskResMsg(task);

    } while ((!childEnded) && (!stopTasks));

    if (childEnded) { //&& (taskData[0]["State"]["ExitCode"].asInt() == 0)) {
        // Set final parameters
        taskData[0]["State"]["Progress"] = "100";
        taskData[0]["TaskAgent"] = selfPeer()->name;
        taskData[0]["NameExtended"] = selfPeer()->name + ":/" + taskData[0]["Name"].asString();

        // Set actual exit code
        task.taskExitCode = taskData[0]["State"]["ExitCode"].asInt();
        task.taskStatus   = (task.taskExitCode == 0) ? TASK_FINISHED : TASK_FAILED;

        // Set end time
        std::string endTime = taskData[0]["State"]["FinishedAt"].asString();
        endTime = LibComm::replaceAll(endTime, "-", "");
        endTime = LibComm::replaceAll(endTime, ":", "");
        endTime = LibComm::replaceAll(endTime, "Z", "");
        task.taskEnd = endTime;

        // Incorporate taskData to task data structure
        task.setData("taskData", taskData[0]);

        // Send message
        sendTaskResMsg(task);
    }

    // Task finished
    numRunningTasks--;
}

//----------------------------------------------------------------------
// Method: sendTaskResMsg
// Send a TaskResultMsg to the Task Manager, with information on the
// processing task at hand
//----------------------------------------------------------------------
bool TaskAgent::sendTaskResMsg(TaskInfo & task)
{
    Message_TASK_RES msg;
    buildMsgHeader(MSG_TASK_RES_IDX, selfPeer()->name, "TskMng", msg.header);
    msg.task.setData(task.getData());
    //std::cerr << "TASKRES: " << msg.getDataString() << std::endl;
    PeerMessage * taskResMsg = new PeerMessage;
    buildPeerMsg(*taskResMsg, "TskMng", msg.getDataString(), MSG_TASK_RES);
    registerMsg(selfPeer()->name, *taskResMsg);
    setTransmissionToPeer("TskMng", taskResMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: inspectDockerRunningTask
// Send a TaskResultMsg to the Task Manager, with information on the
// processing task at hand
//----------------------------------------------------------------------
std::string TaskAgent::inspectDockerRunningTask(std::string id)
{
    std::string cmd = "docker inspect " + id;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL) { result += buffer; }
    }
    pclose(pipe);
    return result;
}

}
