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
#include <cerrno>

#define L(x) do { std::cerr << x << std::endl << std::flush; } while (0);

#include <sys/stat.h>

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
    hmiNeeded(false),
    deploymentCompleted(false)
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
    while (!deploymentCompleted) {}
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
// Method: getConfigHandler
// Returns the configuration handler
//----------------------------------------------------------------------
Configuration * Deployer::getConfigHandler()
{
    return cfg;
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
    cfg   = new Configuration(configFile);
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    hmiNeeded = cfgInfo.hmiPresent;
}

//----------------------------------------------------------------------
// Method: launchPeerNodes
// Launches the different nodes execution, either by creating a new
// thread or by spawning a new process
//----------------------------------------------------------------------
void Deployer::launchPeerNodes()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    L("Running as " << cfgInfo.currentUser << " @ " << cfgInfo.currentMachine);

    // Launch all (as processes or as threads)
    // Event Manager launched always as a thread
    L("Launching nodes . . .");
    for (unsigned int k = 0; k < cfgInfo.peerNodes.size(); ++k) {
        Component * node = cfgInfo.peerNodes.at(k);
        node->initialize();
        L("   ====> Initialising " << node->selfPeer()->name);
        usleep(usec);
        if (node->selfPeer()->type == "evtmng") {
            node->create();
            evtMng = dynamic_cast<EventManager*>(node);
            L("   ====> Creating " << node->selfPeer()->name);
        } else {
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

    deploymentCompleted = true;
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
        usleep(500000);
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
}

//----------------------------------------------------------------------
// Method: existsDir
// Removes old log and msg files
//----------------------------------------------------------------------
bool Deployer::existsDir(std::string pathName)
{
    struct stat sb;
    return (stat(pathName.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}


}
