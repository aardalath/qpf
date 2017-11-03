/******************************************************************************
 * File:    tskage.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.TskAge
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
 *   Declare Tskage class
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

#ifndef TSKAGE_H
#define TSKAGE_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - thread
//------------------------------------------------------------
#include <thread>
#include <mutex>
#include <sys/stat.h>
#include <fstream>
#include <time.h>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - component.h
//------------------------------------------------------------
#include "component.h"
#include "dckmng.h"
#include "urlhdl.h"
#include "hostinfo.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

//==========================================================================
// Class: TskAge
//==========================================================================
class TskAge : public Component {

public:
    // Information on the service to be created, in Docker Swarm
    struct ServiceInfo {
        std::string              service;
        std::string              serviceImg;
        std::string              exe;
        std::vector<std::string> args;
        int                      scale;
    };

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    TskAge(const char * name, const char * addr = 0, Synchronizer * s = 0,
           AgentMode mode = CONTAINER,
           const std::vector<std::string> & nds = std::vector<std::string>(),
           ServiceInfo * srvInfo = 0);

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    TskAge(std::string name, std::string addr = std::string(), Synchronizer * s = 0,
           AgentMode mode = CONTAINER,
           const std::vector<std::string> & nds = std::vector<std::string>(),
           ServiceInfo * srvInfo = 0);

protected:

#undef T
#define TLIST_PSTATUS T(IDLE), T(WAITING), T(PROCESSING), T(FINISHING)

#define T(x) x
    enum ProcStatus { TLIST_PSTATUS };
#undef T
    static const std::string ProcStatusName[];

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: runEachIteration
    //----------------------------------------------------------------------
    virtual void runEachIteration();

    //----------------------------------------------------------------------
    // Method: processTskProcMsg
    //----------------------------------------------------------------------
    virtual void processTskProcMsg(ScalabilityProtocolRole* c, MessageString & m);

    //----------------------------------------------------------------------
    // Method: processSubcmdMsg
    //----------------------------------------------------------------------
    virtual void processSubcmdMsg(MessageString & m);

private:
    //----------------------------------------------------------------------
    // Method: runEachIterationForContainers
    //----------------------------------------------------------------------
    void runEachIterationForContainers();

    //----------------------------------------------------------------------
    // Method: runEachIterationForServices
    //----------------------------------------------------------------------
    void runEachIterationForServices();

    //----------------------------------------------------------------------
    // Method: applyActionOnContainer
    //----------------------------------------------------------------------
    void applyActionOnContainer(std::string & act, std::string & contId);

    //----------------------------------------------------------------------
    // Method: armHostInfoTimer
    //----------------------------------------------------------------------
    void armHostInfoTimer();
    
    //----------------------------------------------------------------------
    // Method: sendTaskReport
    //----------------------------------------------------------------------
    void sendTaskReport(std::string contId);

    //----------------------------------------------------------------------
    // Method: transferOutputProducts
    //----------------------------------------------------------------------
    void transferOutputProducts(TaskInfo & task);

    //----------------------------------------------------------------------
    // Method: sendHostInfoUpdate
    //----------------------------------------------------------------------
    void sendHostInfoUpdate();

    //----------------------------------------------------------------------
    // Method: resetProgress
    //----------------------------------------------------------------------
    void resetProgress();
    
    //----------------------------------------------------------------------
    // Method: updateProgress
    //----------------------------------------------------------------------
    void updateProgress();
    
    //----------------------------------------------------------------------
    // Method: endProgress
    //----------------------------------------------------------------------
    void endProgress();
    
    Property(TskAge, std::string, workDir, WorkDir);
    Property(TskAge, std::string, sysDir,  SysDir);
    Property(TskAge, bool,        remote,  Remote);

private:
    AgentMode                agentMode;
    ProcStatus               pStatus;
    DockerMng *              dckMng;

    ServiceInfo *            serviceInfo;
    std::string              srvManager;
    std::vector<std::string> nodes;

    std::map<std::string, TaskInfo*> containerToTaskMap;
    std::map<std::string, time_t>    containerEpoch;
    
    TaskInfo *               runningTask;
    TaskStatus               taskStatus;

    std::string              internalTaskNameIdx;
    std::string              exchangeDir;
    std::string              exchgIn;
    std::string              exchgOut;
    std::string              exchgLog;

    std::string              ruleBasedName;

    bool                     isLogFileOpen;
    std::ifstream            logFileHdl;
    size_t                   logFilePos;
    std::string              logDir;
    std::string              logFile;

    MessageString            origMsgString;

    int                      numTask;
    int                      waitingCycles;
    int                      maxWaitingCycles;
    int                      idleCycles;
    int                      idleCyclesBeforeRequest;
    int                      workingDuring;

    int                      progress;
    
    URLHandler               urlh;

    HostInfo                 hostInfo;

    bool                     isTaskRequestActive;
};

//}

#endif  /* TASKAGENT_H */
