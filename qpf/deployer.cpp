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
Deployer::Deployer() :
    spawnPeerProcesses(false),
    waitingForSignalToGo(true),
    verboseOutput(false),
    usec(50000),
    newConfigFile(std::string())
{
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
    while ((opt = getopt(argc, argv, "hpvt:c:")) != -1) {
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
// Method: run
// Minimal working example
// The elements are launched and wait for the start from the
// Event Manager.
// Then the systems starts the normal operational cycle.
// Current version of Event Manager takes external events from
// the HMI (simulated external events)
//----------------------------------------------------------------------
int Deployer::run(int argc, char * argv[])
{
    // Change value for delay between peer nodes launches (default: 50000us)
    if (!processCmdLineOpts(argc, argv)) {
        return(EXIT_FAILURE);
    }

    //===== Initial (quick and dirty) cleanup =====
    removeOldFiles();
    LibComm::Log::setConsoleOutput(verboseOutput);

    //===== Read Configuration =====
    QPFConfig qpfCfg;
    const char * configFile = 0;
    if (!newConfigFile.empty()) { configFile = newConfigFile.c_str(); }
    readConfig(qpfCfg, configFile);

    //===== Create peer nodes =====
    PeerNodesData pnd;
    createPeerNodes(qpfCfg, pnd);

    //===== Launch HMI =====
    L("Launching and connecting QPF HMI . . .");
    pid_t qpfhmiPid = launchQPFHMI();

    //===== Create peer node connections =====
    createPeerNodeConnections(qpfCfg, pnd);

    //===== Launch them =====
    std::vector<pid_t> childrenPids;
    launchPeerNodes(qpfCfg, pnd, childrenPids);

    //===== Start System =====
    start(pnd, qpfhmiPid);

    //===== System cleanup at shutdown =====
    cleanUp(childrenPids);

    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------
// Method: launchQPFHMI
// Launches (fork) HMI as a separate process
//----------------------------------------------------------------------
pid_t Deployer::launchQPFHMI()
{
    pid_t pid = fork();

    if (pid < 0) {
        // Error, failed to fork()
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // We are the child
        char * qpfhmiExe = getenv("QPFHMI");
        //char * newArgv[] = { qpfhmiExe, "-style", "fusion", NULL };
        char * newArgv[] = { qpfhmiExe, NULL };
        execv(newArgv[0], newArgv);
        L("execv: " << strerror(errno));
        exit(EXIT_FAILURE);
    }
    return pid;
}

//----------------------------------------------------------------------
// Method: readConfig
// Reads configuration file
//----------------------------------------------------------------------
void Deployer::readConfig(QPFConfig & qpfCfg, const char * configFile)
{
    Configuration * cfg = new Configuration(configFile);

    cfg->getGeneralInfo(qpfCfg.appName, qpfCfg.appVersion, qpfCfg.lastAccess);
    cfg->getProductTypes(qpfCfg.orcParams.productTypes);

    for (int i = 0; i < cfg->getNumOrchRules(); ++i) {
        TaskOrchestrator::Rule * rule = new TaskOrchestrator::Rule;
        cfg->getOrchRule(rule->name, rule->inputs,
                         rule->outputs, rule->processingElement);
        qpfCfg.orcParams.rules.push_back(rule);
    }

    for (int i = 0; i < cfg->getNumProcs(); ++i) {
        TaskOrchestrator::ProcElem * pe = new TaskOrchestrator::ProcElem;
        cfg->getProc(pe->name, pe->exePath, pe->inPath, pe->outPath);
        qpfCfg.orcParams.procElems[pe->name] = pe;
    }

    for (int i = 0; i < cfg->getNumNodes(); ++i) {
        LibComm::Router2RouterPeer::Peer * peer = new LibComm::Router2RouterPeer::Peer;
        cfg->getNode(peer->name, peer->type, peer->clientAddr, peer->serverAddr);
        qpfCfg.peersCfg.push_back(*peer);
        qpfCfg.peerNames.push_back(peer->name);
    }

    qpfCfg.qpfhmiCfg.name = cfg->getHMINodeName();
    cfg->getNodeByName(qpfCfg.qpfhmiCfg.name,
                       qpfCfg.qpfhmiCfg.type,
                       qpfCfg.qpfhmiCfg.clientAddr,
                       qpfCfg.qpfhmiCfg.serverAddr);
}

//----------------------------------------------------------------------
// Method: createPeerNodes
// Creates the peer node objects with the information from the system
// configuration
//----------------------------------------------------------------------
void Deployer::createPeerNodes(QPFConfig & qpfCfg, PeerNodesData & pnd)
{
    L("Creating peer nodes . . .");

    for (unsigned int i = 0; i < qpfCfg.peersCfg.size(); ++i) {

        std::string & peerName = qpfCfg.peersCfg[i].name;
        std::string & peerType = qpfCfg.peersCfg[i].type;

        const char * cpeerName = qpfCfg.peersCfg[i].name.c_str();

        L("Creating peer node " << peerName << " . . .");

        if        (peerType == "evtmng") {
            assert(pnd.evtMng == 0);
            pnd.evtMng = new EventManager(cpeerName);
            pnd.evtMng->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.nodes.push_back(pnd.evtMng);
        } else if (peerType == "datamng") {
            assert(pnd.dataMng == 0);
            pnd.dataMng = new DataManager(cpeerName);
            pnd.dataMng->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.nodes.push_back(pnd.dataMng);
        } else if (peerType == "logmng") {
            assert(pnd.logMng == 0);
            pnd.logMng = new LogManager(cpeerName);
            pnd.logMng->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.nodes.push_back(pnd.logMng);
        } else if (peerType == "taskmng") {
            assert(pnd.tskMng == 0);
            pnd.tskMng = new TaskManager(cpeerName);
            pnd.tskMng->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.nodes.push_back(pnd.tskMng);
        } else if (peerType == "taskorc") {
            assert(pnd.tskOrc == 0);
            pnd.tskOrc = new TaskOrchestrator(cpeerName);
            pnd.tskOrc->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.nodes.push_back(pnd.tskOrc);
        } else if (peerType == "taskagent") {
            TaskAgent * tskAg = new TaskAgent(cpeerName);
            tskAg->addPeer(&(qpfCfg.peersCfg[i]), true);
            pnd.tskAgents.push_back(tskAg);
            pnd.nodes.push_back(tskAg);
        } else {
            // Do nothing, not yet implemented
        }

    }
}

//----------------------------------------------------------------------
// Method: createPeerNodeConnections
// Creates the node connections
//----------------------------------------------------------------------
void Deployer::createPeerNodeConnections(QPFConfig & qpfCfg, PeerNodesData & pnd)
{
    L("Creating peer connections . . .");

    // - Event Manager
    pnd.evtMng->addPeer(pnd.dataMng->selfPeer());
    pnd.evtMng->addPeer(pnd.logMng->selfPeer());
    pnd.evtMng->addPeer(pnd.tskMng->selfPeer());
    pnd.evtMng->addPeer(pnd.tskOrc->selfPeer());
    // Set connection to QPFHMI
    pnd.evtMng->addPeer(&qpfCfg.qpfhmiCfg);
    // TaskAgents must be awaked by the Event Manager
    // (though no other direct messages are expected by them
    //  comming from the Event Manager)
    for (unsigned int k = 0; k < pnd.tskAgents.size(); ++k) {
        pnd.evtMng->addPeer(pnd.tskAgents.at(k)->selfPeer());
    }

    // - Data Manager
    pnd.dataMng->addPeer(pnd.evtMng->selfPeer());
    pnd.dataMng->addPeer(pnd.tskOrc->selfPeer());

    // - Log Manager
    pnd.logMng->addPeer(pnd.evtMng->selfPeer());

    // - Task Manager
    pnd.tskMng->addPeer(pnd.evtMng->selfPeer());
    pnd.tskMng->addPeer(pnd.tskOrc->selfPeer());
    for (unsigned int k = 0; k < pnd.tskAgents.size(); ++k) {
        pnd.tskMng->addPeer(pnd.tskAgents.at(k)->selfPeer());
    }

    // - Task Orchestrator
    pnd.tskOrc->addPeer(pnd.evtMng->selfPeer());
    pnd.tskOrc->addPeer(pnd.tskMng->selfPeer());
    pnd.tskOrc->addPeer(pnd.dataMng->selfPeer());

    // - Task Agents
    for (unsigned int k = 0; k < pnd.tskAgents.size(); ++k) {
        pnd.tskAgents[k]->addPeer(pnd.tskMng->selfPeer());
        pnd.tskAgents[k]->addPeer(pnd.evtMng->selfPeer());
    }
}

//----------------------------------------------------------------------
// Method: launchPeerNodes
// Launches the different nodes execution, either by creating a new
// thread or by spawning a new process
//----------------------------------------------------------------------
void Deployer::launchPeerNodes(QPFConfig qpfCfg, PeerNodesData & pnd,
                               std::vector<pid_t> & childrenPids)
{
    L("Launching nodes . . .");

    // Launch all (as processes or as threads)
    // Event Manager launched as thread
    for (unsigned int k = 0; k < pnd.nodes.size(); ++k) {
        CommNode * node = pnd.nodes.at(k);
        node->initialize();
        L("   ====> Initialising " << node->selfPeer()->name);
        usleep(usec);
        if (node->selfPeer()->type == "evtmng") {
            node->create();
            L("   ====> Creating " << node->selfPeer()->name);
        } else {
            if (node->selfPeer()->type == "taskorc") {
                pnd.tskOrc->defineOrchestrationParams(qpfCfg.orcParams);
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
void Deployer::start(PeerNodesData & pnd, pid_t qpfhmiPid)
{
    // Send the GO! signal
    L("Waiting for START signal . . .");
    //usleep(10000000);
    //while (waitingForSignalToGo) {}
    while (waitingForGoAhead()) { usleep(10000); }
    L("GO!");
    pnd.evtMng->go();

    // Endless loop (just for the test)
    L("Waiting for QPF HMI to end . . .");
    int qpfhmiStatus;
    waitpid(qpfhmiPid, &qpfhmiStatus, 0);

    //L("Endless loop . . .");
    //while(true) {}
}

//----------------------------------------------------------------------
// Method: cleanUp
// Removes any remaining children
//----------------------------------------------------------------------
void Deployer::cleanUp(std::vector<pid_t> & childrenPids)
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
    return !fexists(evtmngGoFile);
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
