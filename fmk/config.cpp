/******************************************************************************
 * File:    config.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.config
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
 *   Implement Config class
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

#include <climits>
#include <cstdlib>

#include "config.h"

#include "channels.h"

#include "dbg.h"
#include "str.h"
#include "tools.h"
#include "dbhdlpostgre.h"
#include "except.h"
#include "log.h"

#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <libgen.h>
#include <memory>

using Configuration::cfg;

#define WRITE_MESSAGE_FILES

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

Config & Config::_()
{
    static Config configInstance {};

    return configInstance;
}

//----------------------------------------------------------------------
// Method: init
//----------------------------------------------------------------------
void Config::init(json v)
{
    value = v;

    fillData();
    isActualFile = false;
    processConfig();
}

//----------------------------------------------------------------------
// Method: setCurrentHostAddress
//----------------------------------------------------------------------
void Config::setCurrentHostAddress(std::string & addr)
{
    currentHostAddr = addr;
}

//----------------------------------------------------------------------
// Method: init
//----------------------------------------------------------------------
void Config::init(std::string fName)
{
    isLive = false;
    sessionId = timeTag();
    DbgMsg("Provided fName='" + fName + "'");
    if (fName.compare(0,5,"db://") == 0) {
        DbgMsg("A database URL!");
        // fName in fact is a db url in the form:
        //   db://user:pwd@host:port/dbname
        // So, take the fields
        std::string url = fName.substr(5);
        DBUser = url.substr(0, url.find(":")); url.erase(0, url.find(":") + 1); // take username
        DBPwd  = url.substr(0, url.find("@")); url.erase(0, url.find("@") + 1); // take password
        DBHost = url.substr(0, url.find(":")); url.erase(0, url.find(":") + 1); // take host
        DBPort = url.substr(0, url.find("/")); url.erase(0, url.find("/") + 1); // take port
        DBName = url; // take database name
        DbgMsg(DBUser + ":" + DBPwd + "@" + DBHost + ":" + DBPort + "/" + DBName);
        
        DbgMsg("Configuration is retrieved from db: " + fName);
        readConfigFromDB();
        isActualFile = false;
    } else {
        DbgMsg("Configuration is retrieved from file: " + fName);
        setConfigFile(fName);
        readConfigFromFile();
        if (weAreOnMaster) { saveConfigToDB(); }
        isActualFile = true;
    }

    isLive = true;

    processConfig();
}

//----------------------------------------------------------------------
// Method: synchronizeSessionId
//----------------------------------------------------------------------
void Config::synchronizeSessionId(std::string newId)
{
    std::string newPATHSession(PATHRun + "/" + newId);

    if (rename(PATHSession.c_str(), newPATHSession.c_str()) != 0) {
        perror("Change sessionId");
        TRC("Cannot rename PATHSession from " + PATHSession +
            " to " + newPATHSession);
        return;
    }

    PATHSession = newPATHSession;

    PATHLog     = PATHSession + "/log";
    PATHRlog    = PATHSession + "/rlog";
    PATHTmp     = PATHSession + "/tmp";
    PATHTsk     = PATHSession + "/tsk";
    PATHMsg     = PATHSession + "/msg";
}

//----------------------------------------------------------------------
// Method: fillData
//----------------------------------------------------------------------
void Config::fillData()
{
    json & v = value;
    SET_GRP(CfgGrpGeneral,          general);
    SET_GRP(CfgGrpNetwork,          network);
    SET_GRP(CfgGrpDB,               db);
    SET_GRP(CfgGrpConnectivity,     connectivity);
    SET_GRP(CfgGrpProducts,         products);
    SET_GRP(CfgGrpOrchestration,    orchestration);
    SET_GRP(CfgGrpUserDefToolsList, userDefTools);
    SET_GRP(CfgGrpFlags,            flags);

    Log::setMinLogLevel(general.logLevel());

    DBHost = db.host();
    DBPort = db.port();
    DBName = db.name();
    DBUser = db.user();
    DBPwd  = db.pwd();

    TraceMsg(DBUser + ":" + DBPwd + "@" + DBHost + ":" + DBPort + "/" + DBName);

    weAreOnMaster = (network.masterNode() == currentHostAddr);
}

//----------------------------------------------------------------------
// Method: setLastAccess
// Updates the date of last access to the config
//----------------------------------------------------------------------
void Config::setLastAccess(std::string last)
{
//    cfg["general"]["last_access"] = last;
}

//----------------------------------------------------------------------
// Method: setConfigFile
// Set config file name
//----------------------------------------------------------------------
void Config::setConfigFile(std::string fName)
{
    char actualpath [PATH_MAX+1];
    char * ptr;
    ptr = realpath(fName.c_str(), actualpath);
    cfgFileName = std::string(ptr);
    cfgFilePath = std::string(dirname(ptr));
    TraceMsg("cfgFileName set to: " + cfgFileName);
    TraceMsg("cfgFilePath set to: " + cfgFilePath);
}

//----------------------------------------------------------------------
// Method: readConfigFromFile
// Loads the config file content into memory
//----------------------------------------------------------------------
void Config::readConfigFromFile()
{
    std::ifstream cfgFile(cfgFileName);
    std::stringstream buffer;
    buffer << cfgFile.rdbuf();
    TraceMsg("CONFIG FROM FILE:\n" + buffer.str());
    fromStr(buffer.str());
    fillData();
}

//----------------------------------------------------------------------
// Method: readConfigFromDB
// Loads the config from the DB into memory
//----------------------------------------------------------------------
void Config::readConfigFromDB()
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    dbHdl->setDbHost(Config::DBHost);
    dbHdl->setDbPort(Config::DBPort);
    dbHdl->setDbName(Config::DBName);
    dbHdl->setDbUser(Config::DBUser);
    dbHdl->setDbPasswd(Config::DBPwd);

    // Check that connection with the DB is possible
    try {
        dbHdl->openConnection();
    } catch (RuntimeException & e) {
        DBG("ERROR Trying to open connection to DB");
        Log::log("SYSTEM", Log::FATAL, e.what());
        return;
    }

    // Try to retrieve the config from the DB
    std::vector< std::vector<std::string> > config;
    std::string dateCreated;

    try {
        // Get config table and Transfer DB config info to JSON value
        dbHdl->getTable("configuration", config);
        unsigned int lastConfig = config.size() - 1;
        dateCreated = config.at(lastConfig).at(0);
        std::string configData(config.at(lastConfig).at(1));
        TRC("Retrieving from DB config:\n" + config.at(lastConfig).at(1));
        cfg.fromStr(configData);
        cfgFileName = "<internalDB> " + Config::DBName + "::configuration";
    } catch (RuntimeException & e) {
        DBG("ERROR Trying to retrieve configuration table");
        Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        DBG("ERROR Trying to retrieve configuration table");
        Log::log("SYSTEM", Log::ERROR,
                 "Unexpected error accessing "
                 "database for retrieval of system configuration");
        return;
    }    
    
    // Modificar fecha de último accesso
    std::string now = timeTag();
    std::string cmd("UPDATE configuration SET last_accessed = '" + now + "' "
                    "WHERE created='" + dateCreated + "'");
    
    try {
        dbHdl->runCmd(cmd);
    } catch (RuntimeException & e) {
        Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        Log::log("SYSTEM", Log::ERROR,
                 "Unexpected error accessing "
                 "database for retrieval of system configuration");
        return;
    }

    // Get session id
    if (sessionId.empty()) {
        try {
            std::pair<std::string, std::string> sessionAndState;
            sessionAndState = dbHdl->getLatestState();
            sessionId = sessionAndState.first;
            if (sessionId.empty()) {
                sessionId = timeTag();
            }
        } catch (...) {
            Log::log("SYSTEM", Log::ERROR,
                     "Unexpected error accessing "
                     "database for retrieval of session name");
            return;
        }
    }
    TraceMsg("SessionId: " + sessionId);

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: saveConfigToDB
// Store the config into the DB
//----------------------------------------------------------------------
void Config::saveConfigToDB()
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    dbHdl->setDbHost(Config::DBHost);
    dbHdl->setDbPort(Config::DBPort);
    dbHdl->setDbName(Config::DBName);
    dbHdl->setDbUser(Config::DBUser);
    dbHdl->setDbPasswd(Config::DBPwd);

    // Check that connection with the DB is possible
    try {
        dbHdl->openConnection();
    } catch (RuntimeException & e) {
        Log::log("SYSTEM", Log::FATAL, e.what());
        return;
    }

    // Modify cfg to store
    //ConfigInfo & cfgInfo = ConfigInfo::data();
    //Json::Value dbcfg(cfg);
    json dbcfg = cfg.val();
    dbcfg["products"]["parsing_regex"] = cfg.products.parsingRegEx();

    // Transfer configuration from JSON value to DB
    std::string cmd;
    std::string now = timeTag();
    Json::FastWriter writer;
    std::string cfgString = writer.write(dbcfg);

    cmd = "INSERT INTO configuration (created, last_accessed, cfg) VALUES ";
    cmd += "('" + now + "', '" + now + "', '" + cfgString + "')";

    try {
         dbHdl->runCmd(cmd);
    } catch (RuntimeException & e) {
        Log::log("SYSTEM", Log::ERROR, e.what());
        return;
    } catch (...) {
        Log::log("SYSTEM", Log::ERROR,
                          "Unexpected error accessing "
                          "database for storage of system config");
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: getRegExFromCfg
// Retrieves filename regex from cfg or from designated file
//----------------------------------------------------------------------
std::string Config::getRegExFromCfg(std::string & regexStr)
{
    /*
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
*/
}

