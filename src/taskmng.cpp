/******************************************************************************
 * File:    taskmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskmng
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
 *   Implement TaskManager class
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

#include "taskmng.h"
#include "str.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>
#include <array>

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
TaskManager::TaskManager(const char * name) :
    Component(name)
{
    canProcessMessage(MSG_TASK_PROC_IDX);
    canProcessMessage(MSG_TASK_RES_IDX); // TODO: Deprecate this channel for EvtMng in favour of DB
    canProcessMessage(MSG_MONIT_INFO_IDX);
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TaskManager::fromRunningToOperational()
{
    // Create Agents table
    AgentInfo emptyInfo;
    for (unsigned int i = 0; i < peers.size(); ++i) {
        Peer * p = peers.at(i);
        if (p->type == "taskagent") {
            agents.push_back(p);
            emptyInfo.idx = i;
            emptyInfo.runningTasks = 0;
            emptyInfo.failedTasks = 0;
            emptyInfo.finishedTasks = 0;
            emptyInfo.launchedTasks = 0;
            emptyInfo.load = 0;
            agentInfo[p] = emptyInfo;
        }
    }
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void TaskManager::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: processTASK_PROC
//----------------------------------------------------------------------
void TaskManager::processTASK_PROC()
{
    // Check the product type as input for any rule
    Message_TASK_PROC * msg = dynamic_cast<Message_TASK_PROC *>(msgData.msg);
    std::string & ruleDesc = msg->rule;
    InfoMsg("Requested Processing of Rule:\n" + ruleDesc);

    // Execute rule requested
    exeRule(msg);
}

//----------------------------------------------------------------------
// Method: processTASK_RES
//----------------------------------------------------------------------
void TaskManager::processTASK_RES()
{
    // Forward message to TskOrc
    Message_TASK_RES * msg = dynamic_cast<Message_TASK_RES *>(msgData.msg);
    sendTaskRes(msg);
}

//----------------------------------------------------------------------
// Method: processMONIT_INFO
//----------------------------------------------------------------------
void TaskManager::processMONIT_INFO()
{
    Message_MONIT_INFO * msg = dynamic_cast<Message_MONIT_INFO *>(msgData.msg);

    // Save information for TaskAgent selection
    std::string & senderName = msg->header.source;
    double load = str::strTo<double>(msg->variables.paramList["load1m"]);
    for (LibComm::Router2RouterPeer::Peer * p : agents) {
        if (p->name == senderName) {
            AgentInfo & ag = agentInfo[p];
            ag.load = load;
//            std::cerr << p->name << " info updated to : "
//                      << ag.runningTasks << " tasks, load = " << ag.load << "\n";
        }
    }

    // ... and forward message to EvtMsg
    sendMonitInfo(msg);
}

//----------------------------------------------------------------------
// Method: exeRule
// Execute the rule requested by Task Orchestrator
//----------------------------------------------------------------------
void TaskManager::exeRule(Message_TASK_PROC * msg)
{
    std::string peName = msg->task.taskPath;
    InfoMsg("Proc.Elem. in rule: " + peName);

    // Select TaskAgent to use
    LibComm::Router2RouterPeer::Peer * agent = selectAgent();
    InfoMsg("Selected Agent is " + agent->name);

    // Send message to agent
    if (sendTaskAgMsg(msg, agent->name)) {
        agentInfo[agent].launchedTasks++;
        agentInfo[agent].runningTasks++;
    }

}

inline double weightFunc(double load, double tasks) { return 100 * load + tasks; }

//----------------------------------------------------------------------
// Method: selectAgent
// Send a TaskProcessingMsg to the Task Manager, requesting the
// execution of a rule
//----------------------------------------------------------------------
LibComm::Router2RouterPeer::Peer * TaskManager::selectAgent()
{
/*
    // Select agent with smaller number of running tasks
    int nRunTasks = -1;
    int agIdx;
    for (unsigned int i = 0; i < agents.size(); ++i) {
        Peer * p = agents.at(i);
        AgentInfo & agInfo = agentInfo[p];
        if ((nRunTasks < 0) || (nRunTasks > agInfo.runningTasks)) {
            nRunTasks = agInfo.runningTasks;
            agIdx = i;
        }
    }
*/

    // Select agent with lower weight (try to balance load)
    double weight = -1;
    double newW;
    int agIdx = 0;
    for (unsigned int i = 0; i < agents.size(); ++i) {
        Peer * p = agents.at(i);
        AgentInfo & agInfo = agentInfo[p];
        newW = weightFunc(agInfo.load, agInfo.runningTasks);
        if ((weight < 0) || (weight > newW)) {
            weight = newW;
            agIdx = i;
        }
    }
    // Return agent peer
    return agents.at(agIdx);
}

//----------------------------------------------------------------------
// Method: sendTaskAgMsg
// Send a TaskProcessingMsg to the Task Manager, requesting the
// execution of a rule
//----------------------------------------------------------------------
bool TaskManager::sendTaskAgMsg(Message_TASK_PROC * msg,
                                std::string agName)
{
    MessageData msgDataToAg(new Message_TASK_PROC);
    msgDataToAg.msg->setData(msg->getData());
    setForwardTo(agName, msgDataToAg.msg->header);
    PeerMessage * msgForAg = buildPeerMsg(msgDataToAg.msg->header.destination,
                                          msgDataToAg.msg->getDataString(),
                                          MSG_TASK_PROC);
    registerMsg(selfPeer()->name, *msgForAg);
    setTransmissionToPeer(agName, msgForAg);

    return true;
}

//----------------------------------------------------------------------
// Method: sendTaskRes
// Send a TaskResMsg to the Event Manager
//----------------------------------------------------------------------
bool TaskManager::sendTaskRes(Message_TASK_RES * msg)
{
    // Send TASK_RES to all the recipients
    // TODO: Deprecate this channel for EvtMng in favour of DB
    std::array<std::string,2> fwdRecip = {"DataMng", "EvtMng"};
    for (std::string & recip : fwdRecip) {
        MessageData msgToRecip(new Message_TASK_RES);
        msgToRecip.msg->setData(msg->getData());
        setForwardTo(recip, msgToRecip.msg->header);
        PeerMessage * msgForRecip = buildPeerMsg(msgToRecip.msg->header.destination,
                                                 msgToRecip.msg->getDataString(),
                                                 MSG_TASK_RES);
        registerMsg(selfPeer()->name, *msgForRecip);
        setTransmissionToPeer(recip, msgForRecip);
    }
    return true;
}

//----------------------------------------------------------------------
// Method: sendMonitInfo
// Send a TaskResMsg to the Event Manager
//----------------------------------------------------------------------
bool TaskManager::sendMonitInfo(Message_MONIT_INFO * msg)
{
    // Send TASK_RES to all the recipients
    // TODO: Deprecate this channel for EvtMng in favour of DB
    std::array<std::string,1> fwdRecip = {"EvtMng"};
    for (std::string & recip : fwdRecip) {
        MessageData msgToRecip(new Message_MONIT_INFO);
        msgToRecip.msg->setData(msg->getData());
        setForwardTo(recip, msgToRecip.msg->header);
        PeerMessage * msgForRecip = buildPeerMsg(msgToRecip.msg->header.destination,
                                                 msgToRecip.msg->getDataString(),
                                                 MSG_MONIT_INFO);
        //registerMsg(selfPeer()->name, *msgForRecip);
        setTransmissionToPeer(recip, msgForRecip);
    }
    return true;
}

}
