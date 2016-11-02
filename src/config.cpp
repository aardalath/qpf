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

#include "dbg.h"
#include "str.h"

#include "evtmng.h"
#include "datamng.h"
#include "logmng.h"
#include "taskmng.h"
#include "taskorc.h"
#include "taskagent.h"
#include "filenamespec.h"

#include "tools.h"

#include "dbhdlpostgre.h"
#include "except.h"
#include "log.h"
using namespace LibComm;

#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <libgen.h>

#define WRITE_MESSAGE_FILES

//static const std::string QPFExecutionsBase("/qpf/run/");

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
    init(fName);
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Configuration::Configuration(const char * fName)
{
    init(std::string(fName));
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
void Configuration::init(std::string fName)
{
    std::cerr << "Provided fName='" << fName << "'" << std::endl;
    if (fName.compare(0,5,"db://") == 0) {
        std::cerr << "A database URL!" << std::endl;

        // fName in fact is a db url in the form:
        //   db://user:pwd@host:port/dbname
        // So, take the fields
        std::string url = fName.substr(5);
        DBUser = url.substr(0, url.find(":")); url.erase(0, url.find(":") + 1); // take username
        DBPwd  = url.substr(0, url.find("@")); url.erase(0, url.find("@") + 1); // take password
        DBHost = url.substr(0, url.find(":")); url.erase(0, url.find(":") + 1); // take host
        DBPort = url.substr(0, url.find("/")); url.erase(0, url.find("/") + 1); // take port
        DBName = url; // take database name

        std::cerr << DBUser << ", "
                  << DBPwd  << ", "
                  << DBHost << ", "
                  << DBPort << ", "
                  << DBName << std::endl;
        fName = ""; // clear filename, to read from DB
    }
    if (! fName.empty()) {
        applyNewConfig(fName);
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
    return ConfigurationInfo::data();
}

//----------------------------------------------------------------------
// Method: getGeneralInfo
// Get general parameters from the configuration
//----------------------------------------------------------------------
void Configuration::getGeneralInfo(std::string & appName, std::string & appVer, std::string & last)
{
    appName = cfg["general"]["app_name"].asString();
    appVer  = cfg["general"]["app_version"].asString();
    last    = cfg["general"]["last_access"].asString();

    DBHost = cfg["db"]["host"].asString();
    DBPort = cfg["db"]["port"].asString();
    DBName = cfg["db"]["name"].asString();
    DBUser = cfg["db"]["user"].asString();
    DBPwd  = cfg["db"]["pwd"].asString();

    PATHBase = cfg["storage"]["base"]["path"].asString();
    PATHRun  = cfg["storage"]["run"]["path"].asString();

    PATHBin     = PATHRun + "/bin";
    PATHSession = PATHRun + "/" + LibComm::sessionTag();
    PATHLog     = PATHSession + "/log";
    PATHRlog    = PATHSession + "/rlog";
    PATHTmp     = PATHSession + "/tmp";
    PATHTsk     = PATHSession + "/tsk";
    PATHMsg     = PATHSession + "/msg";
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
    toolIt = cfg["userdeftools"].begin();
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
                                std::vector<std::string> & out, std::string & pElem,
                                std::string & condition)
{
    Json::Value const & v = (*ruleIt);
    name  = ruleIt.key().asString();
    in    = str::split(v["inputs"].asString(), ',');
    out   = str::split(v["outputs"].asString(), ',');
    pElem = v["processing"].asString();
    condition = v["condition"].asString();

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
// Method: getNumUserDefTools
// Return number of user defined tools
//----------------------------------------------------------------------
int Configuration::getNumUserDefTools()
{
    return cfg["userdeftools"].size();
}

//----------------------------------------------------------------------
// Method: getNode
// Return node parameters
//----------------------------------------------------------------------
void Configuration::getUserDefTool(UserDefTool & t)
{
    Json::Value const & v = (*toolIt);
    t.name = v["name"].asString();
    t.desc = v["description"].asString();
    t.exe  = v["executable"].asString();
    t.args = v["arguments"].asString();

    t.prod_types.clear();
    Json::Value pTypes = v["product_types"];
    for (unsigned int i = 0; i < pTypes.size(); ++i) {
        t.prod_types.push_back(pTypes[i].asString());
    }

    toolIt++;
    if (toolIt == cfg["nodes"]["node_list"].end()) {
        toolIt = cfg["nodes"]["node_list"].begin();
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
    saveConfigurationToDB();
}

//----------------------------------------------------------------------
// Method: setConfigFile
// Set configuration file name
//----------------------------------------------------------------------
void Configuration::setConfigFile(std::string fName)
{
    cfgFileName = fName;
    cfgFilePath = std::string(dirname(strdup(fName.c_str())));
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

    processConfiguration();
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
        LibComm::Log::log("SYSTEM", Log::FATAL, e.what());
        return;
    }

    // Try to retrieve the configuration from the DB
    std::vector< std::vector<std::string> > configuration;
    std::string dateCreated;

    try {
        dbHdl->getTable("configuration", configuration);

        // Transfer DB config info to JSON value
        cfg.clear();
        unsigned int lastConfiguration = configuration.size() - 1;
        Json::Reader reader;
        reader.parse(configuration.at(lastConfiguration).at(1), cfg);
        dateCreated = configuration.at(lastConfiguration).at(0);
        cfgFileName = "<internalDB> " + Configuration::DBName + "::configuration";
    } catch (RuntimeException & e) {
        LibComm::Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        LibComm::Log::log("SYSTEM", Log::ERROR,
                          "Unexpected error accessing "
                          "database for retrieval of system configuration");
        return;
    }

    // Modificar fecha de último accesso
    std::string now = LibComm::timeTag();
    std::string cmd("UPDATE configuration SET last_accessed = '" + now + "' "
                    "WHERE created='" + dateCreated + "'");

    try {
        dbHdl->runCmd(cmd);
    } catch (RuntimeException & e) {
        LibComm::Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        LibComm::Log::log("SYSTEM", Log::ERROR,
                          "Unexpected error accessing "
                          "database for retrieval of system configuration");
        return;
    }

    // Close connection
    dbHdl->closeConnection();

    processConfiguration();
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
        LibComm::Log::log("SYSTEM", Log::FATAL, e.what());
        return;
    }

    // Modify cfg to store
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();
    Json::Value dbcfg(cfg);
    dbcfg["products"]["parsing_regex"] = cfgInfo.parsing_regex;

    // Transfer config from JSON value to DB
    std::string cmd;
    std::string now = LibComm::timeTag();
    Json::FastWriter writer;
    std::string cfgString = writer.write(dbcfg);

    cmd = "INSERT INTO configuration (created, last_accessed, cfg) VALUES ";
    cmd += "('" + now + "', '" + now + "', '" + cfgString + "')";

    try {
         dbHdl->runCmd(cmd);
    } catch (RuntimeException & e) {
        LibComm::Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        LibComm::Log::log("SYSTEM", Log::ERROR,
                          "Unexpected error accessing "
                          "database for storage of system configuration");
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: getRegExFromCfg
// Retrieves filename regex from cfg or from designated file
//----------------------------------------------------------------------
std::string Configuration::getRegExFromCfg(std::string & regexStr)
{
    // Regex for parsing file names might be in a separate file pointed by
    // parsing_regex parameter if the first character is '@'
    if (regexStr.at(0) == '@') {
        std::ifstream parsingReFile;
        parsingReFile.open(cfgFilePath + "/" + regexStr.substr(1),
                           std::ifstream::in);
        if (parsingReFile.good()) {
            std::string fileLine;
            do {
                std::getline(parsingReFile, fileLine);
            } while ((fileLine.length() < 1) ||
                    ((fileLine.at(0) == '#') ||
                    (fileLine.at(0) == '%') ||
                    (fileLine.at(0) == '\'') ||
                    (fileLine.at(0) == ':') ||
                    (fileLine.at(0) == '\t') ||
                    (fileLine.at(0) == ' ') ||
                    (fileLine.at(0) == '!')));
            parsingReFile.close();
            return fileLine;
        }
    } else {
        return regexStr;
    }
}

//----------------------------------------------------------------------
// Method: createNewComponent
// Create the appropriate component according to the peer type
//----------------------------------------------------------------------
Component * Configuration::createNewComponent(ConfigurationInfo & cfgInfo,
                                              std::string & peerType,
                                              const char * cpeerName)
{
    Component * component = 0;

    if        (peerType == "evtmng") {
        component = new EventManager(cpeerName);
    } else if (peerType == "hmi") {
        cfgInfo.hmiPresent = true;
    } else if (peerType == "datamng") {
        component = new DataManager(cpeerName);
    } else if (peerType == "logmng") {
        component = new LogManager(cpeerName);
    } else if (peerType == "taskmng") {
        component = new TaskManager(cpeerName);
    } else if (peerType == "taskorc") {
        component = new TaskOrchestrator(cpeerName);
    } else if (peerType == "taskagent") {
        TaskAgent * ag = new TaskAgent(cpeerName);
        ag->setSysDir(Configuration::PATHRun);
        ag->setWorkDir(Configuration::PATHTsk);
        ag->setAgentAddress(cfgInfo.currentUser + "@" + cfgInfo.currentMachine);
        ag->setMasterAddress(cfgInfo.currentUser + "@" + cfgInfo.masterMachine);
        ag->setRemote(!cfgInfo.isMaster);
        component = ag;
        cfgInfo.peerAgents.push_back(component);
    } else {
        // Do nothing, not yet implemented
    }

    return component;
}

//----------------------------------------------------------------------
// Method: processConfiguration
// Loads the configuration file content into memory
//----------------------------------------------------------------------
void Configuration::processConfiguration()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    Json::StyledWriter w;

    // START OF: Configuration Reading

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

    // Product datatypes
    getProductTypes(cfgInfo.orcParams.productTypes);

    // File name convention parameters
    const Json::Value & prds = cfg["products"];
    cfgInfo.parsing_assign   = prds["parsing_assign"].asString();
    cfgInfo.product_id_tpl   = prds["product_id_tpl"].asString();
    cfgInfo.data_ext         = prds["data_ext"].asString();
    cfgInfo.meta_ext         = prds["meta_ext"].asString();
    cfgInfo.log_ext          = prds["log_ext"].asString();

    std::string parsing_regex_str = prds["parsing_regex"].asString();
    cfgInfo.parsing_regex    = getRegExFromCfg(parsing_regex_str);

    FileNameSpec fs(cfgInfo.parsing_regex, cfgInfo.parsing_assign);
    fs.setProductIdTpl(cfgInfo.product_id_tpl);

    // Orchestration rules file
    for (int i = 0; i < getNumOrchRules(); ++i) {
        Rule * rule = new Rule;
        getOrchRule(rule->name, rule->inputs,
                    rule->outputs, rule->processingElement,
                    rule->condition);
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
            cfgInfo.evtMngCfg.type = peer->type;
            cfgInfo.evtMngCfg.clientAddr = peer->clientAddr;
            cfgInfo.evtMngCfg.serverAddr = peer->serverAddr;
        }
    }

    // HMI node
    cfgInfo.qpfhmiCfg.name = getHMINodeName();
    getNodeByName(cfgInfo.qpfhmiCfg.name,
                  cfgInfo.qpfhmiCfg.type,
                  cfgInfo.qpfhmiCfg.clientAddr,
                  cfgInfo.qpfhmiCfg.serverAddr);

    // Master node
    cfgInfo.masterMachine = cfg["nodes"]["master_machine"].asString();
    cfgInfo.isMaster = (cfgInfo.masterMachine == cfgInfo.currentMachine);

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

    // Storage areas information
    const Json::Value & stge             = cfg["storage"];
    const Json::Value & stgeBase         = stge["base"];
    const Json::Value & stgeRun          = stge["run"];
    const Json::Value & stgeIn           = stge["incoming"];
    const Json::Value & stgeLocal        = stge["local_archive"];
    const Json::Value & stgeGatew        = stge["gateway"];
    const Json::Value & stgeArch         = stge["archive"];
    const Json::Value & stgeOut          = stge["outgoing"];

    cfgInfo.storage.base                 = stgeBase["path"].asString();
    cfgInfo.storage.run                  = stgeRun["path"].asString();
    cfgInfo.storage.local_archive.path   = stgeLocal["path"].asString();
    cfgInfo.storage.gateway.path         = stgeGatew["path"].asString();

    cfgInfo.storage.tasks                = PATHTsk;

    getExternalStorage(stgeIn,   cfgInfo.storage.inbox);
    getExternalStorage(stgeOut,  cfgInfo.storage.outbox);
    getExternalStorage(stgeArch, cfgInfo.storage.archive);

    // User Defined Tools
    for (int i = 0; i < getNumUserDefTools(); ++i) {
        UserDefTool udt;
        getUserDefTool(udt);
        cfgInfo.userDefTools[udt.name] = udt;
    }

    // Flags
    const Json::Value & flags = cfg["flags"];
    const Json::Value & monitFlags = flags["monitoring"];
    const Json::Value & procFlags  = flags["processing"];
    const Json::Value & archFlags  = flags["archiving"];

    Json::Value::iterator it = monitFlags["msgs_to_disk"].begin();
    while (it != monitFlags["msgs_to_disk"].end()) {
        Json::Value const & v = (*it);
        std::string msgName = v.asString();
        cfgInfo.flags.monit.msgsToDisk[msgName] = true;
        ++it;
    }
    it = monitFlags["msgs_to_db"].begin();
    while (it != monitFlags["msgs_to_db"].end()) {
        Json::Value const & v = (*it);
        std::string msgName = v.asString();
        cfgInfo.flags.monit.msgsToDB[msgName] = true;
        ++it;
    }
    cfgInfo.flags.monit.notifyMsgArrival         = monitFlags["notify_msg_arrival"].asBool();
    cfgInfo.flags.monit.groupTaskAgentLogs       = monitFlags["group_task_agent_logs"].asBool();

    cfgInfo.flags.proc.allowReprocessing         = procFlags["allow_reprocessing"].asBool();
    cfgInfo.flags.proc.allowReprocessing         = procFlags["intermediate_products"].asBool();

    cfgInfo.flags.arch.sendOutputsToMainArchive  = archFlags["send_outputs_to_main_archive"].asBool();

    // END OF: Configuration Reading

    // Create peer commnodes for nodes in current machine
    std::vector<std::string> & machineNodes =
            cfgInfo.machineNodes[cfgInfo.currentMachine];

    for (unsigned int i = 0; i < machineNodes.size(); ++i) {

        Peer * peer = cfgInfo.peersCfgByName[machineNodes.at(i)];
        std::string & peerName = peer->name;
        std::string & peerType = peer->type;

        Component * component = createNewComponent(cfgInfo, peerType, peerName.c_str());
        if (component == 0) { continue; }

        component->addPeer(cfgInfo.peersCfgByName[peerName], true);
        DBG("Creating connections for " << peerName
            << "  [" << peer->clientAddr
            << " ; " << peer->serverAddr << "]");

        std::vector<std::string> & connectNodes = cfgInfo.connections[peerName];

        for (unsigned int j = 0; j < connectNodes.size(); ++j) {
            Peer * otherPeer = cfgInfo.peersCfgByName[connectNodes.at(j)];
            component->addPeer(otherPeer);
            DBG("  Connecting to " << otherPeer->name
                << "  [" << otherPeer->clientAddr
                << " ; " << otherPeer->serverAddr << "]");
        }

        cfgInfo.peerNodes.push_back(component);

    }

}

//----------------------------------------------------------------------
// Method: getExternalStorage
// Stores the values of a StorageExternal strucure from configuration
// into the appropriate data structure
//----------------------------------------------------------------------
void Configuration::getExternalStorage(const Json::Value & section,
                                       StorageExternal & ext)
{
    ext.protocol          = section["protocol"].asString();
    ext.address           = section["address"].asString();
    ext.port              = section["port"].asString();
    ext.user              = section["user"].asString();
    ext.passwd            = section["password"].asString();
    ext.path              = section["path"].asString();
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

//----------------------------------------------------------------------
// Method: isLE1Product
// Returns TRUE if the product type corresponds to a LE1 product file
//----------------------------------------------------------------------
bool isLE1Product(std::string const & p)
{
    return ((p == "VIS_LE1") ||
            (p == "NIR_LE1") ||
            (p == "SIR_LE1"));
}

//----------------------------------------------------------------------
// Method: isLE1Metadata
// Returns TRUE if the product type corresponds to a LE1 metadata file
//----------------------------------------------------------------------
bool isLE1Metadata(std::string const & p)
{
    return ((p == "VIS_LE1_META") ||
            (p == "NIR_LE1_META") ||
            (p == "SIR_LE1_META"));
}

//----------------------------------------------------------------------
// Method: isValidExtension
// Returns TRUE if the file extension provided is one of the valid ones
//----------------------------------------------------------------------
bool isValidExtension(std::string const & e)
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    return ((e == cfgInfo.data_ext) || (e == cfgInfo.meta_ext));
}

std::string Configuration::DBHost;
std::string Configuration::DBPort;
std::string Configuration::DBName;
std::string Configuration::DBUser;
std::string Configuration::DBPwd;

std::string Configuration::PATHBase;
std::string Configuration::PATHRun;
std::string Configuration::PATHBin;
std::string Configuration::PATHSession;
std::string Configuration::PATHLog;
std::string Configuration::PATHRlog;
std::string Configuration::PATHTmp;
std::string Configuration::PATHTsk;
std::string Configuration::PATHMsg;

mode_t Configuration::PATHMode = 0755;

}
