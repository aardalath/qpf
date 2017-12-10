/******************************************************************************
 * File:    taskmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskmng
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement TskMng class
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

#include "tskmng.h"

#include <sys/time.h>
#include <array>
#include <memory>

#include "channels.h"
#include "str.h"
#include "log.h"
#include "tools.h"
#include "config.h"
#include "timer.h"

using Configuration::cfg;

const int FMK_INFO_TIMER = 5000;
const int TSK_REP_TIMER  = 3000;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TskMng::TskMng(const char * name, const char * addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TskMng::TskMng(std::string name, std::string addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TskMng::fromRunningToOperational()
{
    // Create Agents Info. table
    AgentInfo emptyInfo;
    for (auto & a : cfg.agentNames) {
        agents.push_back(a);
        emptyInfo.name = a;
        emptyInfo.load = 0.0;
        agentInfo[a] = emptyInfo;
    }

    // Initialize Task Status maps
    for (int k = TASK_SCHEDULED; k != TASK_UNKNOWN_STATE; ++k) {
        TaskStatus status = TaskStatus(k);
        serviceTaskStatus[status]   = 0;
        containerTaskStatus[status] = 0;
        for (auto & a : cfg.agentNames) {
            TaskStatusPerAgent tskPair = std::make_pair(a, status);
            containerTaskStatusPerAgent[tskPair] = 0;
        }
    }

    // Launch the HTTP Server for Docker Swarm Services, and to
    // serve information through HTML pages. We activate the service to
    // provide data files only if we do really need it (swarm services)
    httpSrv = new HttpServer("httpSrv",
                             "localhost:8080", //"cfg.network.httpServerAddress()",
                             cfg.network.swarms().size() > 0);
    httpSrv->start();

    sendingPeriodicFmkInfo = false;
    sendingTskRegInfo      = false;

    // Transit to Operational
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: runEachIteration
//----------------------------------------------------------------------
void TskMng::runEachIteration()
{
    static std::string lastTrace;

    // Send pending messages just in case there are any in the buffer

    std::string trace = 
        ("SWARM: Q(" + std::to_string(serviceTasks.size()) + ")" +
         "R(" + std::to_string(serviceTaskStatus[TASK_RUNNING]) + ")" +
         "W(" + std::to_string(serviceTaskStatus[TASK_SCHEDULED]) + ")" +
         "P(" + std::to_string(serviceTaskStatus[TASK_PAUSED]) + ")" +
         "S(" + std::to_string(serviceTaskStatus[TASK_STOPPED]) + ")" +
         "E(" + std::to_string(serviceTaskStatus[TASK_FAILED]) + ")" +
         "F(" + std::to_string(serviceTaskStatus[TASK_FINISHED]) + ")" +
         "   CONT.: Q(" + std::to_string(containerTasks.size()) + ")" +
         "R(" + std::to_string(containerTaskStatus[TASK_RUNNING]) + ")" +
         "W(" + std::to_string(containerTaskStatus[TASK_SCHEDULED]) + ")" +
         "P(" + std::to_string(containerTaskStatus[TASK_PAUSED]) + ")" +
         "S(" + std::to_string(containerTaskStatus[TASK_STOPPED]) + ")" +
         "E(" + std::to_string(containerTaskStatus[TASK_FAILED]) + ")" +
         "F(" + std::to_string(containerTaskStatus[TASK_FINISHED]) + ")"
         );
    if (trace != lastTrace) {
        TRC(trace);
        lastTrace = trace;
    }
}

//----------------------------------------------------------------------
// Method: processTskRqstMsg
//----------------------------------------------------------------------
void TskMng::processTskRqstMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    // Define and set task object
    Message<MsgBodyTSK> msg(m);
    std::string agName(msg.header.source());
    DBG("TASK REQUEST FROM " << agName << " RECEIVED");
    
    // Task Requests ONLY should come from container agents (not from swarm agents)
    bool isSrvRqst = (agName.find("Swarm") != std::string::npos);
    if (isSrvRqst) {
        // Raise alert (TODO)
        RaiseSysAlert(Alert(Alert::System,
                            Alert::Warning,
                            Alert::Comms,
                            std::string(__FILE__ ":" Stringify(__LINE__)),
                            "Task Request received as coming from Swarm "
                            + agName,
                            0));
        return;
    }

    // Check that no previous message was sent (and the Agent was not
    // aware of if).  In that case, resend it.
    std::map<std::string, MessageString>::iterator it = containerTaskLastMessage.find(agName);
    if (it != containerTaskLastMessage.end()) {
        // There is a message send to this agent.  A new request by
        // this agent without a removal of this last message from the
        // map containerTaskLastMessage means that the message was not
        // noticed by the agent.  Therefore, we resend the same
        // message.
        MessageString & msgStr = it->second;
        send(ChnlTskProc + "_" + agName, msgStr);
        DBG("Task message resent to " + agName);
        return;
    }

    
    // Select task to send
    TraceMsg("Pool of tasks has size of " + std::to_string(containerTasks.size()));
    if (containerTasks.size() < 1) { return; }        

    json taskInfoData = containerTasks.front().val();
    containerTasks.pop_front();
    
    std::string taskName = agName + "_" + taskInfoData["taskName"].asString();
    taskInfoData["taskName"] = taskName;
    
    // Create message
    msg.buildHdr(ChnlTskProc, MsgTskProc, CHNLS_IF_VERSION,
                 compName, agName, "", "", "");
    MsgBodyTSK body;
    
    body["info"] = taskInfoData;
    msg.buildBody(body);

    MessageString msgStr(msg.str());
    send(ChnlTskProc + "_" + agName, msgStr);

    containerTaskLastMessage[agName] = msgStr;
    
    taskRegistry[taskName] = TASK_SCHEDULED;
    containerTaskStatus[TASK_SCHEDULED]++;
    containerTaskStatusPerAgent[std::make_pair(agName, TASK_SCHEDULED)]++;
    
    DBG("Task " + taskName + "sent to " + agName);
}

//----------------------------------------------------------------------
// Method: processTskRepMsg
//----------------------------------------------------------------------
void TskMng::processTskRepMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    Message<MsgBodyTSK> msg(m);
    MsgBodyTSK & body = msg.body;
    TaskInfo task(body["info"]);

    std::string taskName  = task.taskName();
    TaskStatus oldStatus  = taskRegistry[taskName];

    if (oldStatus == TASK_FINISHED) { return; }
    
    std::string agName    = task.taskAgent();
    TaskStatus taskStatus = TaskStatus(task.taskStatus());

    TraceMsg("Processing TaskReport: status: " + TaskStatusName[oldStatus] +
             " ==> " + TaskStatusName[taskStatus]);

    // Update registry and status maps if needed
    if (oldStatus != taskStatus) {
        taskRegistry[taskName] = taskStatus;
        if (task.taskSet() == "SERVICE") {
            serviceTaskStatus[oldStatus]--;
            serviceTaskStatus[taskStatus]++;
        } else {
            containerTaskStatus[oldStatus]--;
            containerTaskStatus[taskStatus]++;
            containerTaskStatusPerAgent[std::make_pair(agName, oldStatus)]--;
            containerTaskStatusPerAgent[std::make_pair(agName, taskStatus)]++;

            if ((taskStatus == TASK_STOPPED) ||
                (taskStatus == TASK_FAILED) ||
                (taskStatus == TASK_FINISHED) ||
                (taskStatus == TASK_UNKNOWN_STATE)){
                containerTaskLastMessage.erase(containerTaskLastMessage.find(agName));
            }

            TaskStatusSpectra spec = convertTaskStatusToSpectra(agName);

            const std::string & hostIp = task.taskHost();
            ProcessingHostInfo * procHostInfo = Config::procFmkInfo->hostsInfo[hostIp];
            TraceMsg("HOSTIP: " + hostIp);
            for (auto & agi : procHostInfo->agInfo) {
                TraceMsg("-- AGNAME: " + agi.name);
                if (agi.name == agName) {
                    agi.taskStatus = spec;
                    TraceMsg("Placing spectrum " + spec.toJsonStr() +
                        " placed for " + agName);
                }
            }
        }
    }

    if (taskStatus == TASK_FINISHED) {
        InfoMsg("Finished task " + taskName);
    }

    tskRegMsgs[taskName] = task.str();
}

//----------------------------------------------------------------------
// Method: processHostMonMsg
//----------------------------------------------------------------------
void TskMng::processHostMonMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    // Place new information in general structure
    consolidateMonitInfo(m);
}

//----------------------------------------------------------------------
// Method: convertTaskStatusToSpectra
// Convert set of status for an agent to a spectra tuple
//----------------------------------------------------------------------
TaskStatusSpectra TskMng::convertTaskStatusToSpectra(std::string & agName)
{
    //TaskStatusSpectra spec;

    std::map<TaskStatusPerAgent, int>::iterator
        nonValid = containerTaskStatusPerAgent.end();

    auto getNumOf = [&] (TaskStatus s) -> int
        { std::map<TaskStatusPerAgent, int>::iterator
          it = containerTaskStatusPerAgent.find(std::make_pair(agName, s));
          return (it == nonValid) ? 0 : it->second; };

    TaskStatusSpectra spec(getNumOf(TASK_RUNNING),
                           getNumOf(TASK_SCHEDULED),
                           getNumOf(TASK_PAUSED),
                           getNumOf(TASK_STOPPED),
                           getNumOf(TASK_FAILED),
                           getNumOf(TASK_FINISHED));
    TraceMsg("~~~~~> " + agName + ": "
             + std::to_string(spec.scheduled) + ", "
             + std::to_string(spec.running) + ", "
             + std::to_string(spec.paused) + ", "
             + std::to_string(spec.stopped) + ", "
             + std::to_string(spec.failed) + ", "
             + std::to_string(spec.finished) + " = "
             + std::to_string(spec.total));

    return spec;

    // return spec;
}

//----------------------------------------------------------------------
// Method: execTask
// Execute the rule requested by Task Orchestrator
//----------------------------------------------------------------------
void TskMng::execContainerTask()
{
}

inline double weightFunc(double load, double tasks) {
    return 100 * load + tasks;
}

//----------------------------------------------------------------------
// Method: selectAgent
// Send a TaskProcessingMsg to the Task Manager, requesting the
// execution of a rule
//----------------------------------------------------------------------
std::string TskMng::selectAgent()
{
    return std::string();
}

//----------------------------------------------------------------------
// Method: consolidateMonitInfo
// Consolidates the monitoring info retrieved from the processing hosts
//----------------------------------------------------------------------
void TskMng::consolidateMonitInfo(MessageString & m)
{
    std::unique_lock<std::mutex> ulck(mtxHostInfo);

    Message<MsgBodyTSK> msg(m);
    MsgBodyTSK & body = msg.body;
    Json::FastWriter fastWriter;
    std::string s(fastWriter.write(body["info"]));
    HostInfo hostInfo(s);
    std::string hostIp = hostInfo.hostIp;
    TraceMsg("Consolidating " + s + (Config::agentMode[hostIp] == CONTAINER ?
                                " (CONT) " :
                                " (SRV) ") + " for host " + hostIp);
    
    switch (Config::agentMode[hostIp]) {
    case CONTAINER:
        Config::procFmkInfo->hostsInfo[hostIp]->hostInfo = hostInfo;
        break;
    case SERVICE:
        Config::procFmkInfo->swarmInfo[hostIp]->hostInfo = hostInfo;
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------
// Method: getTskRepUpdate
// Provide an update on the Task Report information
//----------------------------------------------------------------------
bool TskMng::getTskRepUpdate(json & tskRepData)
{
    std::unique_lock<std::mutex> ulck(mtxTskRegMsg);
    bool dataAvailable = ! tskRegMsgs.empty();
    if (dataAvailable) {

        // Prepare message and send it
        int maxSize = MAX_MESSAGE_SIZE - 400; // header must be added

        std::string multiMsg;
        for (auto & kv : tskRegMsgs) {
            if ((multiMsg.length() + kv.second.length()) < maxSize) {
                multiMsg += "\"" + kv.first + "\":" + kv.second + ",";
                tskRegMsgs.erase(kv.first);
            }
        }
        multiMsg.pop_back();

        tskRepData = JValue("{" + multiMsg + "}").val();
    }
    
    return dataAvailable;
}

//----------------------------------------------------------------------
// Method: getProcFmkInfoUpdate
// Provide an update on the ProcessingFrameworkInfo structure
//----------------------------------------------------------------------
void TskMng::getProcFmkInfoUpdate(json & fmkInfoValue)
{
    std::unique_lock<std::mutex> ulck(mtxHostInfo);
    fmkInfoValue = JValue( Config::procFmkInfo->toJsonStr() ).val();
}

//----------------------------------------------------------------------
// Method: scheduleTask
// Schedule task for later provision to the requesting agents
//----------------------------------------------------------------------
void TskMng::scheduleTask(TaskInfo & task)
{
    // Store task in specific container
    if (task.taskSet() == "CONTAINER") {
        containerTasks.push_back(task);
    } else if (task.taskSet() == "SERVICE") {
        serviceTasks.push_back(task);
    } else {
        WarnMsg("Task not identified neither for Container nor Services: " + task.taskSet());
        RaiseSysAlert(Alert(Alert::System,
                            Alert::Warning,
                            Alert::Comms,
                            std::string(__FILE__ ":" Stringify(__LINE__)),
                            "Task not identified neither for Container nor Services: "
                            + task.taskSet(),
                            0));
    }
}


//}
