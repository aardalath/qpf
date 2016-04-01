/******************************************************************************
 * File:    cfginfo.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.ConfigurationInfo
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
 *   Implement ConfigurationInfo class
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

#include "cfginfo.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

ConfigurationInfo::ConfigurationInfo()
{
}

void ConfigurationInfo::clear()
{
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

ConfigurationInfo & ConfigurationInfo::data() {
    static ConfigurationInfo cfgInfoInstance {};
    return cfgInfoInstance;
}

void ConfigurationInfo::dump()
{
    std::cerr << "==[ CONFIGURATION ]============================================================" << std::endl;

    // Configuration file name
    std::cerr << "cfgFileName                : " << cfgFileName << std::endl;
    std::cerr << "currentMachine             : " << currentMachine << std::endl;
    std::cerr << "currentUser                : " << currentUser << std::endl;

    std::cerr << "masterMachine              : " << masterMachine << std::endl;
    std::cerr << "isMaster                   : " << isMaster << std::endl;

    // General
    std::cerr << "appName                    : " << appName << std::endl;
    std::cerr << "appVersion                 : " << appVersion << std::endl;
    std::cerr << "lastAccess                 : " << lastAccess << std::endl;

    // Orchestration
    //OrchestrationParameters             orcParams;

    // File naming
    std::cerr << "parsing_regex              : " << parsing_regex << std::endl;
    std::cerr << "parsing_assign             : " << parsing_assign << std::endl;
    std::cerr << "product_id_tpl             : " << product_id_tpl << std::endl;
    std::cerr << "data_ext                   : " << data_ext << std::endl;
    std::cerr << "meta_ext                   : " << meta_ext << std::endl;
    std::cerr << "log_ext                    : " << log_ext << std::endl;

    // Nodes
    /*
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
    */

    // Storage information
    std::cerr << "storage.base               : " << storage.base << std::endl;
    std::cerr << "storage.tasks              : " << storage.tasks << std::endl;
    std::cerr << "storage.inbox.protocol     : " << storage.inbox.protocol << std::endl;
    std::cerr << "storage.inbox.address      : " << storage.inbox.address << std::endl;
    std::cerr << "storage.inbox.port         : " << storage.inbox.port << std::endl;
    std::cerr << "storage.inbox.user         : " << storage.inbox.user << std::endl;
    std::cerr << "storage.inbox.passwd       : " << storage.inbox.passwd << std::endl;
    std::cerr << "storage.inbox.path         : " << storage.inbox.path << std::endl;
    std::cerr << "storage.local_archive.path : " << storage.local_archive.path << std::endl;
    std::cerr << "storage.archive.protocol   : " << storage.archive.protocol << std::endl;
    std::cerr << "storage.inbox.address      : " << storage.inbox.address << std::endl;
    std::cerr << "storage.inbox.port         : " << storage.inbox.port << std::endl;
    std::cerr << "storage.inbox.user         : " << storage.inbox.user << std::endl;
    std::cerr << "storage.inbox.passwd       : " << storage.inbox.passwd << std::endl;
    std::cerr << "storage.inbox.path         : " << storage.inbox.path << std::endl;
    std::cerr << "storage.gateway.path       : " << storage.gateway.path << std::endl;
    std::cerr << "storage.outbox.protocol    : " << storage.outbox.protocol << std::endl;
    std::cerr << "storage.outbox.address     : " << storage.outbox.address << std::endl;
    std::cerr << "storage.outbox.port        : " << storage.outbox.port << std::endl;
    std::cerr << "storage.outbox.user        : " << storage.outbox.user << std::endl;
    std::cerr << "storage.outbox.passwd      : " << storage.outbox.passwd << std::endl;
    std::cerr << "storage.outbox.path        : " << storage.outbox.path << std::endl;

    // Data Base info
    std::cerr << "DBHost                     : " << DBHost << std::endl;
    std::cerr << "DBPort                     : " << DBPort << std::endl;
    std::cerr << "DBName                     : " << DBName << std::endl;
    std::cerr << "DBUser                     : " << DBUser << std::endl;
    std::cerr << "DBPwd                      : " << DBPwd << std::endl;

    std::cerr << "==============================================================================" << std::endl;
}

}
