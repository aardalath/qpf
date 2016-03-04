/******************************************************************************
 * File:    deployer.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.QPF.Deployer
 *
 * Version: 0.0.1
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement Deployer class
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

#include "deployer.h"

#include <thread>

#define L(x) do { std::cerr << x << std::endl << std::flush; } while (0);

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Constructor: Deployer
//----------------------------------------------------------------------
Deployer::Deployer(int argc, char *argv[]) :
    spawnPeerProcesses(false),
    waitingForSignalToGo(true),
    verboseOutput(false),
    usec(50000),
    newConfigFile(std::string()),
    configFile(0),
    cfg(0),
    evtMng(0),
    hmiNeeded(false)
{
    // Change value for delay between peer nodes launches (default: 50000us)
    if (!processCmdLineOpts(argc, argv)) { exit(EXIT_FAILURE); }

    //===== Read Configuration =====
    if (!newConfigFile.empty()) { configFile = newConfigFile.c_str(); }
    readConfig(configFile);

    std::thread(&Deployer::run, this).detach();
}

//----------------------------------------------------------------------
// Constructor: Deployer
//----------------------------------------------------------------------
Deployer::~Deployer()
{
}

//----------------------------------------------------------------------
// Method: mustLaunchHMI
// Returns true if the application must launch the HMI
//----------------------------------------------------------------------
bool Deployer::mustLaunchHMI()
{
    return hmiNeeded;
}

//----------------------------------------------------------------------
// Method: getConfigFileName
// Returns the name of the configuration file name used
//----------------------------------------------------------------------
char * Deployer::getConfigFileName()
{
    return const_cast<char*>(configFile);
}

//----------------------------------------------------------------------
// Method: run
// Minimal working example
// The elements are launched and wait for the start from the
// Event Manager.
// Then the systems starts the normal operational cycle.
// Current version of Event Manager takes external events from
// the HMI (simulated external events)
//----------------------------------------------------------------------
int Deployer::run()
{
    //===== Initial (quick and dirty) cleanup =====
    removeOldFiles();
    LibComm::Log::setConsoleOutput(verboseOutput);

    //===== Launch them =====
    launchPeerNodes();

    //===== Start System =====
    start();

    //===== System cleanup at shutdown =====
    cleanUp();

    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------
// Method: processCmdLineOpts
// Processes command line options to configure execution
//----------------------------------------------------------------------
bool Deployer::processCmdLineOpts(int argc, char * argv[])
{
    bool retVal = true;
    int exitCode = EXIT_FAILURE;

    int opt;
    while ((opt = getopt(argc, argv, "hpvt:c:m:")) != -1) {
        switch (opt) {
        case 'p':
            spawnPeerProcesses = true;
            break;
        case 'v':
            verboseOutput = true;
            break;
        case 't':
            usec = (useconds_t)(atoi(optarg));
            break;
        case 'c':
            newConfigFile = std::string(optarg);
            break;
        case 'h':
            exitCode = EXIT_SUCCESS;
        default: /* '?' */
            L("Usage: " << argv[0] << " [-p] [-c configFile] [-t usecs] [-h]");
            L("where:");
            L("\t-p         Components are deployed as separate processes,");
            L("\t           instead of threads.");
            L("\t-c cfgFile System is reconfigured with configuration in");
            L("\t           file cfgFile (configuration is then saved to DB).");
            L("\t-t usecs   Number of microseconds of time lapse between ");
            L("\t           components deployment (default: 50000us).");
            L("\t-h         shows this help message.");
            exit(exitCode);
        }
    }

    return retVal;
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void Deployer::readConfig(const char * configFile)
{
    Configuration * cfg = new Configuration(configFile);

    cfg->getGeneralInfo(qpfCfg->appName, qpfCfg->appVersion, qpfCfg->lastAccess);
    cfg->getProductTypes(qpfCfg->orcParams.productTypes);
    cfg->reset();

    for (int i = 0; i < cfg->getNumOrchRules(); ++i) {
        TaskOrchestrator::Rule * rule = new TaskOrchestrator::Rule;
        cfg->getOrchRule(rule->name, rule->inputs,
                         rule->outputs, rule->processingElement);
        qpfCfg->orcParams.rules.push_back(rule);
    }

    for (int i = 0; i < cfg->getNumProcs(); ++i) {
        TaskOrchestrator::ProcElem * pe = new TaskOrchestrator::ProcElem;
        cfg->getProc(pe->name, pe->exePath, pe->inPath, pe->outPath);
        qpfCfg->orcParams.procElems[pe->name] = pe;
    }

    for (int i = 0; i < cfg->getNumNodes(); ++i) {
        LibComm::Router2RouterPeer::Peer * peer = new LibComm::Router2RouterPeer::Peer;
        cfg->getNode(peer->name, peer->type, peer->clientAddr, peer->serverAddr);
        qpfCfg->peersCfg.push_back(*peer);
        qpfCfg->peerNames.push_back(peer->name);
    }

    qpfCfg->qpfhmiCfg.name = cfg->getHMINodeName();
    cfg->getNodeByName(qpfCfg->qpfhmiCfg.name,
                       qpfCfg->qpfhmiCfg.type,
                       qpfCfg->qpfhmiCfg.clientAddr,
                       qpfCfg->qpfhmiCfg.serverAddr);

    // Ensure paths for the execution are available and readu
    std::vector<std::string> runPaths { Configuration::PATHRun,
                                        Configuration::PATHLog,
                                        Configuration::PATHRlog,
                                        Configuration::PATHTmp,
                                        Configuration::PATHTsk,
                                        Configuration::PATHMsg };
    for (auto & p : runPaths) {
        if (mkdir(p.c_str(), Configuration::PATHMode) != 0) {
            std::perror(("mkdir " + p).c_str());
            exit(EXIT_FAILURE);
        }
    }
    LibComm::Log::setLogBaseDir(Configuration::PATHRun);

    delete cfg;
}

