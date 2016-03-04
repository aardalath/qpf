/******************************************************************************
 * File:    cfginfo.h
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
 *   Declare ConfigurationInfo class
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

#ifndef CFGINFO_H
#define CFGINFO_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - cfg.h
//------------------------------------------------------------
#include "cfg.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

class Component;

//==========================================================================
// Class: ConfigurationInfo
//==========================================================================
class ConfigurationInfo {

public:
    static ConfigurationInfo & data();

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

    // File naming
    std::string                         parsing_regex;
    std::string                         parsing_assign;
    std::string                         product_id_tpl;
    std::string                         data_ext;
    std::string                         meta_ext;
    std::string                         log_ext;

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

    void clear();

private:
    ConfigurationInfo();

};

const std::string ExternalSpace("EXTERNAL");
const std::string UserSpace("USER");
const std::string InboxSpace("INBOX");
const std::string LocalArchSpace("LOCAL_ARCHIVE");
const std::string SharedSpace("SHARED");
const std::string ProcessingSpace("PROCESSING");

const std::set<std::string> URLSpaces
{ ExternalSpace, UserSpace, InboxSpace, LocalArchSpace, SharedSpace, ProcessingSpace };

}

#endif  /* CFGINFO_H */
