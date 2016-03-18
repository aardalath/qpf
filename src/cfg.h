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
 *   Declare configuration related datatypes
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
#include "datatypes.h"
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
    std::string                         condition;
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

enum LocalArchiveMethod { LINK, MOVE, COPY, REMOTE_COPY, SYMLINK };

struct StorageExternal {
    std::string protocol;
    std::string address;
    std::string port;
    std::string user;
    std::string passwd;
    std::string path;
};

struct StorageLocal {
    std::string path;
};

struct StorageConfig {
    std::string       base;
    std::string       tasks;
    StorageExternal   inbox;
    StorageLocal      local_archive;
    StorageExternal   archive;
    StorageLocal      gateway;
    StorageExternal   outbox;
};

}

#endif  /* CFG_H */
