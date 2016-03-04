/******************************************************************************
 * File:    cfg.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.cfg
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
 *   Declare configuration related types
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

#ifndef CFG_H
#define CFG_H

//------------------------------------------------------------
// Topic: System dependencies
//------------------------------------------------------------
#include <string>
#include <map>

//------------------------------------------------------------
// Topic: Project dependencies
//------------------------------------------------------------
#include "evtmng.h"
#include "datamng.h"
#include "logmng.h"
#include "taskmng.h"
#include "taskagent.h"
#include "taskorc.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

    typedef LibComm::Router2RouterPeer::Peer           Peer;
    typedef LibComm::Router2RouterPeer::Frame          Frame;
    typedef TaskOrchestrator::OrchestrationParameters  OrcParameters;

    struct PeerNodesData {
    PeerNodesData() :
        evtMng(0), dataMng(0), logMng(0), tskMng(0), tskOrc(0) {}
        EventManager *                      evtMng;
        DataManager *                       dataMng;
        LogManager *                        logMng;
        TaskManager *                       tskMng;
        TaskOrchestrator *                  tskOrc;
        std::vector<TaskAgent *>            tskAgents;
        std::vector<CommNode *>             nodes;
        std::map<std::string,
            std::vector<std::string> >      connections;
    };

    struct ConfigurationInfo {
        // Configuration file name
        std::string                         cfgFileName;

        // General
        std::string                         appName;
        std::string                         appVersion;
        std::string                         lastAccess;

        // Orchestration
        OrcParameters                       orcParams;

        // Nodes
        PeerNodesData                       peerNodesData;
        std::vector<std::string>            peerNames;
        std::vector<Peer>                   peersCfg;
        Peer                                qpfhmiCfg;
        Peer                                evtMngCfg;

        // Machine network
        std::vector<std::string>            machines;
        std::map<std::string,
            std::vector<std::string> >      machineNodes;

        // Data Base info
        std::string                         DBHost;
        std::string                         DBPort;
        std::string                         DBName;
        std::string                         DBUser;
        std::string                         DBPwd;
    };

}

#endif  /* CFG_H */
