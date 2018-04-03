/******************************************************************************
 * File:    taskage.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskage
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement TaskAge class
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

#include "tskage.h"

#include "log.h"
#include "tools.h"
#include "channels.h"
#include "message.h"
#include "str.h"

#include <dirent.h>

#include "cntrmng.h"
#include "srvmng.h"
#include "filenamespec.h"
#include "timer.h"
#include "filetools.h"
using namespace FileTools;

#include "config.h"

using Configuration::cfg;

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

#define T(x) #x
const std::string TskAge::ProcStatusName[] { TLIST_PSTATUS };
#undef T

const int HOST_INFO_TIMER            = 10000;

const int MAX_WAITING_CYCLES         = 50;
const int IDLE_CYCLES_BEFORE_REQUEST = 1;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TskAge::TskAge(const char * name, const char * addr, Synchronizer * s,
               AgentMode mode, const std::vector<std::string> & nds,
               ServiceInfo * srvInfo)
    : Component(name, addr, s), remote(true), agentMode(mode), nodes(nds),
      pStatus(IDLE), serviceInfo(srvInfo)
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TskAge::TskAge(std::string name, std::string addr, Synchronizer * s,
               AgentMode mode, const std::vector<std::string> & nds,
               ServiceInfo * srvInfo)
    : Component(name, addr, s), remote(true), agentMode(mode), nodes(nds),
      pStatus(IDLE), serviceInfo(srvInfo)
{
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TskAge::fromRunningToOperational()
{
    if (agentMode == CONTAINER) {

        // Create Container Manager
        dckMng = new ContainerMng;

        // Set parameters for requesting tasks and waiting
        idleCycles              = 0;
        maxWaitingCycles        = MAX_WAITING_CYCLES;
        idleCyclesBeforeRequest = IDLE_CYCLES_BEFORE_REQUEST;

        TraceMsg("Agent Mode: CONTAINER");

    } else {

        // Create list of workers
        srvManager = nodes.at(0);
        nodes.erase(nodes.begin());

        TraceMsg("Agent Mode: SERVICE  - " + srvManager);

        // Create Service Manager
        dckMng = new ServiceMng(srvManager, nodes);

        if (serviceInfo != 0) {
            // Create Service
            dckMng->createService(serviceInfo->service, serviceInfo->serviceImg,
                                  serviceInfo->scale,
                                  serviceInfo->exe, serviceInfo->args);
        }
    }

    numTask = 0;

    isTaskRequestActive = true;

    prevTaskStatus = TASK_UNKNOWN_STATE;
    prevProgress = -1;
    prevInspStatus = "";
    prevInspCode = -127;

    // Get initial values for Host Info structure
    hostInfo.hostIp = compAddress;
    hostInfo.cpuInfo.overallCpuLoad.timeInterval = 0;
    hostInfo.update();
    hostInfo.cpuInfo.overallCpuLoad.timeInterval = 10;

    armHostInfoTimer();

    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: runEachIteration
//----------------------------------------------------------------------
void TskAge::runEachIteration()
{
    if (agentMode == CONTAINER) {
        runEachIterationForContainers();
    } else {
        runEachIterationForServices();
    }
}

//----------------------------------------------------------------------
// Method: runEachIterationForContainers
//----------------------------------------------------------------------
void TskAge::runEachIterationForContainers()
{
    // Declare status
    DbgMsg("Status is " + ProcStatusName[pStatus] +
           " at iteration " + str::toStr<int>(iteration));

    // Upon status, perform the required action
    switch (pStatus) {
    case IDLE:
        ++idleCycles;
        // Request task for processing in case the agent is idle
        if (isTaskRequestActive) {
            if (idleCycles > idleCyclesBeforeRequest) {
                // Create message and send
                Message<MsgBodyTSK> msg;
                msg.buildHdr(ChnlTskProc, MsgTskRqst, CHNLS_IF_VERSION,
                             compName, "TskMng",
                             "", "", "");
                
                std::string chnl(ChnlTskProc + "_" + compName);
                pStatus = WAITING;
                InfoMsg("Switching to status " + ProcStatusName[pStatus]);
                waitingCycles = 0;
                send(chnl, msg.str());

                DBG("Sending request via channel " + chnl);
                DbgMsg("Sending request via channel " + chnl);
            }
        }
        break;
    case WAITING:
        ++waitingCycles;
        if (waitingCycles > maxWaitingCycles) {
            pStatus = IDLE;
            InfoMsg("Switching back to status " + ProcStatusName[pStatus]);
            idleCycles = 0;
        }
        break;
    case PROCESSING:
        break;
    case FINISHING:
        pStatus = IDLE;
        InfoMsg("Switching back to status " + ProcStatusName[pStatus]);
        idleCycles = 0;
        break;
    default:
        break;
    }

    // Update status for running containers
    for (auto const & kv : containerEpoch) {
        std::string contId = kv.first;
        sendTaskReport(contId);
    }

}

//----------------------------------------------------------------------
// Method: runEachIterationForServices
//----------------------------------------------------------------------
void TskAge::runEachIterationForServices()
{
    // Every N iterations, check if this is the first agent in the
    // host, and if it is update the hostInfo structure, and send it
    // to the TskMng
    
    if ((iteration % 50) == 0) {
        sendHostInfoUpdate();
    }
}

//----------------------------------------------------------------------
// Method: armHostInfoTimer
// Arm new timer for sending HostInfo updates
//----------------------------------------------------------------------
void TskAge::armHostInfoTimer()
{
    Timer * hstnfoSender = new Timer(HOST_INFO_TIMER, true,
                                     &TskAge::sendHostInfoUpdate, this);
}

//----------------------------------------------------------------------
// Method: processTskProcMsg
//----------------------------------------------------------------------
void TskAge::processTskProcMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    Message<MsgBodyTSK> msg(m);

    // Return if not recipient
    if (msg.header.target() != compName) { return; }

    if ((pStatus != WAITING) && (pStatus != IDLE)) { return; }

    // Define and set task object
    MsgBodyTSK & body = msg.body;
    TaskInfo * runningTask = new TaskInfo(body["info"]);
    TaskInfo & task = (*runningTask);

    task["taskHost"]  = compAddress;
    task["taskAgent"] = compName;

    assert(compName == msg.header.target());
    DBG(">>>>>>>>>> " << compName
        << " RECEIVED TASK INFO FOR PROCESSING\n"
        ">>>>>>>>>> Task name:" << msg.body("info")["name"].asString());

    numTask++;

    //---- Define processing environment
    std::string sessId = task.taskSession();
    DBG(">> [" << sessId << "] vs. [" << cfg.sessionId << "]");
    if (sessId != cfg.sessionId) {
        DBG(compName + ">> CHANGING SESSION ID");
        str::replaceAll(workDir, cfg.sessionId, sessId);
        cfg.synchronizeSessionId(sessId);
    }

    //---- Create exchange area
    internalTaskNameIdx = (compName + "-" + timeTag() + "-" +
                           std::to_string(numTask));

    exchangeDir = workDir + "/" + internalTaskNameIdx;
    exchgIn     = exchangeDir + "/in";
    exchgOut    = exchangeDir + "/out";
    exchgLog    = exchangeDir + "/log";

    mkdir(exchangeDir.c_str(), Config::PATHMode);
    mkdir(exchgIn.c_str(),     Config::PATHMode);
    mkdir(exchgOut.c_str(),    Config::PATHMode);
    mkdir(exchgLog.c_str(),    Config::PATHMode);

    //---- Retrieve the input products
    urlh.setProcElemRunDir(workDir, internalTaskNameIdx);
    if (remote) {
        urlh.setRemoteCopyParams(cfg.network.masterNode(), compAddress);
    }

    int i = 0;
    for (auto & m : task.inputs.products) {
        urlh.setProduct(m);
        ProductMetadata & mg = urlh.fromGateway2Processing();
        task.inputs.products.push_back(mg);
        task["inputs"][i] = mg.val();
        ++i;
    }

    //----  * * * LAUNCH TASK * * *
    std::string contId;
    std::string procName(task.taskPath());
    std::string sourceProcCfgFile = Config::PATHProcs + "/" + procName + "/sample.cfg.json";
    std::string targetProcCfgFile = exchangeDir + "/" + procName + ".cfg";
    copyfile(sourceProcCfgFile, targetProcCfgFile);
    TRC("Copying " + sourceProcCfgFile + " to " + targetProcCfgFile);
    if (dckMng->createContainer(procName, exchangeDir, contId)) {
        InfoMsg("Running task " + task.taskName() +
                " (" + task.taskPath() + ") within container " + contId);
        origMsgString = m;

        // Save container info
        containerToTaskMap[contId]  = runningTask;
        containerEpoch[contId]      = time(0);
        
        usleep(50000); // 50 ms

        // Set processing status
        pStatus = PROCESSING;
        workingDuring = 0;
        resetProgress();            
    } else {
        WarnMsg("Couldn't execute docker container");
        
        delete runningTask;

        pStatus = IDLE;
        InfoMsg("Switching back to status " + ProcStatusName[pStatus]);
        idleCycles = 0;
    }
}

//----------------------------------------------------------------------
// Method: processSubcmdMsg
//----------------------------------------------------------------------
void TskAge::processSubcmdMsg(MessageString & m)
{
    TRC("Sub-command message received: " + m);

    Message<MsgBodyTSK> msg(m);

    std::string subCmd   = msg.body["subcmd"].asString();
    SubjectId   subj     = (SubjectId)(msg.body["target_type"].asInt());
    std::string subjName = msg.body["target"].asString();

    switch (subj) {
    case PROC_TASK:
        if (containerToTaskMap.find(subjName) == containerToTaskMap.end()) { return; }

        TRC("Trying to " + subCmd + " container with id " + subjName);
        
        applyActionOnContainer(subCmd, subjName);
        sendTaskReport(subjName);

        break;

    case PROC_AGENT:
        TraceMsg("Trying to " + subCmd + " agent " + subjName);
        if (compName == subjName) {
            for (auto const & kv : containerEpoch) {
                std::string contId = kv.first;
                TaskInfo & task = (*(containerToTaskMap[contId]));
                TaskStatus taskStatus = TaskStatus(task.taskStatus());
                // Send new update on container info, unless it is too old
                if ((taskStatus != TASK_FAILED) && (taskStatus != TASK_FINISHED) &&
                    (time(0) - kv.second) < cfg.MaxContainerAge) {
                    applyActionOnContainer(subCmd, contId);
                }
            }
        }
        break;

    case PROC_HOST:
        TraceMsg("Trying to " + subCmd + " host " + subjName);
        if (cfg.currentHostAddr == subjName) {
            for (auto const & kv : containerEpoch) {
                std::string contId = kv.first;
                TaskInfo & task = (*(containerToTaskMap[contId]));
                TaskStatus taskStatus = TaskStatus(task.taskStatus());
                // Send new update on container info, unless it is too old
                if ((taskStatus != TASK_FAILED) && (taskStatus != TASK_FINISHED) &&
                    (time(0) - kv.second) < cfg.MaxContainerAge) {
                    applyActionOnContainer(subCmd, contId);
                }
            }
        }
        break;

    default:
        break;
    }
    TRC("Processing of subcmd message done.");
}

//----------------------------------------------------------------------
// Method: applyActionOnContainer
//----------------------------------------------------------------------
void TskAge::applyActionOnContainer(std::string & act, std::string & contId)
{
    std::vector<std::string> noargs;
    
    isTaskRequestActive = true;

    if ((act == "PAUSE") || (act == "SUSPEND")) {
        dckMng->runCmd("pause",   noargs, contId);
        isTaskRequestActive = false;
    } else if ((act == "RESUME") || (act == "REACTIVATE")) {
        dckMng->runCmd("unpause", noargs, contId);
    } else if ((act == "CANCEL") || (act == "STOP")) {
        dckMng->runCmd("stop",    noargs, contId);
    } else {
        //
    }    
}

//----------------------------------------------------------------------
// Method: sendTaskReport
//----------------------------------------------------------------------
void TskAge::sendTaskReport(std::string contId)
{
    // Define and set task object
    auto const & itTaskInfo = containerToTaskMap.find(contId);
    if (itTaskInfo == containerToTaskMap.end()) { return; }
    
    TaskInfo & task = (*(itTaskInfo->second));
    if ((task.taskStatus() == TASK_FAILED) ||
        (task.taskStatus() == TASK_FINISHED)) { return; }

    // Get updated Docker info
    json taskData = retrieveDockerInfo(contId, false);

    std::string inspStatus = taskData["State"]["Status"].asString();
    int         inspCode   = taskData["State"]["ExitCode"].asInt();

    taskStatus = computeTaskStatus(inspStatus, inspCode);

    bool taskHasEnded = taskEnded(taskStatus);

    if (taskHasEnded) {
        InfoMsg("Task container monitoring finished");
        if (taskStatus == TASK_FINISHED) { 
            endProgress(); 
        }
        taskData = retrieveDockerInfo(contId, true);
    } else {
        workingDuring++;
        updateProgress();
    }

    // Avoid unnecessary messages
    if ((taskStatus == prevTaskStatus) && 
        (progress   == prevProgress) &&
        (inspStatus == prevInspStatus) &&
        (inspCode   == prevInspCode)) { return; }
    
    prevTaskStatus = taskStatus; 
    prevProgress   = progress; 
    prevInspStatus = inspStatus; 
    prevInspCode   = inspCode;

    // Update progress
    taskData["State"]["Progress"] = std::to_string(progress);

    // Put declared status in task info structure...
    // ... and add it as well to the taskData JSON structure
    task["taskStatus"] = taskStatus;
    taskData["State"]["TaskStatus"] = taskStatus;

    // Include additional info
    json addInfo;
    addInfo["TaskName"]  = task.taskName();
    addInfo["Agent"]     = compName;
    addInfo["Proc"]      = task.taskPath();
    addInfo["Inputs"]    = task.inputs.str();
    addInfo["MainInput"] = task.inputs.products.at(0).productId();
    TRC("INPUTS: " + task.inputs.str());
    
    taskData["Info"] = addInfo;
    
    // Place all taskdata information into task structure
    task["taskData"] = taskData;

    if ((taskStatus == TASK_FINISHED) || (taskStatus == TASK_FAILED)) {
        transferOutputProducts(task);
        taskHasEnded = true;
    }

    sendBodyElem<MsgBodyTSK>(ChnlTskProc,
                             ChnlTskProc + "_" + compName, MsgTskRep,
                             compName, "TskMng",
                             "info", task.str(),
                             origMsgString);

    if (taskHasEnded) {
        pStatus = FINISHING;
        InfoMsg("Switching to status " + ProcStatusName[pStatus]);
        
        containerToTaskMap.erase(containerToTaskMap.find(contId));
        containerEpoch.erase(containerEpoch.find(contId));
    }
    
}

//----------------------------------------------------------------------
// Method: computeTaskStatus
//----------------------------------------------------------------------
TaskStatus TskAge::computeTaskStatus(std::string & inspStatus, int & inspCode)
{
    if        (inspStatus == "running") {
        return TASK_RUNNING;
    } else if (inspStatus == "paused") {
        return TASK_PAUSED;
    } else if (inspStatus == "created") {
        return TASK_STOPPED;
    } else if (inspStatus == "dead") {
        return TASK_STOPPED;
    } else if (inspStatus == "exited") {
        return (inspCode == 0) ? TASK_FINISHED : TASK_FAILED;
    } else {
        return TASK_UNKNOWN_STATE;
    }
}

//----------------------------------------------------------------------
// Method: taskEnded
//----------------------------------------------------------------------
bool TskAge::taskEnded(TaskStatus & taskStatus)
{
    return ((taskStatus == TASK_STOPPED) ||
            (taskStatus == TASK_FAILED) ||
            (taskStatus == TASK_FINISHED) ||
            (taskStatus == TASK_UNKNOWN_STATE));
}

//----------------------------------------------------------------------
// Method: retrieveDockerInfo
//----------------------------------------------------------------------
json TskAge::retrieveDockerInfo(std::string & contId, bool fullInfo)
{
    static std::string inspectSelection("{\"Id\":{{json .Id}},"
                                        "\"State\":{{json .State}},"
                                        "\"Path\":{{json .Path}},"
                                        "\"Args\":{{json .Args}}}");
    std::stringstream info;
    bool ok = true;
    do {
        if (fullInfo) {
            info.str(""); 
        } else {
            info.str(inspectSelection);
        }
        ok = dckMng->getInfo(contId, info);
    } while (! ok);

    return ((fullInfo) ? JValue(info.str()).val()[0] : JValue(info.str()).val());
}

//----------------------------------------------------------------------
// Method: transferOutputProducts
//----------------------------------------------------------------------
void TskAge::transferOutputProducts(TaskInfo & task)
{
    DBG("Transferring output products for task: " << task.taskName());

    //-------------------------------------------------------------------
    // Get output data
    //-------------------------------------------------------------------
    std::vector<std::string> outFiles;
    DIR * dp = NULL;
    struct dirent * dirp;
    for (auto & vd : {exchgOut, exchgLog}) {
        if ((dp = opendir(vd.c_str())) == NULL) {
            WarnMsg("Cannot open output directory " + vd);
        } else {
            while ((dirp = readdir(dp)) != NULL) {
                if (dirp->d_name[0] != '.') {
                    std::string dname(dirp->d_name);
                    //if (dname.substr(0, 3) != "EUC") { continue; }
                    std::string outputFile = vd + "/" + dname;
                    outFiles.push_back(outputFile);
                }
            }
            closedir(dp);
        }
    }

    TraceMsg("outFiles has " + std::to_string(outFiles.size()) + " elements");
    task.outputs.products.clear();

    ProductMetadata imd = task.inputs.products.at(0);
    
    FileNameSpec fs;
    for (unsigned int i = 0; i < outFiles.size(); ++i) {
        ProductMetadata m;
        if (fs.parseFileName(outFiles.at(i), m, ProcessingSpace, task.taskPath())) {
            // Place output product at external (output) shared area
            m["procTargetType"] = imd["procTargetType"];
            m["procTarget"]     = imd["procTarget"];
            urlh.setProduct(m);
            m = urlh.fromProcessing2Gateway();
        } else {
            continue;
        }
        task.outputs.products.push_back(m);
        task["outputs"][i] = m.val();
    }
}

//----------------------------------------------------------------------
// Method: sendHostInfoUpdate
//----------------------------------------------------------------------
void TskAge::sendHostInfoUpdate()
{
    // Update host information
    hostInfo.update();

    sendBodyElem<MsgBodyTSK>(ChnlTskProc,
                             ChnlTskProc + "_" + compName, MsgHostMon,
                             compName, "TskMng",
                             "info", hostInfo.toJsonStr(), "");

    armHostInfoTimer();
}

//----------------------------------------------------------------------
// Method: resetProgress
//----------------------------------------------------------------------
void TskAge::resetProgress()
{
    // Initialize progress and log related variables
    progress = 0;
    isLogFileOpen = false;
}

//----------------------------------------------------------------------
// Method: updateProgress
//----------------------------------------------------------------------
void TskAge::updateProgress()
{
    // First, check that file exists and is open
    if (! isLogFileOpen) {
        logFilePos = 0;
        logDir = exchangeDir + "/log";
        logFile = "";
        
        // Look for log file in <exchangeDirr>/log
        while (logFile.empty()) {
            DIR * dp = NULL;
            struct dirent * dirp;
            if ((dp = opendir(logDir.c_str())) == NULL) {
                WarnMsg("Cannot open log directory " + logDir);
            } else {
                while ((dirp = readdir(dp)) != NULL) {
                    if (dirp->d_name[0] != '.') {
                        std::string dname(dirp->d_name);
                        //if (dname.substr(0, 3) != "EUC") { continue; }
                        logFile = logDir + "/" + dname;
                    }
                }
                closedir(dp);
            }
        }
        // Open log file
        if (! logFile.empty()) {
            logFileHdl.open(logFile);
            isLogFileOpen = true;
        }
    }

    if (! isLogFileOpen) { return; }
    
    // See if new content can be obtained from the log file
    logFileHdl.seekg(0, logFileHdl.end);
    int length = logFileHdl.tellg();
    
    const std::string ProgressTag(cfg.flags.progressString());
    
    // If new content is there, read it and process it
    if (length > logFilePos) {
        logFileHdl.seekg(logFilePos, logFileHdl.beg);

        std::string line;
        while (! logFileHdl.eof()) {
            // Get line, look for progress mark, and parse it
            // It is assumed that  progress is shown as follows:
            // .....:PROGRESS:... XXX%
            // where XXX is a float number representing the percentage
            // of progress, and that no other % appears in the line
            std::getline(logFileHdl, line);
            if (line.length() < 1) { break; }
            size_t progressTagPos = line.find(ProgressTag);
                      
            if (progressTagPos != std::string::npos) {
                size_t porcEndsAt = line.find_first_of("%");
                if (porcEndsAt != std::string::npos) {
                    std::string untilPerc = line.substr(0, porcEndsAt);
                    size_t porcBeginsAt = untilPerc.find_last_of(" ");
                    if (porcBeginsAt != std::string::npos) {
                        std::string percentage =
                            line.substr(porcBeginsAt + 1,
                                        porcEndsAt - porcBeginsAt);
                        progress = (int)floor(std::stof(percentage));
                    }
                }
            }
        }
        
        logFileHdl.clear();
        logFilePos = length;
    }
}

//----------------------------------------------------------------------
// Method: endProgress
//----------------------------------------------------------------------
void TskAge::endProgress()
{
    progress = 100;
    logFileHdl.close();
    isLogFileOpen = false;
}

//}