//----------------------------------------------------------------------
// Method: createPeerNodes
// Creates the peer node objects with the information from the system
// configuration
//----------------------------------------------------------------------
void Deployer::createPeerNodes()
{
    L("Creating peer nodes . . .");

    for (unsigned int i = 0; i < qpfCfg->peersCfg.size(); ++i) {

        std::string & peerName = qpfCfg->peersCfg[i].name;
        std::string & peerType = qpfCfg->peersCfg[i].type;

        const char * cpeerName = qpfCfg->peersCfg[i].name.c_str();

        L("Creating peer node " << peerName << " . . .");

        if        (peerType == "evtmng") {
            assert(pnd->evtMng == 0);
            pnd->evtMng = new EventManager(cpeerName);
            pnd->evtMng->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->nodes.push_back(pnd->evtMng);
        } else if (peerType == "datamng") {
            assert(pnd->dataMng == 0);
            pnd->dataMng = new DataManager(cpeerName);
            pnd->dataMng->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->nodes.push_back(pnd->dataMng);
        } else if (peerType == "logmng") {
            assert(pnd->logMng == 0);
            pnd->logMng = new LogManager(cpeerName);
            pnd->logMng->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->nodes.push_back(pnd->logMng);
        } else if (peerType == "taskmng") {
            assert(pnd->tskMng == 0);
            pnd->tskMng = new TaskManager(cpeerName);
            pnd->tskMng->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->nodes.push_back(pnd->tskMng);
        } else if (peerType == "taskorc") {
            assert(pnd->tskOrc == 0);
            pnd->tskOrc = new TaskOrchestrator(cpeerName);
            pnd->tskOrc->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->nodes.push_back(pnd->tskOrc);
        } else if (peerType == "taskagent") {
            TaskAgent * tskAg = new TaskAgent(cpeerName);
            tskAg->addPeer(&(qpfCfg->peersCfg[i]), true);
            pnd->tskAgents.push_back(tskAg);
            pnd->nodes.push_back(tskAg);
        } else {
            // Do nothing, not yet implemented
        }

    }
}

//----------------------------------------------------------------------
// Method: createPeerNodeConnections
// Creates the node connections
//----------------------------------------------------------------------
void Deployer::createPeerNodeConnections()
{
    L("Creating peer connections . . .");

    // - Event Manager
    pnd->evtMng->addPeer(pnd->dataMng->selfPeer());
    pnd->evtMng->addPeer(pnd->logMng->selfPeer());
    pnd->evtMng->addPeer(pnd->tskMng->selfPeer());
    pnd->evtMng->addPeer(pnd->tskOrc->selfPeer());
    // Set connection to QPFHMI
    pnd->evtMng->addPeer(&qpfCfg->qpfhmiCfg);
    // TaskAgents must be awaked by the Event Manager
    // (though no other direct messages are expected by them
    //  comming from the Event Manager)
    for (unsigned int k = 0; k < pnd->tskAgents.size(); ++k) {
        pnd->evtMng->addPeer(pnd->tskAgents.at(k)->selfPeer());
    }

    // - Data Manager
    pnd->dataMng->addPeer(pnd->evtMng->selfPeer());
    pnd->dataMng->addPeer(pnd->tskOrc->selfPeer());
    pnd->dataMng->addPeer(pnd->tskMng->selfPeer());
    pnd->dataMng->addPeer(pnd->logMng->selfPeer());

    // - Log Manager
    pnd->logMng->addPeer(pnd->evtMng->selfPeer());
    pnd->logMng->addPeer(pnd->dataMng->selfPeer());
    pnd->logMng->addPeer(pnd->tskMng->selfPeer());
    pnd->logMng->addPeer(pnd->tskOrc->selfPeer());
    for (unsigned int k = 0; k < pnd->tskAgents.size(); ++k) {
        pnd->logMng->addPeer(pnd->tskAgents.at(k)->selfPeer());
    }

    // - Task Manager
    pnd->tskMng->addPeer(pnd->evtMng->selfPeer());
    pnd->tskMng->addPeer(pnd->tskOrc->selfPeer());
    pnd->tskMng->addPeer(pnd->dataMng->selfPeer());
    pnd->tskMng->addPeer(pnd->logMng->selfPeer());
    for (unsigned int k = 0; k < pnd->tskAgents.size(); ++k) {
        pnd->tskMng->addPeer(pnd->tskAgents.at(k)->selfPeer());
    }

    // - Task Orchestrator
    pnd->tskOrc->addPeer(pnd->evtMng->selfPeer());
    pnd->tskOrc->addPeer(pnd->tskMng->selfPeer());
    pnd->tskOrc->addPeer(pnd->dataMng->selfPeer());
    pnd->tskOrc->addPeer(pnd->logMng->selfPeer());

    // - Task Agents
    for (unsigned int k = 0; k < pnd->tskAgents.size(); ++k) {
        pnd->tskAgents[k]->addPeer(pnd->tskMng->selfPeer());
        pnd->tskAgents[k]->addPeer(pnd->evtMng->selfPeer());
        pnd->tskAgents[k]->addPeer(pnd->logMng->selfPeer());
    }
}

