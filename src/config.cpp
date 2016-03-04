/******************************************************************************
 * File:    config.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.config
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
 *   Implement Configuration class
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

#include <cstdlib>

#include "config.h"

#include "evtmng.h"
#include "datamng.h"
#include "logmng.h"
#include "taskmng.h"
#include "taskorc.h"
#include "taskagent.h"
#include "tools.h"

#include "dbhdlpostgre.h"
#include "except.h"
#include "log.h"
using namespace LibComm;

#include <sys/time.h>
#include <fstream>
#include <iostream>

#define WRITE_MESSAGE_FILES

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
Configuration::Configuration(std::string fName)
{
    if (! fName.empty()) {
        applyNewConfig(fName);
    } else {
        readConfigurationFromDB();
    }
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Configuration::Configuration(const char * fName)
{
    if (fName != 0) {
        applyNewConfig(std::string(fName));
    } else {
        readConfigurationFromDB();
    }
}

//----------------------------------------------------------------------
// Method: getCfgInfo
// Get entire configuration info structure
//----------------------------------------------------------------------
ConfigurationInfo & Configuration::getCfgInfo()
{
    return cfgInfo;
}

//----------------------------------------------------------------------
// Method: getGeneralInfo
// Get general parameters from the configuration
//----------------------------------------------------------------------
void Configuration::getGeneralInfo(std::string & appName, std::string appVer, std::string & last)
{
    appName = cfg["general"]["app_name"].asString();
    appVer  = cfg["general"]["app_version"].asString();
    last    = cfg["general"]["last_access"].asString();

    PATHRun = PATHBase + "/" + LibComm::timeTag();
    PATHLog = PATHRun + "/log";
    PATHTmp = PATHRun + "/tmp";
    PATHTsk = PATHRun + "/tsk";
    PATHMsg = PATHRun + "/msg";
}
    
//----------------------------------------------------------------------
// Method: setLastAccess
// Updates the date of last access to the configuration
//----------------------------------------------------------------------
void Configuration::setLastAccess(std::string last)
{
    cfg["general"]["last_access"] = last;
}

//----------------------------------------------------------------------
// Method: reset
// Restarts iterators to begin of containers
//----------------------------------------------------------------------
void Configuration::reset()
{
    ruleIt = cfg["orchestration"]["rules"].begin();
    procIt = cfg["processing"]["processors"].begin();
    nodeIt = cfg["nodes"]["node_list"].begin();
    machIt = cfg["machines"].begin();
}

//----------------------------------------------------------------------
// Method: getProductTypes
// Return vector of product type identifiers
//----------------------------------------------------------------------
void Configuration::getProductTypes(std::vector<std::string> & vec)
{
    vec.clear();
    Json::Value pTypes = cfg["products"]["product_types"];
    for (unsigned int i = 0; i < pTypes.size(); ++i) {
        vec.push_back(pTypes[i].asString());
    }
}

//----------------------------------------------------------------------
// Method: getNumOrchRules
// Return number of orchestration rules
//----------------------------------------------------------------------
int Configuration::getNumOrchRules()
{
    return cfg["orchestration"]["rules"].size();
}

//----------------------------------------------------------------------
// Method: getOrchRule
// Return orchestration rule parameters
//----------------------------------------------------------------------
void Configuration::getOrchRule(std::string & name, std::vector<std::string> & in,
                                std::vector<std::string> & out, std::string & pElem)
{
    Json::Value const & v = (*ruleIt);
    name  = ruleIt.key().asString();
    in    = LibComm::split(v["inputs"].asString(), ',');
    out   = LibComm::split(v["outputs"].asString(), ',');
    pElem = v["processing"].asString();

    ruleIt++;
    if (ruleIt == cfg["orchestration"]["rules"].end()) {
        ruleIt = cfg["orchestration"]["rules"].begin();
    }
}

//----------------------------------------------------------------------
// Method: getNumProcs
// Return number of processors
//----------------------------------------------------------------------
int Configuration::getNumProcs()
{
    return cfg["processing"]["processors"].size();
}

//----------------------------------------------------------------------
// Method: getProc
// Return processor parameters
//----------------------------------------------------------------------
void Configuration::getProc(std::string & name, std::string & exe,
                            std::string & in, std::string & out)
{
    Json::Value const & v = (*procIt);
    name = v["name"].asString();
    exe  = v["exe_path"].asString();
    in   = v["input_path"].asString();
    out  = v["output_path"].asString();

    procIt++;
    if (procIt == cfg["processing"]["processors"].end()) {
        procIt = cfg["processing"]["processors"].begin();
    }
}
    
//----------------------------------------------------------------------
// Method: getNumNodes
// Return number of nodes
//----------------------------------------------------------------------
int Configuration::getNumNodes()
{
    return cfg["nodes"]["node_list"].size();
}

//----------------------------------------------------------------------
// Method: getNode
// Return node parameters
//----------------------------------------------------------------------
void Configuration::getNode(std::string & name, std::string & type,
                            std::string & cAddr, std::string & sAddr)
{
    Json::Value const & v = (*nodeIt);
    name  = nodeIt.key().asString();
    type  = v["type"].asString();
    cAddr = v["client"].asString();
    sAddr = v["server"].asString();

    nodeIt++;
    if (nodeIt == cfg["nodes"]["node_list"].end()) {
        nodeIt = cfg["nodes"]["node_list"].begin();
    }
}
    
//----------------------------------------------------------------------
// Method: getNodeByName
// Return node parameters for a node with the name specified
//----------------------------------------------------------------------
void Configuration::getNodeByName(std::string name, std::string & type,
                                  std::string & cAddr, std::string & sAddr)
{
    Json::Value & v = cfg["nodes"]["node_list"][name];
    type  = v["type"].asString();
    cAddr = v["client"].asString();
    sAddr = v["server"].asString();
}
    
//----------------------------------------------------------------------
// Method: getHMINodeName
// Return HMI node name
//----------------------------------------------------------------------
std::string Configuration::getHMINodeName()
{
    return cfg["nodes"]["hmi_node"].asString();
}

//----------------------------------------------------------------------
// Method: getNumMachines
// Return number of machines in the network
//----------------------------------------------------------------------
int Configuration::getNumMachines()
{
    return cfg["machines"].size();
}

//----------------------------------------------------------------------
// Method: getMachine
// Return list of nodes to be deployed in a machine
//----------------------------------------------------------------------
void Configuration::getMachine(std::string & machine,
                               std::vector<std::string> & vec)
{
    Json::Value const & v = (*machIt);
    machine = machIt.key().asString();

    vec.clear();
    const Json::Value & nodesInMachine = v;
    for (unsigned int i = 0; i < nodesInMachine.size(); ++i) {
        vec.push_back(nodesInMachine[i].asString());
    }

    machIt++;
    if (machIt == cfg["machines"].end()) {
        machIt = cfg["machines"].begin();
    }
}

//----------------------------------------------------------------------
// Method: getConnectionsForNode
// Return list of nodes connecting to a given one
//----------------------------------------------------------------------
void Configuration::getConnectionsForNode(std::string nodeName,
                                          std::vector<std::string> & vec)
{
    vec.clear();
    const Json::Value & nodesToConnect = cfg["connections"][nodeName];
    for (unsigned int i = 0; i < nodesToConnect.size(); ++i) {
        vec.push_back(nodesToConnect[i].asString());
    }
}

//----------------------------------------------------------------------
// Method: applyNewConfig
// Get new configuration information from an external (JSON) file
//----------------------------------------------------------------------
void Configuration::applyNewConfig(std::string fName)
{
    setConfigFile(fName);
    readConfigurationFromFile();
    //saveConfigurationToDB();
}

//----------------------------------------------------------------------
// Method: setConfigFile
// Set configuration file name
//----------------------------------------------------------------------
void Configuration::setConfigFile(std::string fName)
{
    cfgFileName = fName;
}

//----------------------------------------------------------------------
// Method: readConfigurationFromFile
// Loads the configuration file content into memory
//----------------------------------------------------------------------
void Configuration::readConfigurationFromFile()
{
    std::ifstream cfgJsonFile(cfgFileName);
    if (!cfgJsonFile.good()) {
        cfgJsonFile.close();
        return;
    }
    Json::Reader reader;
    reader.parse(cfgJsonFile, cfg);
    cfgJsonFile.close();

    // Now, fill in ConfigurationInfo structure
    reset();
    cfgInfo.clear();
    cfgInfo.hmiPresent = false;

    // Configuration file name
    cfgInfo.currentMachine = getEnvVar("HOSTNAME");
    cfgInfo.currentUser    = getEnvVar("USER");
    cfgInfo.cfgFileName    = cfgFileName;

    // General
    getGeneralInfo(cfgInfo.appName, cfgInfo.appVersion, cfgInfo.lastAccess);

    // Product types
    getProductTypes(cfgInfo.orcParams.productTypes);

    // Orchestration rulesfile
    for (int i = 0; i < getNumOrchRules(); ++i) {
        Rule * rule = new Rule;
        getOrchRule(rule->name, rule->inputs,
                    rule->outputs, rule->processingElement);
        cfgInfo.orcParams.rules.push_back(rule);
    }

    // Processors
    for (int i = 0; i < getNumProcs(); ++i) {
        Processor * pe = new Processor;
        getProc(pe->name, pe->exePath, pe->inPath, pe->outPath);
        cfgInfo.orcParams.processors[pe->name] = pe;
    }

    // Nodes
    for (int i = 0; i < getNumNodes(); ++i) {
        Peer * peer = new Peer;
        getNode(peer->name, peer->type, peer->clientAddr, peer->serverAddr);
        cfgInfo.peersCfg.push_back(*peer);
        cfgInfo.peerNames.push_back(peer->name);
        cfgInfo.peersCfgByName[peer->name] = peer;
        if (peer->type == "evtmng") {
            cfgInfo.evtMngCfg.name = peer->name;
            cfgInfo.evtMngCfg.type = peer->name;
            cfgInfo.evtMngCfg.clientAddr = peer->clientAddr;
            cfgInfo.evtMngCfg.serverAddr = peer->serverAddr;
        }
    }

    cfgInfo.qpfhmiCfg.name = getHMINodeName();
    getNodeByName(cfgInfo.qpfhmiCfg.name,
                  cfgInfo.qpfhmiCfg.type,
                  cfgInfo.qpfhmiCfg.clientAddr,
                  cfgInfo.qpfhmiCfg.serverAddr);

    // Machines and connections
    reset();
    std::string mname;
    std::vector<std::string> mnodes;
    for (int i = 0; i < getNumMachines(); ++i) {
        mnodes.clear();
        getMachine(mname, mnodes);
        cfgInfo.machines.push_back(mname);
        cfgInfo.machineNodes[mname] = mnodes;
    }

    for (unsigned int i = 0; i < cfgInfo.peerNames.size(); ++i) {
        std::vector<std::string> nconn;
        getConnectionsForNode(cfgInfo.peerNames.at(i), nconn);
        cfgInfo.connections[cfgInfo.peerNames.at(i)] = nconn;
    }

    // Create peer commnodes for nodes in current machine
    std::vector<std::string> & machineNodes =
            cfgInfo.machineNodes[cfgInfo.currentMachine];

    for (unsigned int i = 0; i < machineNodes.size(); ++i) {

        Peer * peer = cfgInfo.peersCfgByName[machineNodes.at(i)];
        std::string & peerName = peer->name;
        std::string & peerType = peer->type;
        const char * cpeerName = peerName.c_str();

        CommNode * commNode = 0;

        if        (peerType == "evtmng") {
            commNode = new EventManager(cpeerName);
        } else if (peerType == "hmi") {
            cfgInfo.hmiPresent = true;
        } else if (peerType == "datamng") {
            commNode = new DataManager(cpeerName);
        } else if (peerType == "logmng") {
            commNode = new LogManager(cpeerName);
        } else if (peerType == "taskmng") {
            commNode = new TaskManager(cpeerName);
        } else if (peerType == "taskorc") {
            commNode = new TaskOrchestrator(cpeerName);
        } else if (peerType == "taskagent") {
            commNode = new TaskAgent(cpeerName);
            cfgInfo.peerAgents.push_back(commNode);
        } else {
            // Do nothing, not yet implemented
        }

        if (commNode == 0) { continue; }

        commNode->addPeer(cfgInfo.peersCfgByName[peerName], true);
        std::cerr << "Creating connections for " << peerName
                  << "  [" << peer->clientAddr
                  << " ; " << peer->serverAddr << "]\n";

        std::vector<std::string> & connectNodes = cfgInfo.connections[peerName];

        for (unsigned int j = 0; j < connectNodes.size(); ++j) {
            Peer * otherPeer = cfgInfo.peersCfgByName[connectNodes.at(j)];
            commNode->addPeer(otherPeer);
            std::cerr << "  Connecting to " << otherPeer->name
                      << "  [" << otherPeer->clientAddr
                      << " ; " << otherPeer->serverAddr << "]\n";
        }

        cfgInfo.peerNodes.push_back(commNode);

    }

}

//----------------------------------------------------------------------
// Method: readConfigurationFromDB
// Loads the configuration from the DB into memory
//----------------------------------------------------------------------
void Configuration::readConfigurationFromDB()
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    dbHdl->setDbHost(Configuration::DBHost);
    dbHdl->setDbPort(Configuration::DBPort);
    dbHdl->setDbName(Configuration::DBName);
    dbHdl->setDbUser(Configuration::DBUser);
    dbHdl->setDbPasswd(Configuration::DBPwd);

    // Check that connection with the DB is possible
    try {
        dbHdl->openConnection();
    } catch (RuntimeException & e) {
        Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    }

    // Try to retrieve the configuration from the DB
    std::vector< std::vector<std::string> > config_general;
    std::vector< std::vector<std::string> > config_products;
    std::vector< std::vector<std::string> > config_orchestration;
    std::vector< std::vector<std::string> > config_processors;
    std::vector< std::vector<std::string> > config_nodes;

    dbHdl->getTable("config_general", config_general);
    dbHdl->getTable("config_products", config_products);
    dbHdl->getQuery("select o.rule_name, o.inputs, o.outputs, p.processor_name "
                    "from config_orchestration as o "
                    "inner join config_processors as p "
                    "on o.processor_id = p.processor_id;", config_orchestration);
    dbHdl->getTable("config_processors", config_processors);
    dbHdl->getTable("config_nodes", config_nodes);

    // Transfer DB config info to JSON value
    cfg.clear();

    // 1. config_general
    for (unsigned int i = 0; i < config_general.size(); ++i) {
        std::string & name = config_general.at(i).at(0);
        std::string & content = config_general.at(i).at(1);
        cfg["general"][name] = content;
    }

    // 2. config_products
    for (unsigned int i = 0; i < config_products.size(); ++i) {
        cfg["products"]["product_types"].append(config_products.at(i).at(0));
    }

    // 3. config_processors
    for (unsigned int i = 0; i < config_processors.size(); ++i) {
        Json::Value proc;
        proc["name"] = config_processors.at(i).at(1);
        proc["exe_path"] = config_processors.at(i).at(2);
        proc["input_path"] = config_processors.at(i).at(3);
        proc["output_path"] = config_processors.at(i).at(4);
        cfg["processing"]["processors"][config_processors.at(i).at(1)] = proc;
    }

    // 4. config_orchestration
    for (unsigned int i = 0; i < config_orchestration.size(); ++i) {
        Json::Value rule;
        rule["inputs"] = config_orchestration.at(i).at(1);
        rule["outputs"] = config_orchestration.at(i).at(2);
        rule["processing"] = config_orchestration.at(i).at(3);
        cfg["orchestration"]["rules"][config_orchestration.at(i).at(0)] = rule;
    }

    // 5. config_nodes
    for (unsigned int i = 0; i < config_nodes.size(); ++i) {
        Json::Value node;
        node["type"] = config_nodes.at(i).at(1);
        node["client"] = config_nodes.at(i).at(2);
        node["server"] = config_nodes.at(i).at(3);
        cfg["nodes"]["node_list"][config_nodes.at(i).at(0)] = node;
    }
    cfg["nodes"]["hmi_node"] = cfg["general"]["hmi_node"];

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: saveConfigurationToDB
// Store the configuration into the DB
//----------------------------------------------------------------------
void Configuration::saveConfigurationToDB()
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    dbHdl->setDbHost(Configuration::DBHost);
    dbHdl->setDbPort(Configuration::DBPort);
    dbHdl->setDbName(Configuration::DBName);
    dbHdl->setDbUser(Configuration::DBUser);
    dbHdl->setDbPasswd(Configuration::DBPwd);

    // Check that connection with the DB is possible
    try {
        dbHdl->openConnection();
    } catch (RuntimeException & e) {
        LibComm::Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    }

    // Try to retrieve the configuration from the DB
    std::vector< std::vector<std::string> > config_general;
    std::vector< std::vector<std::string> > config_products;
    std::vector< std::vector<std::string> > config_orchestration;
    std::vector< std::vector<std::string> > config_processors;
    std::vector< std::vector<std::string> > config_nodes;

//    dbHdl->getTable("config_general", config_general);
//    dbHdl->getTable("config_products", config_products);
//    dbHdl->getQuery("select o.rulename, o.inputs, o.outputs, p.processorname "
//                    "from config_orchestration as o "
//                    "inner join config_processors as p "
//                    "on o.processorid = p.processorid;", config_orchestration);
//    dbHdl->getTable("config_processors", config_processors);
//    dbHdl->getTable("config_nodes", config_nodes);

    // Transfer config from JSON value to DB
    Json::Value::iterator it;
    std::string cmd;
    std::map<std::string, int> procs;

    // 0. clear config tables
    cmd = "TRUNCATE TABLE config_general, config_products, config_processors, "
          "config_orchestration, config_nodes CASCADE";
    dbHdl->runCmd(cmd);

    // 1. config_general
    Json::Value r = cfg["general"];
    cmd = "INSERT INTO config_general (param_name, content) VALUES ";
    for (it = r.begin(); it != r.end(); ++it) {
        cmd += "('" + it.key().asString() + "','" + (*it).asString() + "'),";
    }
    cmd += "('hmi_node','" + cfg["nodes"]["hmi_node"].asString() + "')";
    dbHdl->runCmd(cmd);

    // 2. config_products
    r = cfg["products"]["product_types"];
    cmd = "INSERT INTO config_products (product_type) VALUES ";
    for (unsigned int i = 0; i < r.size(); ++i) {
        cmd += "('" + r[i].asString() + "'),";
    }
    cmd.erase(cmd.end() - 1);
    dbHdl->runCmd(cmd);

    // 3. config_processors
    r = cfg["processing"]["processors"];
    cmd = "INSERT INTO config_processors (processor_id, processor_name, "
          "exe_path, input_path, output_path) VALUES ";
    int k = 0;
    for (it = r.begin(); it != r.end(); ++it, ++k) {
        cmd += "(" + LibComm::toStr<int>(k) + ",'" +
               (*it)["name"].asString() + "','" +
                (*it)["exe_path"].asString() + "', '" +
                (*it)["input_path"].asString() + "', '"+
                (*it)["output_path"].asString() + "'),";
        procs[(*it)["name"].asString()] = k;
    }
    cmd.erase(cmd.end() - 1);
    dbHdl->runCmd(cmd);

    // 4. config_orchestration
    r = cfg["orchestration"]["rules"];
    cmd = "INSERT INTO config_orchestration (rule_id, rule_name, "
          "inputs, outputs, processor_id) VALUES ";
    k = 0;
    for (it = r.begin(); it != r.end(); ++it, ++k) {
        cmd += "(" + LibComm::toStr<int>(k) + ",'" +
               it.key().asString() + "','" +
               (*it)["inputs"].asString() + "', '" +
                (*it)["outputs"].asString() + "', "+
                LibComm::toStr<int>(procs[(*it)["processing"].asString()]) + "),";
    }
    cmd.erase(cmd.end() - 1);
    dbHdl->runCmd(cmd);

    // 5. config_nodes
    r = cfg["nodes"]["node_list"];
    cmd = "INSERT INTO config_nodes (node_name, node_type, "
          "client_addr, server_addr) VALUES ";
    for (it = r.begin(); it != r.end(); ++it) {
        cmd += "('" + it.key().asString() + "','" +
                (*it)["type"].asString() + "', '" +
                (*it)["client"].asString() + "', '"+
                (*it)["server"].asString() + "'),";
    }
    cmd.erase(cmd.end() - 1);
    dbHdl->runCmd(cmd);

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: getEnvVar
// Returns the content of an environment variable
//----------------------------------------------------------------------
std::string Configuration::getEnvVar(std::string const & key) const
{
    char * val = std::getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

std::string Configuration::DBHost("127.0.0.1"); // localhost
std::string Configuration::DBPort("5432");
std::string Configuration::DBName("qpfdb");
std::string Configuration::DBUser("jcgonzalez");
std::string Configuration::DBPwd("euclidjcg");

std::string Configuration::PATHBase("/var/run/qpf");
std::string Configuration::PATHRun;
std::string Configuration::PATHLog;
std::string Configuration::PATHTmp;
std::string Configuration::PATHTsk;
std::string Configuration::PATHMsg;

mode_t Configuration::PATHMode = 0755;

}


