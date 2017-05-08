/******************************************************************************
 * File:    taskagent.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskagent
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
#include "procelem.h"
#include "str.h"

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
    //setHeartBeatPeriod(3, 0);

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

    processingElements.clear();
    finishedProcessingElements.clear();
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
    ProcessingElement * p = new ProcessingElement(this);

    ++numTasks;
    ++numWaitingTasks;

    // Setpup and launch the PE
    p->setAgentName(selfPeer()->name);
    p->setSysDir(sysDir);
    p->setWorkDir(workDir);
    p->setTaskInfo(msg->task);
    p->setNumTask(numTasks);
    p->exec();

    // Store it at the PEs vector
    processingElements.push_back(p);

    // Send initial (registering) message with Task info
    sendTaskResMsg(p->getTask());
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
#ifdef DEBUG_BUILD
    std::stringstream ss;
    ss << "numTasks: " << numTasks << "  "
       << "numRunningTasks: " << numRunningTasks << "  "
       << "maxRunningTasks: " << maxRunningTasks << "  "
       << "numWaitingTasks: " << numWaitingTasks << "  ";
    ss << " R: ";
    for (unsigned int i = 0; (i < processingElements.size()); ++i) {
        ss << processingElements.at(i) << ' ';
    }
    ss << "   F: ";
    for (unsigned int i = 0; i < finishedProcessingElements.size(); ++i) {
        ss << finishedProcessingElements.at(i) << ' ';
    }
    DbgMsg(ss.str());
#endif

    // Check status of child processing elements
    checkProcessingElements();
}

//----------------------------------------------------------------------
// Method: checkProcessingElements
// Check list of processing elements
//----------------------------------------------------------------------
void TaskAgent::procElemFinished(ProcessingElement * p)
{
    InfoMsg("Ending proc.elem. " + selfPeer()->name + "-" +
            str::toStr<int>(p->getNumTask()));
    finishedProcessingElements.push_back(p);
}

//----------------------------------------------------------------------
// Method: checkProcessingElements
// Check list of processing elements
//----------------------------------------------------------------------
void TaskAgent::checkProcessingElements()
{
    // 1. Remove all processing elements that are marked as finished
    for (unsigned int i = 0; i < finishedProcessingElements.size(); ++i) {

        ProcessingElement * p = finishedProcessingElements[i];

        // Look for it in the processingElements list, and remove from it
        std::vector<ProcessingElement*>::iterator it =
                find(processingElements.begin(), processingElements.end(), p);

        if (it != processingElements.end()) {
            processingElements.erase(it);
            --numRunningTasks;
        }

        // Finaly, delete the ProcessingElement
        delete p;

    }

    // Clear the finished proc. elems. vector
    finishedProcessingElements.clear();

    // 2. Bring to life waiting proc.elements as soon as others are finished
    for (unsigned int i = 0; ((i < processingElements.size()) &&
                              (numRunningTasks < maxRunningTasks)); ++i) {

        ProcessingElement * p = processingElements[i];

        if (p->getStatus() == TASK_SCHEDULED) {
            // The number of running tasks was equal to or above the threshold
            // Check if this PE can be brought to life
            p->start();
            ++numRunningTasks;
            --numWaitingTasks;
            InfoMsg("Starting proc.elem. " + selfPeer()->name + "-" +
                    str::toStr<int>(p->getNumTask()));
        }

    }
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

inline std::string dbl2IntStr(double x) {
    return str::toStr<int>((int)(floor(x)));
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

    msg.variables.paramList["load1m"]     = str::toStr<double>(sysInfo.loadAvgs.at(0));
    msg.variables.paramList["load5m"]     = str::toStr<double>(sysInfo.loadAvgs.at(1));
    msg.variables.paramList["load15m"]    = str::toStr<double>(sysInfo.loadAvgs.at(2));

    msg.variables.paramList["uptime"]     = str::toStr<int>(sysInfo.upTime);

    msg.variables.paramList["totalMem"]   = dbl2IntStr(sysInfo.memStat.total);
    msg.variables.paramList["usedMem"]    = dbl2IntStr(sysInfo.memStat.values.at(0));
    msg.variables.paramList["buffersMem"] = dbl2IntStr(sysInfo.memStat.values.at(1));
    msg.variables.paramList["cachedMem"]  = dbl2IntStr(sysInfo.memStat.values.at(2));

    msg.variables.paramList["totalSwap"]  = dbl2IntStr(sysInfo.swapStat.total);
    msg.variables.paramList["usedSwap"]   = dbl2IntStr(sysInfo.swapStat.values.at(0));

    for (int i = 1; i <= sysInfo.cpuCount; ++i) {
        std::string cpu = "cpu" + str::toStr<int>(i);
        msg.variables.paramList[cpu] = str::toStr<double>(sysInfo.cpuPercent.at(i));
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