//----------------------------------------------------------------------
// Method: launchPeerNodes
// Launches the different nodes execution, either by creating a new
// thread or by spawning a new process
//----------------------------------------------------------------------
void Deployer::launchPeerNodes()
{
    ConfigurationInfo & cfgInfo = cfg->getCfgInfo();

    L("Running as " << cfgInfo.currentUser << " @ " << cfgInfo.currentMachine);

    // Launch all (as processes or as threads)
    // Event Manager launched always as a thread
    L("Launching nodes . . .");
    for (unsigned int k = 0; k < cfgInfo.peerNodes.size(); ++k) {
        CommNode * node = cfgInfo.peerNodes.at(k);
        node->initialize();
        L("   ====> Initialising " << node->selfPeer()->name);
        usleep(usec);
        if (node->selfPeer()->type == "evtmng") {
            node->create();
            evtMng = dynamic_cast<EventManager*>(node);
            L("   ====> Creating " << node->selfPeer()->name);
        } else {
            if (node->selfPeer()->type == "taskorc") {
                TaskOrchestrator * taskOrc = dynamic_cast<TaskOrchestrator*>(node);
                taskOrc->defineOrchestrationParams(cfgInfo.orcParams);
            }
            if (spawnPeerProcesses) {
                childrenPids.push_back(node->spawn(node));
                L("   ====> Spawning " << node->selfPeer()->name);
            } else {
                node->create();
                L("   ====> Creating " << node->selfPeer()->name);
            }
        }
        usleep(usec);
    }
}

//----------------------------------------------------------------------
// Method: start
// Starts the entire system execution
//----------------------------------------------------------------------
void Deployer::start()
{
    // Send the GO! signal
    if (hmiNeeded) {
        L("Waiting for START signal . . .");
        while (waitingForGoAhead()) { usleep(10000); }
        L("GO!");
        evtMng->go();
    } else {
        L("Starting...");
    }

    while(true) {}
}

//----------------------------------------------------------------------
// Method: cleanUp
// Removes any remaining children
//----------------------------------------------------------------------
void Deployer::cleanUp()
{
    // Kill remaining children
    for (unsigned int i = 0; i < childrenPids.size(); ++i) {
        kill(childrenPids.at(i), SIGTERM);
    }
    L("Exiting . . .");
}

//----------------------------------------------------------------------
// Method: fexists
// Checks that a file exists
//----------------------------------------------------------------------
bool Deployer::fexists(const char * name)
{
    struct stat buffer;
    return (stat(name, &buffer) == 0);
}

//----------------------------------------------------------------------
// Method: waitingForGoAhead
// Returns true if the "Go ahead" message (existence of a given file)
// is received
//----------------------------------------------------------------------
bool Deployer::waitingForGoAhead()
{
    static const char evtmngGoFile[] = "/tmp/EventManager.GO.msg";
    bool keepWaiting = !fexists(evtmngGoFile);
    if (!keepWaiting) { unlink(evtmngGoFile); }
    return keepWaiting;
}

//----------------------------------------------------------------------
// Method: removeOldFiles
// Removes old log and msg files
//----------------------------------------------------------------------
void Deployer::removeOldFiles()
{
    (void)system("rm -rf /tmp/*.log");
    (void)system("rm -rf /tmp/*.msg");
}

}
