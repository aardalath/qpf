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
    Component(name), workDir("")
{
    setHeartBeatPeriod(0, 500000);

    canProcessMessage(MSG_TASK_PROC_IDX);
    canProcessMessage(MSG_CMD_IDX);
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
    numWaitingTasks = 0;
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void TaskAgent::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
    flushLog();

    stopTasks = true;
}

//----------------------------------------------------------------------
// Method: processTASK_PROC
//----------------------------------------------------------------------
void TaskAgent::processTASK_PROC()
{
    // Check the product type as input for any rule
    Message_TASK_PROC * msg = dynamic_cast<Message_TASK_PROC *>(msgData.msg);
    InfoMsg("Processor to be launched: " + msg->task.taskPath);

    // Create new processing element with the task information
    ProcessingElement * pe = new ProcessingElement();

    ++numTasks;
    ++numWaitingTasks;

    pe->setAgentName(selfPeer()->name);
    pe->setSysDir(sysDir);
    pe->setWorkDir(workDir);
    pe->setTaskInfo(msg->task);
    pe->setNumTask(numTasks);

    // Store it at the PEs vector
    processingElements.push_back(pe);

    // Launch the PE
    pe->exec();

    // Send first TASK_RES message
    sendTaskResMsg(pe->getTask());
}

//----------------------------------------------------------------------
// Method: processCMD
//----------------------------------------------------------------------
void TaskAgent::processCMD()
{

}

//----------------------------------------------------------------------
// Method: execAdditonalLoopTasks
//----------------------------------------------------------------------
void TaskAgent::execAdditonalLoopTasks()
{
    static int cycle = 0;

    cycle = (cycle + 1) % 3;

    // Check status of child processing elements
    checkProcessingElements();

    // Send host-related monitoring information
    if (cycle == 0) {
        if (!sendMonitInfo()) {
            WarnMsg("Couldn't send host monitoring information");
        }
    }
}

//----------------------------------------------------------------------
// Method: checkProcessingElements
// Check list of processing elements
//----------------------------------------------------------------------
void TaskAgent::checkProcessingElements()
{
    static unsigned int numOfProcElems = 0;
    static unsigned int idxProcElemToCheck = 0;

    // Check num of processing elements in list
    numOfProcElems = processingElements.size();
    if (numOfProcElems < 1) return;

    // Check idx of proc elem to be checked
    if (idxProcElemToCheck >= numOfProcElems) return;

    // Start checking process
    unsigned int & i = idxProcElemToCheck;

    bool sendMsg        = false;
    bool markAsArchived = false;

    ProcessingElement * pe = processingElements.at(i);

    TaskStatus status = pe->getStatus();

    switch (status) {

    case TASK_ARCHIVED:
        // Task already finished, and final information was sent to be
        // registered in the DB, so the pe can be deleted
        InfoMsg("Removing proc.elem. for task " + pe->getTask().taskName);

        delete pe;
        pe = 0;
        processingElements.erase(processingElements.begin() + i);
        numOfProcElems = processingElements.size();
        sendMsg = false;
        break;

    case TASK_FINISHED:
        // Send latest information for completed task
        // And decrease the number of tasks running
        --numRunningTasks;
        markAsArchived = true;
        sendMsg = true;
        break;

    case TASK_SCHEDULED:
        // The number of running tasks was equal to or above the threshold
        // Check if this PE can be brought to life
        if (numRunningTasks < maxRunningTasks) {
            pe->start();
            ++numRunningTasks;
            --numWaitingTasks;
            InfoMsg("Starting proc.elem. " + selfPeer()->name +
                    "-" + LibComm::toStr<int>(i + 1));
        }
        // It could be that the task is just starting, se we do not
        // send message about stated task, but wait until next iteration
        sendMsg = false;
        break;

    case TASK_FAILED:
        // Send latest information for task
        // ... and set processing element status to ARCHIVED
        markAsArchived = true;
        sendMsg = true;
        break;

    case TASK_PAUSED:
        // Task is idle, do nothing, just report its state

        break;

    case TASK_STOPPED:
        // Task has been explicitly stopped
        // Send latest information for task
        // ... and set processing element status to ARCHIVED
        markAsArchived = true;
        sendMsg = true;
        break;

    case TASK_RUNNING:
        // Update information about the processing element
        // and send it to the managing components
        sendMsg = true;
        break;

    default:
        break;
    }

    if (sendMsg) {
        sendTaskResMsg(pe->getTask());
    }

    if (markAsArchived) {
        pe->markAsArchived();
    }

    // Update idx of proc. elem. for next iteration
    idxProcElemToCheck++;
    if (idxProcElemToCheck >= numOfProcElems) { idxProcElemToCheck = 0; }
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
    //registerMsg(selfPeer()->name, *taskResMsg);
    setTransmissionToPeer("TskMng", taskResMsg);
    sendMsgsMutex.unlock();

    return true;
}

inline std::string dbl2IntStr(double x) {
    return LibComm::toStr<int>((int)(floor(x)));
}

//----------------------------------------------------------------------
// Method: sendMonitInfo
// Send a MonitInfoMsg to the Task Manager, with information on
// processing node status (load avgs., uptime, etc.)
//----------------------------------------------------------------------
bool TaskAgent::sendMonitInfo()
{
    static LibComm::SysInfo sysInfo {};

    if (!sysInfo.update()) { return false; }
    sysInfo.computeStats();

    Message_MONIT_INFO msg;
    buildMsgHeader(MSG_MONIT_INFO_IDX, selfPeer()->name, "TskMng", msg.header);

    msg.variables.paramList["load1m"]     = LibComm::toStr<double>(sysInfo.loadAvgs.at(0));
    msg.variables.paramList["load5m"]     = LibComm::toStr<double>(sysInfo.loadAvgs.at(1));
    msg.variables.paramList["load15m"]    = LibComm::toStr<double>(sysInfo.loadAvgs.at(2));

    msg.variables.paramList["uptime"]     = LibComm::toStr<int>(sysInfo.upTime);

    msg.variables.paramList["totalMem"]   = dbl2IntStr(sysInfo.memStat.total);
    msg.variables.paramList["usedMem"]    = dbl2IntStr(sysInfo.memStat.values.at(0));
    msg.variables.paramList["buffersMem"] = dbl2IntStr(sysInfo.memStat.values.at(1));
    msg.variables.paramList["cachedMem"]  = dbl2IntStr(sysInfo.memStat.values.at(2));

    msg.variables.paramList["totalSwap"]  = dbl2IntStr(sysInfo.swapStat.total);
    msg.variables.paramList["usedSwap"]   = dbl2IntStr(sysInfo.swapStat.values.at(0));

    for (int i = 1; i <= sysInfo.cpuCount; ++i) {
        std::string cpu = "cpu" + LibComm::toStr<int>(i);
        msg.variables.paramList[cpu] = LibComm::toStr<double>(sysInfo.cpuPercent.at(i));
    }
/*
    std::cerr << "== Monit. Info. - " << selfPeer()->name << " ======================" << std::endl;
    for (auto & kv : msg.variables.paramList) {
        std::cerr << kv.first << ": " << kv.second << std::endl;
    }
    std::cerr << std::endl;
*/
    PeerMessage * monitInfoMsg = buildPeerMsg("TskMng", msg.getDataString(), MSG_MONIT_INFO);
    // MonitInfo messages are not registered
    //registerMsg(selfPeer()->name, *monitInfoMsg);
    setTransmissionToPeer("TskMng", monitInfoMsg);

    return true;
}

}
