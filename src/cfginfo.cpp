/******************************************************************************
 * File:    cfginfo.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.ConfigurationInfo
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
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
#include "config.h"
#include "tools.h"

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

ConfigurationInfo & ConfigurationInfo::data()
{
    static ConfigurationInfo cfgInfoInstance {};
    return cfgInfoInstance;
}

std::string ConfigurationInfo::toJSONString()
{
    std::stringstream s;
    std::string i4("    ");
    std::string nl("\n");

#define C(x)  "\"" << x << "\""
#define Cc(x) "\"" << x << "\","

    // START
    s << "{" << nl;

    // General section
    s << i4 << "\"general\": {" << nl
    << i4 << i4 << "\"app_name\": " << Cc(appName) << nl
    << i4 << i4 << "\"app_version\": " << Cc(appVersion) << nl
    << i4 << i4 << "\"last_access\": " << Cc(lastAccess) << nl
    << i4 << i4 << "\"run_path\": " << C(storage.run) << nl
    << i4 << "}," << nl;

    // Nodes
    s << i4 << "\"nodes\": {" << nl
    << i4 << i4 << "\"hmi_node\": " << Cc(qpfhmiCfg.name) << nl
    << i4 << i4 << "\"master_machine\": " << Cc(masterMachine) << nl
    << i4 << i4 << "\"node_list\": {" << nl;
    int n = peersCfg.size();
    for (int i = 0; i < n; ++i) {
        Peer & p = peersCfg.at(i);
        s << i4 << i4 << i4 << C(p.name) << ": {" << nl
        << i4 << i4 << i4 << i4 << "\"client\": " << Cc(p.clientAddr) << nl
        << i4 << i4 << i4 << i4 << "\"server\": " << Cc(p.serverAddr) << nl
        << i4 << i4 << i4 << i4 << "\"type\": " << C(p.type) << nl
        << i4 << i4 << i4 << ((i < (n - 1)) ? "}," : "}") << nl;
    }
    s << i4 << i4 << "}" << nl
    << i4 << "}," << nl;

    // Machines
    s << i4 << "\"machines\": {" << nl;
    n = machines.size();
    if (n > 0) {
        for (int i = 0; i < n; ++i) {
            std::string & mach = machines.at(i);
            std::vector<std::string> & mnodes = machineNodes[mach];
            s << i4 << i4 << C(mach) << ": [" << nl;
            int m = mnodes.size();
            if (m > 0) {
                for (int j = 0; j < m - 1; ++j) {
                    s << i4 << i4 << i4 << Cc(mnodes.at(j)) << nl;
                }
                s << i4 << i4 << i4 << C(mnodes.at(m - 1)) << nl;
            }
            s << i4 << i4 << ((i < (n - 1)) ? "]," : "]") << nl;
        }
    }
    s << i4 << "}," << nl;

    // Connections
    s << i4 << "\"connections\": {" << nl;
    n = peersCfg.size();
    if (n > 0) {
        for (int i = 0; i < n; ++i) {
            Peer & p = peersCfg.at(i);
            std::vector<std::string> & nconn = connections[p.name];
            s << i4 << i4 << C(p.name) << ": [" << nl;
            int m = nconn.size();
            if (m > 0) {
                for (int j = 0; j < m - 1; ++j) {
                    s << i4 << i4 << i4 << Cc(nconn.at(j)) << nl;
                }
                s << i4 << i4 << i4 << C(nconn.at(m - 1)) << nl;
            }
            s << i4 << i4 << ((i < (n - 1)) ? "]," : "]") << nl;
        }
    }
    s << i4 << "}," << nl;

    // Database
    s << i4 << "\"db\": {" << nl
    << i4 << i4 << "\"host\": " << Cc(Configuration::DBHost) << nl
    << i4 << i4 << "\"port\": " << Cc(Configuration::DBPort) << nl
    << i4 << i4 << "\"name\": " << Cc(Configuration::DBName) << nl
    << i4 << i4 << "\"user\": " << Cc(Configuration::DBUser) << nl
    << i4 << i4 << "\"pwd\": " << C(Configuration::DBPwd) << nl
    << i4 << "}," << nl;

    // Products
    std::string rx;
    for (auto c : parsing_regex) {
        rx.push_back(c);
        if (c == '\\') { rx.push_back(c); }
    }

    s << i4 << "\"products\": {" << nl
    << i4 << i4 << "\"product_types\": [" << nl;
    n = orcParams.productTypes.size();
    if (n > 0) {
        for (int i = 0; i < n - 1; ++i) {
            s << i4 << i4 << i4 << Cc(orcParams.productTypes.at(i)) << nl;
        }
        s << i4 << i4 << i4 << C(orcParams.productTypes.at(n - 1)) << nl;
    }
    s << i4 << i4 << " ]," << nl
    << i4 << i4 << "\"parsing_regex\": " << Cc(rx) << nl
    << i4 << i4 << "\"parsing_assign\": " << Cc(parsing_assign) << nl
    << i4 << i4 << "\"product_id_tpl\": " << Cc(product_id_tpl) << nl
    << i4 << i4 << "\"data_ext\": " << Cc(data_ext) << nl
    << i4 << i4 << "\"meta_ext\": " << Cc(meta_ext) << nl
    << i4 << i4 << "\"log_ext\": " << C(log_ext) << nl
    << i4 << "}," << nl;

    // Orchestration
    s << i4 << "\"orchestration\": {" << nl
    << i4 << i4 << "\"rules\": {" << nl;
    n = orcParams.rules.size();
    for (int i = 0; i < n; ++i) {
        Rule * r = orcParams.rules.at(i);
        s << i4 << i4 << i4 << C(r->name) << ": {" << nl
        << i4 << i4 << i4 << i4 << "\"inputs\": ";
        int m = r->inputs.size();
        s << "\"" << r->inputs.at(0);
        for (int j = 1; j < m; ++j) {
            s << "," << r->inputs.at(j);
        }
        s << "\"," << nl
        << i4 << i4 << i4 << i4 << "\"outputs\": ";
        m = r->outputs.size();
        s << "\"" << r->outputs.at(0);
        for (int j = 1; j < m; ++j) {
            s << "," << r->outputs.at(j);
        }
        s << "\"," << nl
        << i4 << i4 << i4 << i4 << "\"processing\": " << Cc(r->processingElement) << nl
        << i4 << i4 << i4 << i4 << "\"condition\": " << C(r->condition) << nl
        << i4 << i4 << i4 << ((i < (n - 1)) ? "}," : "}") << nl;
    }
    s << i4 << i4 << "}" << nl
    << i4 << "}," << nl;

    // Processors
    s << i4 << "\"processing\": {" << nl
    << i4 << i4 << "\"processors\": {" << nl;
    n = orcParams.processors.size();
    int k = 0;
    for (auto & kv : orcParams.processors) {
        Processor * p = kv.second;
        s << i4 << i4 << i4 << C(p->name) << ": {" << nl
        << i4 << i4 << i4 << i4 << "\"exe_path\": " << Cc(p->exePath) << nl
        << i4 << i4 << i4 << i4 << "\"input_path\": " << Cc(p->inPath) << nl
        << i4 << i4 << i4 << i4 << "\"name\": " << Cc(p->name) << nl
        << i4 << i4 << i4 << i4 << "\"output_path\": " << C(p->outPath) << nl
        << i4 << i4 << i4 << ((k < (n - 1)) ? "}," : "}") << nl;
        ++k;
    }
    s << i4 << i4 << "}" << nl
    << i4 << "}," << nl;

    // Storage
    s
    << i4 << "\"storage\": {" << nl
    << i4 << i4 << "\"base\": {" << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.base) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"run\": {" << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.run) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"incoming\": {" << nl
    << i4 << i4 << i4 << "\"protocol\": " << Cc(storage.inbox.protocol) << nl
    << i4 << i4 << i4 << "\"address\": " << Cc(storage.inbox.address) << nl
    << i4 << i4 << i4 << "\"port\": " << Cc(storage.inbox.port) << nl
    << i4 << i4 << i4 << "\"user\": " << Cc(storage.inbox.user) << nl
    << i4 << i4 << i4 << "\"password\": " << Cc(storage.inbox.passwd) << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.inbox.path) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"local_archive\": {" << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.local_archive.path) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"archive\": {" << nl
    << i4 << i4 << i4 << "\"protocol\": " << Cc(storage.archive.protocol) << nl
    << i4 << i4 << i4 << "\"address\": " << Cc(storage.archive.address) << nl
    << i4 << i4 << i4 << "\"port\": " << Cc(storage.archive.port) << nl
    << i4 << i4 << i4 << "\"user\": " << Cc(storage.archive.user) << nl
    << i4 << i4 << i4 << "\"password\": " << Cc(storage.archive.passwd) << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.archive.path) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"gateway\": {" << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.gateway.path) << nl
    << i4 << i4 << "}," << nl
    << i4 << i4 << "\"outgoing\": {" << nl
    << i4 << i4 << i4 << "\"protocol\": " << Cc(storage.outbox.protocol) << nl
    << i4 << i4 << i4 << "\"address\": " << Cc(storage.outbox.address) << nl
    << i4 << i4 << i4 << "\"port\": " << Cc(storage.outbox.port) << nl
    << i4 << i4 << i4 << "\"user\": " << Cc(storage.outbox.user) << nl
    << i4 << i4 << i4 << "\"password\": " << Cc(storage.outbox.passwd) << nl
    << i4 << i4 << i4 << "\"path\": " << C(storage.outbox.path) << nl
    << i4 << i4 << "}" << nl
    << i4 << "}," << nl;

    // User Defined Tools
    s << i4 << "\"userdeftools\": [" << nl;
    n = userDefTools.size();
    k = 0;
    for (auto & kv : userDefTools) {
        UserDefTool & u = kv.second;
        s << i4 << i4 << "{" << nl
        << i4 << i4 << i4 << "\"name\": " << Cc(u.name) << nl
        << i4 << i4 << i4 << "\"description\": " << Cc(u.desc) << nl
        << i4 << i4 << i4 << "\"arguments\": " << Cc(u.args) << nl
        << i4 << i4 << i4 << "\"executable\": " << Cc(u.exe) << nl
        << i4 << i4 << i4 << "\"product_types\": [ ";
        int m = u.prod_types.size();
        s << C(u.prod_types.at(0));
        for (int j = 1; j < m; ++j) {
            s << ", " << C(u.prod_types.at(j));
        }
        s << " ]" << nl
        << i4 << i4 << ((k < (n - 1)) ? "}," : "}") << nl;
        ++k;
    }
    s << i4 << "]," << nl;

    // Flags
    s << i4 << "\"flags\": {" << nl
    << i4 << i4 << "\"monitoring\": {" << nl;

    n = flags.monit.msgsToDisk.size();
    k = 0;
    s << i4 << i4 << i4 << "\"msgs_to_disk\": [";
    for (auto & kv : flags.monit.msgsToDisk) {
        k++;
        if (k == n) {
            s << ' ' << C(kv.first);
        } else {
            s << ' ' << Cc(kv.first);
        }
    }
    s << " ]," << nl;

    n = flags.monit.msgsToDB.size();
    k = 0;
    s << i4 << i4 << i4 << "\"msgs_to_db\": [ ";
    for (auto & kv : flags.monit.msgsToDB) {
        k++;
        if (k == n) {
            s << ' ' << C(kv.first);
        } else {
            s << ' ' << Cc(kv.first);
        }
    }
    s << " ]," << nl;

    s << i4 << i4 << i4 << "\"notify_msg_arrival\": "
    << ((flags.monit.notifyMsgArrival) ? "true" : "false") << "," << nl;
    s << i4 << i4 << i4 << "\"group_task_agent_logs\": "
    << ((flags.monit.groupTaskAgentLogs) ? "true" : "false") << nl;

    s << i4 << i4 << "}," << nl
    << i4 << i4 << "\"processing\": {" << nl;

    s << i4 << i4 << i4 << "\"allow_reprocessing\": "
    << ((flags.proc.allowReprocessing) ? "true" : "false") << "," << nl;
    s << i4 << i4 << i4 << "\"intermediate_products\": "
    << ((flags.proc.intermedProducts) ? "true" : "false") << nl;

    s << i4 << i4 << "}," << nl
    << i4 << i4 << "\"archiving\": {" << nl;

    s << i4 << i4 << i4 << "\"send_outputs_to_main_archive\": "
    << ((flags.arch.sendOutputsToMainArchive) ? "true" : "false") << nl;

    s << i4 << i4 << "}" << nl
    << i4 << "}" << nl;


    s << "}" << nl;
    // END

    return s.str();
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
    std::cerr << "storage.run                : " << storage.run << std::endl;
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

    // User defined tools
    std::cerr << "-- User def. tools --\n";

    for (auto && kv : userDefTools) {
        UserDefTool & udt = kv.second;
        std::cerr << udt.name << ", " << udt.desc << " ["
                  << udt.exe << ' ' << udt.args << "] valid for:";
        for (auto & s : udt.prod_types) {
            std::cerr << ' ' << s;
        }
        std::cerr << '\n';
    }

    std::cerr << "==============================================================================" << std::endl;
}

}
