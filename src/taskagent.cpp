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
void TaskAgent::executeProcessingElement(TaskInfo task)
{
    std::string pe = task.taskPath;

    // Update number of tasks managed by this agent
    numTasks++;


    // Define task name and exchange dir.
    std::string internalTaskNameIdx = (selfPeer()->name + "-" +
                                       LibComm::timeTag() +
                                       "-" +
                                       LibComm::toStr<int>(numTasks));
    std::string exchangeDir = workDir + internalTaskNameIdx;

    // Send initial Task information (for storage)
    createNewTaskInfo(task);
    if (numRunningTasks >= maxRunningTasks) {
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
    if (runProcessor(exchangeDir, pe)) {

        //-------------------------------------------------------------------
        // Start monitoring
        //-------------------------------------------------------------------

        // Get dockerId
        std::string dockerId = getDockerId(exchangeDir);

        // Define task data handler to be obtained from Docker
        Json::Value taskData(Json::arrayValue);

        // Loop monitoring
        if (monitorDockerTask(dockerId, task, taskData)) {
            reportEndOfTask(task, taskData);
        }

    } else {
        WarnMsg("Couldn't execute processing element for task " + internalTaskNameIdx);
    }

    //-------------------------------------------------------------------
    // Task finished
    //-------------------------------------------------------------------
    numRunningTasks--;
}

//----------------------------------------------------------------------
// Method: runProcessor
// Prepares a task processor environment, and launches it
//----------------------------------------------------------------------
bool TaskAgent::runProcessor(std::string exchangeDir, std::string pe)
{
    bool retVal = false;

    std::string taskDriver;
    std::string cfgFile;

    if (prepareFolders(exchangeDir, taskDriver, cfgFile)) {
        executeTaskDriver(pe, taskDriver, cfgFile);
        retVal = true;
    }

    return retVal;
}

//----------------------------------------------------------------------
// Method: executeProcessingElement
// repares a task processor environment and folders
//----------------------------------------------------------------------
bool TaskAgent::prepareFolders(std::string exchangeDir,
                               std::string & taskDriver, std::string & cfgFile)
{
    bool retVal = true;

    std::string exchgIn     = exchangeDir + "/in";
    std::string exchgOut    = exchangeDir + "/out";

    taskDriver  = workDir + std::string("runTask.sh");
    cfgFile     = exchangeDir + std::string("/dummy.cfg");

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
        retVal = false;
    }

    return retVal;
}

//----------------------------------------------------------------------
// Method: executeTaskDriver
// Executes a task's taskDriver
//----------------------------------------------------------------------
void TaskAgent::executeTaskDriver(std::string pe, std::string taskDriver, std::string cfgFile)
{
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
}

//----------------------------------------------------------------------
// Method: getDockerId
// Returns the docker id of a docker task
//----------------------------------------------------------------------
std::string TaskAgent::getDockerId(std::string exchangeDir)
{
    std::string dockerIdFile = exchangeDir + "/docker.id";
    std::string dockerId("");
    do {
        std::ifstream ifDockerId(dockerIdFile, std::ifstream::in);
        if (ifDockerId.good()) { ifDockerId >> dockerId; }
        ifDockerId.close();
    } while (dockerId.empty());

    return dockerId;
}