//----------------------------------------------------------------------
// Method: processConfig
// Loads the config file content into memory
//----------------------------------------------------------------------
void Config::processConfig()
{
    static std::map<Message_Tag, std::string> msgTags = {
        {Tag_ChnlCmd,      "CMD"},    
        {Tag_ChnlEvtMng,   "EVTMNG"}, 
        {Tag_ChnlHMICmd,   "HMICMD"}, 
        {Tag_ChnlInData,   "INDATA"}, 
        {Tag_ChnlTskSched, "TSKSCHED"},
        {Tag_ChnlTskReg,   "TSKREG"}, 
        {Tag_MsgTskRqst,   "TSKRQST"},
        {Tag_MsgTskProc,   "TSKPROC"},
        {Tag_MsgTskRep,    "TSKREP"}, 
        {Tag_ChnlFmkMon,   "FMKMON"}, 
        {Tag_MsgHostMon,   "HOSTMON"}};

    fillData();

    if (std::string(CONFIG_VERSION) != cfg.general.cfgVersion()) {
        FatalMsg("Configuration version (" +
                 cfg.general.cfgVersion() +
                 ") not compatible with the one supported (" +
                 std::string(CONFIG_VERSION) +
                 ") by this release.\n");
    }
    
    PATHBase    = general.workArea();

    PATHData    = PATHBase + "/data";

    PATHRun     = PATHBase + "/run";

    PATHWww     = PATHBase + "/www";
    
    PATHBin     = PATHRun + "/bin";
    PATHSession = PATHRun + "/" + sessionId;

    PATHLog     = PATHSession + "/log";
    PATHRlog    = PATHSession + "/rlog";
    PATHTmp     = PATHSession + "/tmp";
    PATHTsk     = PATHSession + "/tsk";
    PATHMsg     = PATHSession + "/msg";

    for (auto & p : std::vector<std::string>
                      { PATHSession, PATHLog, PATHRlog,
                        PATHTmp, PATHTsk, PATHMsg }) { TraceMsg(p); }

    storage.inbox    = PATHData + "/inbox";
    storage.archive  = PATHData + "/archive";
    storage.gateway  = PATHData + "/gateway";
    storage.userArea = PATHData + "/user";

    writeMsgsMask = 0;
    for (auto & kv : msgTags) {
        for (const json & msgType : flags["msgsToDisk"]) {
            if (msgType.asString() == kv.second) {
                writeMsgsMask |= static_cast<int>(kv.first);
                break;
            }
        }
    }

    writeMsgsToDisk = flags.writeMsgsToDisk();
}

