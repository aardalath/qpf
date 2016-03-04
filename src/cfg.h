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
#include "msgtypes.h"
//#include "component.h"
#include "commnode.h"
using namespace LibComm;

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

typedef LibComm::Router2RouterPeer::Peer           Peer;
typedef LibComm::Router2RouterPeer::Frame          Frame;

struct Rule {
    std::string                         name;
    std::vector<std::string>            inputs;
    std::vector<std::string>            outputs;
    std::string                         processingElement;
};

typedef std::map<Rule *, ProductList>  RuleInputs;

struct Processor {
    std::string                         name;
    std::string                         exePath;
    std::string                         inPath;
    std::string                         outPath;
};

struct OrchestrationParameters {
    std::vector<std::string>            productTypes;
    std::vector<Rule *>                 rules;
    std::map<std::string, Processor *>  processors;
};

struct OrchestrationMaps {
    std::multimap<std::string, Rule *>  prodAsInput;
    std::map<Rule *, std::string>       ruleDesc;
};

enum LocalArchiveMethod { LINK, MOVE, COPY };

struct StorageExternal {
    std::string protocol;
    std::string address;
    std::string port;
    std::string user;
    std::string passwd;
    std::string inbox;
};

struct StorageLocal {
    std::string path;
    LocalArchiveMethod method;
};

struct StorageShared {
    std::string local_path;
    std::string external_path;
};

struct StorageConfig {
    StorageExternal in;
    StorageLocal    local;
    StorageShared   shared;
    StorageExternal out;
};

class Component;

struct ConfigurationInfo {
    // Configuration file name
    std::string                         cfgFileName;
    std::string                         currentMachine;
    std::string                         currentUser;

    // General
    std::string                         appName;
    std::string                         appVersion;
    std::string                         lastAccess;

    // Orchestration
    OrchestrationParameters             orcParams;

    // Nodes
    std::vector<std::string>            peerNames;
    std::vector<Peer>                   peersCfg;
    std::map<std::string, Peer*>        peersCfgByName;
    std::map<std::string,
             std::vector<std::string> > connections;
    Peer                                qpfhmiCfg;
    Peer                                evtMngCfg;

    // Nodes for current machine
    std::vector<Component *>            peerNodes;
    std::vector<Component *>            peerAgents;

    // Machine network
    std::vector<std::string>            machines;
    std::map<std::string,
             std::vector<std::string> > machineNodes;
    bool                                hmiPresent;

    // Storage information
    StorageConfig                       storage;

    // Data Base info
    std::string                         DBHost;
    std::string                         DBPort;
    std::string                         DBName;
    std::string                         DBUser;
    std::string                         DBPwd;

    void clear() {
        orcParams.processors.clear();
        orcParams.productTypes.clear();
        orcParams.rules.clear();
        peerNames.clear();
        peersCfg.clear();
        peersCfgByName.clear();
        connections.clear();
        peerNodes.clear();
        peerAgents.clear();
        machines.clear();
        machineNodes.clear();
    }
};

}

#endif  /* CFG_H */