//----------------------------------------------------------------------
// Method: monitorDockerTask
// Continuously monitor Docker executing task
//----------------------------------------------------------------------
bool TaskAgent::monitorDockerTask(std::string & dockerId,
                                  TaskInfo & task, Json::Value & taskData)
{
    bool childEnded = false;

    Json::Reader jsonReader;
    std::string inspectInfo;

    float progressValue = 0.;
    std::string progress;
    float monitStep = 5.; //seconds
    float estimatedMonitSteps = 180 / monitStep;
    float estimatedProgressStep = 100. / estimatedMonitSteps;
    int sleepPeriod = (int)(monitStep * 1000000.);

    do {

        // Sleep for some time
        std::this_thread::sleep_for(std::chrono::microseconds(sleepPeriod));

        // Get monitoring information from docker task
        inspectDockerRunningTask(dockerId, inspectInfo);

        // Parse information
        if (inspectInfo == "ERROR") {
            childEnded = true;
            if  (! jsonReader.parse(inspectInfo, taskData)) {
                taskData[0] = task.taskData;
                childEnded = true;
                break;
            } else {
                childEnded =  ! taskData[0]["State"]["Running"].asBool();
            }
        }

        // Additional info
        taskData[0]["TaskAgent"] = selfPeer()->name;
        taskData[0]["NameExtended"] = selfPeer()->name + ":/" + taskData[0]["Name"].asString();

        // Update progress
        if (!childEnded) {
            progressValue += estimatedProgressStep;
            if (progressValue > 100.) { progressValue = 100.; }
            progress = LibComm::toStr<int>(floor(progressValue));
            taskData[0]["State"]["Progress"] = progress;
            task.taskStatus = TASK_RUNNING;
        }

        // Incorporate taskData to task data structure
        task.setData("taskData", taskData[0]);

        // Send message
        sendTaskResMsg(task);

    } while ((!childEnded) && (!stopTasks));

    return childEnded;
}

//----------------------------------------------------------------------
// Method: reportEndOfTask
// Send final TASK_RES message after a Docker task is finished
//----------------------------------------------------------------------
void TaskAgent::reportEndOfTask(TaskInfo & task, Json::Value & taskData)
{
    taskData[0]["TaskAgent"] = selfPeer()->name;
    taskData[0]["NameExtended"] = selfPeer()->name + ":/" + taskData[0]["Name"].asString();

    // Set actual exit code
    task.taskExitCode = taskData[0]["State"]["ExitCode"].asInt();

    if (task.taskExitCode == 0) {
        // Set final parameters
        taskData[0]["State"]["Progress"] = "100";
        task.taskStatus = TASK_FINISHED;
    } else {
        task.taskStatus = TASK_FAILED;
    }

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
    PeerMessage * taskResMsg = buildPeerMsg("TskMng", msg.getDataString(), MSG_TASK_RES);
    registerMsg(selfPeer()->name, *taskResMsg);
    setTransmissionToPeer("TskMng", taskResMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: inspectDockerRunningTask
// Send a TaskResultMsg to the Task Manager, with information on the
// processing task at hand
//----------------------------------------------------------------------
void TaskAgent::inspectDockerRunningTask(std::string id, std::string & result)
{
    static int idx = 0;
    std::string cmd = "docker inspect " + id;
    result = "";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) { return; }
    char buffer[128];
    while (!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL) { result += buffer; }
    }
    pclose(pipe);

    std::string msgFileName("/tmp/");
    char baseName[100];
    sprintf(baseName, "d.%04d.json", ++idx);
    msgFileName += baseName;
    std::ofstream msgOut(msgFileName);
    msgOut << result << "\n";
    msgOut.close();
}

//----------------------------------------------------------------------
// Method: createNewTaskInfo
// Add basic information about processing to task
//----------------------------------------------------------------------
void TaskAgent::createNewTaskInfo(TaskInfo & task)
{
    task.taskData["TaskAgent"] = self.load(std::memory_order_relaxed)->name;
    task.taskData["Name"] = "/UNDEFINED";
    task.taskData["NameExtended"] = (task.taskData["TaskAgent"].asString() +
                                     ":/" + task.taskData["Name"].asString());
    task.taskData["State"]["Dead"] = false;
    task.taskData["State"]["Error"] = "";
    task.taskData["State"]["ExitCode"] = 0;
    task.taskData["State"]["FinishedAt"] = "0001-01-01T00:00:00Z";
    task.taskData["State"]["OOMKilled"] = false;
    task.taskData["State"]["Paused"] = true;
    task.taskData["State"]["Pid"] = 0;
    task.taskData["State"]["Progress"] = "0";
    task.taskData["State"]["Restarting"] = false;
    task.taskData["State"]["Running"] = false;
    task.taskData["State"]["StartedAt"] = "0001-01-01T00:00:00Z";
    task.taskEnd = "";
    task.taskExitCode = 0;
    task.taskName = "task-rule3_20150810T181527";
    task.taskPath = "PE_NISP_S";
    task.taskStart = "";
    task.taskStatus = TASK_WAITING;
    task.toData();
}

}
