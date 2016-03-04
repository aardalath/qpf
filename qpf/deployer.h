/******************************************************************************
 * File:    deployer.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.Deployer
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
 *   Declare Deployer class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   CommNode
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

#ifndef DEPLOYER_H
#define DEPLOYER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - map
//   - vector
//   - fstream
//------------------------------------------------------------

#include <iostream>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------

#include "evtmng.h"
#include "datamng.h"
#include "logmng.h"
#include "taskmng.h"
#include "taskagent.h"
#include "taskorc.h"
#include "config.h"
#include "tools.h"

using namespace QPF;

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: Deployer
//==========================================================================
class Deployer {

public:

    //----------------------------------------------------------------------
    // Constructor: Deployer
    //----------------------------------------------------------------------
    Deployer();

    //----------------------------------------------------------------------
    // Method: run
    // Minimal working example
    // The elements are launched and wait for the start from the
    // Event Manager.
    // Then the systems starts the normal operational cycle.
    // Current version of Event Manager takes external events from
    // the HMI (simulated external events)
    //----------------------------------------------------------------------
    int run(int argc, char * argv[]);

private:

    //----------------------------------------------------------------------
    // Type: QPFConfig (struct)
    // Holds the configuration of the QPF system to be deployed
    //----------------------------------------------------------------------
    struct QPFConfig {
        std::string appName;
        std::string appVersion;
        std::string lastAccess;
        TaskOrchestrator::OrchestrationParameters     orcParams;
        std::vector<std::string>                      peerNames;
        std::vector<LibComm::Router2RouterPeer::Peer> peersCfg;
        LibComm::Router2RouterPeer::Peer              qpfhmiCfg;
    };

    //----------------------------------------------------------------------
    // Type: PeerNodesData (struct)
    // Holds the specifications of the different nodes part of the system
    //----------------------------------------------------------------------
    struct PeerNodesData {
        PeerNodesData() :
            evtMng(0), dataMng(0), logMng(0), tskMng(0), tskOrc(0) {}
        EventManager *           evtMng;
        DataManager *            dataMng;
        LogManager *             logMng;
        TaskManager *            tskMng;
        TaskOrchestrator *       tskOrc;
        std::vector<TaskAgent *> tskAgents;
        std::vector<CommNode *>  nodes;
    };

private:

    //----------------------------------------------------------------------
    // Function: launchQPFHMI
    // Launches (fork) HMI as a separate process
    //----------------------------------------------------------------------
    pid_t launchQPFHMI();

    //----------------------------------------------------------------------
    // Function: readConfig
    // Reads configuration file
    //----------------------------------------------------------------------
    void readConfig(QPFConfig & qpfCfg, const char * configFile = 0);

    //----------------------------------------------------------------------
    // Function: createPeerNodes
    // Creates the peer node objects with the information from the system
    // configuration
    //----------------------------------------------------------------------
    void createPeerNodes(QPFConfig & qpfCfg, PeerNodesData & pnd);

    //----------------------------------------------------------------------
    // Function: createPeerNodeConnections
    // Creates the node connections
    //----------------------------------------------------------------------
    void createPeerNodeConnections(QPFConfig & qpfCfg, PeerNodesData & pnd);

    //----------------------------------------------------------------------
    // Function: launchPeerNodes
    // Launches the different nodes execution, either by creating a new
    // thread or by spawning a new process
    //----------------------------------------------------------------------
    void launchPeerNodes(QPFConfig qpfCfg, PeerNodesData & pnd,
                         std::vector<pid_t> & childrenPids);

    //----------------------------------------------------------------------
    // Function: start
    // Starts the entire system execution
    //----------------------------------------------------------------------
    void start(PeerNodesData & pnd, pid_t qpfhmiPid);

    //----------------------------------------------------------------------
    // Function: cleanUp
    // Removes any remaining children
    //----------------------------------------------------------------------
    void cleanUp(std::vector<pid_t> & childrenPids);

    //----------------------------------------------------------------------
    // Function: fexists
    // Checks that a file exists
    //----------------------------------------------------------------------
    bool fexists(const char * name);

    //----------------------------------------------------------------------
    // Function: waitingForGoAhead
    // Returns true if the "Go ahead" message (existence of a given file)
    // is received
    //----------------------------------------------------------------------
    bool waitingForGoAhead();

    //----------------------------------------------------------------------
    // Function: removeOldFiles
    // Removes old log and msg files
    //----------------------------------------------------------------------
    void removeOldFiles();

    //----------------------------------------------------------------------
    // Method: processCmdLineOpts
    // Processes command line options to configure execution
    //----------------------------------------------------------------------
    bool processCmdLineOpts(int argc, char * argv[]);

private:

    //----------------------------------------------------------------------
    // Flags: Execution options
    //   spawnPeerProcesses - Used to create the components as processes
    //                        instead of threads
    //   waitingForSignalToGo - Control flag to start execution
    //   verboseOutput - Controls if log messages are dumped to console
    //----------------------------------------------------------------------
    bool        spawnPeerProcesses;
    bool        waitingForSignalToGo;
    bool        verboseOutput;

    //----------------------------------------------------------------------
    // Variable: usec
    // Time lapse in microseconds from the launch of one component until the
    // launch of the next component
    //----------------------------------------------------------------------
    useconds_t  usec;

    //----------------------------------------------------------------------
    // Variable: newConfigFile
    // Holds the file name of the configuration file where the new configuration
    // is stored (in case of reconfiguration, =-c newConfigFile= is used)
    //----------------------------------------------------------------------
    std::string newConfigFile;
};

}

#endif  /* DEPLOYER_H */