//----------------------------------------------------------------------
// Method: consolidate
// Injects changes in the components of the cfg structure into the value
//----------------------------------------------------------------------
void Config::consolidate()
{
    value["general"]       = general.val();
    value["network"]       = network.val();
    value["db"]            = db.val();
    value["connectivity"]  = connectivity.val();
    value["connectivity"]["vospace"] = connectivity.vospace.val();
    value["connectivity"]["jupyter"] = connectivity.jupyter.val();
    value["products"]      = products.val();
    value["orchestration"] = orchestration.val();
    value["userDefTools"]  = userDefTools.val();
    value["flags"]         = flags.val();
}

//----------------------------------------------------------------------
// Method: generateProcFmkInfoStructure
// Generates a new ProcFmkInfo structure
//----------------------------------------------------------------------
void Config::generateProcFmkInfoStructure()
{
    // Handy lambda to compute ports number, h=1:procHosts, i=0:agentsInHost
    // We will assume agentsInHost is < 10
    auto portnum = [](int start, int h, int i) -> int
        { return start + 10 * h + i; };
    const int ContainerAgentsOffset = 1000;
    const int SwarmAgentsOffset     = 2000;
    
    char sAgName[100];

    HostInfo hi;
    hi.update();

    std::vector<std::string> & agName    = cfg.agentNames;
    std::vector<std::string> & agHost    = cfg.agHost;
    std::vector<int> &         agPortTsk = cfg.agPortTsk;

    procFmkInfo->numContTasks = 0;
    procFmkInfo->numSrvTasks = 0;

    agName.clear();
    agHost.clear();
    agPortTsk.clear();

    // Create, for each agent (container runner or swarm manager) a name, a port
    // number, and same its host ip
    
    int h = 1;
    for (auto & ckv : cfg.network.processingNodes()) {
        int numOfTskAgents = ckv.second;
        hi.update();

        std::string ip = ckv.first;

        ProcessingHostInfo * ph = new ProcessingHostInfo;
        ph->name      = ip;
        ph->numAgents = numOfTskAgents;
        ph->hostInfo  = hi;
        ph->numTasks  = 0;

        for (int i = 0; i < ph->numAgents; ++i) {
            sprintf(sAgName, "TskAgent_%02d_%02d", h, i + 1);
            agName.push_back(std::string(sAgName));
            agPortTsk.push_back(portnum(startingPort + ContainerAgentsOffset, h, i));
            agHost.push_back(ip);
            
            AgentInfo agInfo;
            agInfo.name       = agName.back();
            agInfo.taskStatus = TaskStatusSpectra();
            agInfo.load       = (rand() % 1000) * 0.01;
            ph->agInfo.push_back(agInfo);
            ph->numTasks += agInfo.taskStatus.total;

            TRC(ip + ": " + std::to_string(i + 1) + "/" +
                std::to_string(ph->numAgents) + "\t" +
                agInfo.name + " : " + std::to_string(agPortTsk.back()));

        }

        procFmkInfo->hostsInfo[ph->name] = ph;
        procFmkInfo->numContTasks += ph->numTasks;
        agentMode[ip] = CONTAINER;
        ++h;
    }

    for (auto & skv : cfg.network.swarms()) {
        hi.update();
        CfgGrpSwarm & swrm = skv.second;
        if (swrm.serviceNodes().size() < 1) { continue; }

        std::string ip = swrm.serviceNodes().at(0);

        sprintf(sAgName, "Swarm_%s", ip.c_str());
        agName.push_back(std::string(sAgName));
        agPortTsk.push_back(portnum(startingPort + SwarmAgentsOffset, h, 0));
        agHost.push_back(ip);

        SwarmInfo * sw = new SwarmInfo;
        sw->name       = ip;
        sw->scale      = swrm.scale();
        sw->hostInfo   = hi;
        sw->taskStatus = TaskStatusSpectra();

        TRC(ip + ": \t\t" + sAgName + " : " + std::to_string(agPortTsk.back()));
        
        procFmkInfo->swarmInfo[ip] = sw;
        procFmkInfo->numSrvTasks += sw->scale;
        agentMode[ip] = SERVICE;
        ++h;
    }

    dump();

    for (int i = 0; i < agName.size(); ++i) {
        TRC(agHost.at(i) + ": \t" + agName.at(i) + " : " + std::to_string(agPortTsk.at(i)));
    }
}

//----------------------------------------------------------------------
// Method: getEnvVar
// Returns the content of an environment variable
//----------------------------------------------------------------------
std::string Config::getEnvVar(std::string const & key) const
{
    char * val = std::getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

std::string Config::DBHost;
std::string Config::DBPort;
std::string Config::DBName;
std::string Config::DBUser;
std::string Config::DBPwd;

std::string Config::PATHBase;
std::string Config::PATHData;
std::string Config::PATHWww;
std::string Config::PATHRun;
std::string Config::PATHBin;
std::string Config::PATHSession;
std::string Config::PATHLog;
std::string Config::PATHRlog;
std::string Config::PATHTmp;
std::string Config::PATHTsk;
std::string Config::PATHMsg;

mode_t Config::PATHMode = 0755;

bool Config::isLive = false;

ProcessingFrameworkInfo * Config::procFmkInfo = new ProcessingFrameworkInfo;

const int Config::MaxContainerAge = 3600;    

std::map<std::string, AgentMode> Config::agentMode;

//}
