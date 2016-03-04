/******************************************************************************
 * File:    procelem.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.procelem
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
 *   Implement ProcessingElement class
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
#include "procelem.h"
#include "tools.h"
#include "taskagent.h"
#include "urlhdl.h"

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <random>
#include <dirent.h>
#include <cassert>

namespace QPF {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ProcessingElement::ProcessingElement(TaskAgent * parent) :
    super(parent),
    status(TASK_SCHEDULED),
    startProc(false),
    endProc(false),
    checkStartSleepPeriod( 500000), // microseconds
    threadLoopSleepPeriod(3000000)  // microseconds
#ifdef DEBUG_BUILD
    ,chkLevel(0)
#endif
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
ProcessingElement::~ProcessingElement()
{
//    peThread.join();
}

//----------------------------------------------------------------------
// Method: setTaskInfo
// Sets the task information for the processing element execution
//----------------------------------------------------------------------
void ProcessingElement::setTaskInfo(TaskInfo & t)
{
    mutexTask.lock();
    task.setData(t.getData());
    mutexTask.unlock();
}

//----------------------------------------------------------------------
// Method: getStatus
// Returns the processing element status
//----------------------------------------------------------------------
TaskStatus ProcessingElement::getStatus()
{
    return status;
}

//----------------------------------------------------------------------
// Method: getTask
// Returns the TaskInfo structure
//----------------------------------------------------------------------
TaskInfo & ProcessingElement::getTask()
{
    return task;
}

//----------------------------------------------------------------------
// Method: start
// Flags the start of the processor
//----------------------------------------------------------------------
void ProcessingElement::start()
{
    startProc = true;
}

//----------------------------------------------------------------------
// Method: end
// Requests the end of the processor
//----------------------------------------------------------------------
void ProcessingElement::end()
{
    endProc = true;
}

//----------------------------------------------------------------------
// Method: exec
// Executes a task
//----------------------------------------------------------------------
void ProcessingElement::exec()
{
    CHKIN;
    // START

    //--------------------------------------------------
    // 1. Send initial Task information (for storage)
    //--------------------------------------------------
    initTaskInfo();

    //--------------------------------------------------
    // 2. Build config files and dir structure
    //--------------------------------------------------
    configureProcElem();

    //--------------------------------------------------
    // Execute processor in a separate thread
    //--------------------------------------------------
    if (status != TASK_FAILED) {
        peThread = std::thread(&ProcessingElement::executeProcessor, this);
        peThread.detach();
    }
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: executeProcessor
// Forks process to run processor
//----------------------------------------------------------------------
void ProcessingElement::executeProcessor()
{
    CHKIN;
    //--------------------------------------------------
    // 3. Fork process to run actual processor
    //--------------------------------------------------
    while (!startProc) { goIdle(checkStartSleepPeriod); }
    forkProcess();

    //--------------------------------------------------
    // 4. Get Identifier
    //--------------------------------------------------
    obtainProcElemId();

    //--------------------------------------------------
    // 5. Monitor running processing element (loop)
    //--------------------------------------------------
    {
        monitorProcElemLoop();
    }

    //--------------------------------------------------
    // 6. Retrieve output products for archive
    //--------------------------------------------------
    retrieveOutputProducts();

    //--------------------------------------------------
    // 7. Clean-up
    //--------------------------------------------------
    cleanup();

    // END
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: initTaskInfo
// Initializes task information
//----------------------------------------------------------------------
void ProcessingElement::initTaskInfo()
{
    CHKIN;
    // Save actual processing element to be executed
    pe = task.taskPath;

    // Define task name and exchange dir.
    nameFromOrchestrator = task.taskName;
    internalTaskNameIdx = (agentName + "-" +
                           LibComm::timeTag() + "-" +
                           LibComm::toStr<int>(numTask));

    mutexTask.lock();

    Json::Value & taskData = task.taskData;

    task.taskStart    = LibComm::timeTag();
    task.taskEnd      = "";
    task.taskExitCode = 0;
    task.taskName     = internalTaskNameIdx;
    task.taskStatus   = status;

    originalRegKey = (getExpandedDateTime(task.taskStart) + "-" +
                      LibComm::toStr<int>(numTask));

    taskData["TaskAgent"]    = agentName;
    taskData["Name"]         = "/Name-not-yet-provided";
    taskData["NameExtended"] = (taskData["TaskAgent"].asString() +
                                ":/" + taskData["Name"].asString());
    taskData["NameInternal"] = internalTaskNameIdx;
    taskData["NameOrc"]      = nameFromOrchestrator;

    taskData["Key"]    = "";
    taskData["RegKey"] = originalRegKey;

    taskData["State"]["Dead"]       = false;
    taskData["State"]["Error"]      = "";
    taskData["State"]["ExitCode"]   = 0;
    taskData["State"]["StartedAt"]  = "0001-01-01T00:00:00Z";
    taskData["State"]["FinishedAt"] = "0001-01-01T00:00:00Z";
    taskData["State"]["OOMKilled"]  = false;
    taskData["State"]["Paused"]     = false;
    taskData["State"]["Pid"]        = 0;
    taskData["State"]["Progress"]   = "0";
    taskData["State"]["Restarting"] = false;
    taskData["State"]["Running"]    = false;
    taskData["State"]["TaskStatus"] = (int)(status);

    // Clear outputs (will be filled in at the end of the task)
    task.outputs.productList.clear();

    mutexTask.unlock();
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: configureProcElem
// Build config files and dir structure
//----------------------------------------------------------------------
void ProcessingElement::configureProcElem()
{
    CHKIN;
    int mn = 60;
    int mx = 180;

    // Prepare folders
    exchangeDir = workDir + "/" + internalTaskNameIdx;
    exchgIn     = exchangeDir + "/in";
    exchgOut    = exchangeDir + "/out";

    std::string sysBinDir = sysDir + "/bin";
    taskDriver  = sysDir + "/bin/runTask.sh";
    cfgFile     = exchangeDir + "/dummy.cfg";

    // Create exchange area
    mkdir(exchangeDir.c_str(), 0755);
    mkdir(exchgIn.c_str(),     0755);
    mkdir(exchgOut.c_str(),    0755);

    // Create input files
    if (task.taskPath == "QLA_VIS") {
        //std::string sourceImg("/qpf/data/mef.fits");
        std::map<ProductType, ProductMetadata>::iterator it =
                task.inputs.productList.begin();
        ProductMetadata & m = it->second;
        std::string sourceImg = m.url.substr(7,1000);
        std::string inputProduct = exchgIn + "/" + m.productId + ".fits";
        URLHandler urlh;
        // Next, we place the data to be analyzed in the task inputs folder
        // With the Host+2 VM Proc.Nodes the symlink version freezes the VMs
        // and the copyfile version eats too much disk space, and is terribly
        // slow.
        // We must find a solution, but in the meanwhile we make a symlink to
        // a VM resident file (with it is in fact identical to the original
        // source)

// Symlink version
//        if (symlink(sourceImg.c_str(), inputProduct.c_str()) != 0) {
//            perror(std::string("symlink input product: from " + sourceImg +
//                               " to " + inputProduct).c_str());
// Copyfile version
//        if (urlh.copyfile(sourceImg, inputProduct) != 0) {
//            perror(std::string("copygile input product: from " + sourceImg +
//                               " to " + inputProduct).c_str());
        if (link("/qpf/data/mef.fits", inputProduct.c_str()) != 0) {
            perror(std::string("link input product: from " + sourceImg +
                               " to " + inputProduct).c_str());
            status = TASK_FAILED;
        }
    } else {
        std::ofstream oInFile1((exchgIn + "/1.in").c_str());
        if (oInFile1.good()) {
            oInFile1 << mn << " " << mx << std::endl;
            oInFile1.close();
        } else {
            std::cerr << "Cannot create " << exchgIn  << "/1.in\n";
            status = TASK_FAILED;
        }
    }
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: forkProcess
// Forks process to run processor
//----------------------------------------------------------------------
void ProcessingElement::forkProcess()
{
    CHKIN;
    // Execute task driver
    pid_t pid = fork();

    if (pid < 0) {
        // Error, failed to fork()
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // We are the child
        char *procTaskCmdLine[] = { (char*)(taskDriver.c_str()),
                                    (char*)(pe.c_str()),
                                    (char*)(cfgFile.c_str()), NULL };
        std::string cmdLine = taskDriver + " " + pe + " " + cfgFile;
        DBG("Trying to execute: '" << cmdLine << "'");
        execv(procTaskCmdLine[0], procTaskCmdLine);
        exit(EXIT_FAILURE);
    }

    status = TASK_RUNNING;
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: obtainProcElemId100.
// Obtain processing element identifier
//----------------------------------------------------------------------
void ProcessingElement::obtainProcElemId()
{
    CHKIN;
    // Get dockerId
    dockerIdFile = exchangeDir + "/docker.id";
    dockerId = "";
    do {
        std::ifstream ifDockerId(dockerIdFile, std::ifstream::in);
        if (ifDockerId.good()) { ifDockerId >> dockerId; }
        ifDockerId.close();
    } while (dockerId.empty());
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: monitorProcElemLoop
// Monitorice running processing element
//----------------------------------------------------------------------
void ProcessingElement::monitorProcElemLoop()
{
    CHKIN;
    // Monitor executing task
    Json::Reader jsonReader;

    estimatedDuration = 100;
    progressValue     = 0.;

    Json::Value & taskData = task.taskData;

    do {
        // Sleep for some time
        goIdle(threadLoopSleepPeriod);

        // Get monitoring information from docker task
        status = TASK_UNKNOWN_STATE; // assumed state

        inspectInfo = getMonitoringInfo(dockerId);
        if (inspectInfo == "ERROR") {
            status = TASK_FINISHED;
        } else {
            Json::Value td(Json::arrayValue);
            if  (jsonReader.parse(inspectInfo, td)) {
                taskData = td[0u];
            } else {
                status = TASK_FAILED;
            }
        }

        if (status == TASK_UNKNOWN_STATE) {
            // Check  flags from docker container info
            running  = taskData["State"]["Running"].asBool();
            dead     = taskData["State"]["Dead"].asBool();
            paused   = taskData["State"]["Paused"].asBool();
            killed   = taskData["State"]["OOMKilled"].asBool();
            excode   = taskData["State"]["ExitCode"].asInt();
            finished = (taskData["State"]["FinishedAt"].asString()
                        != "0001-01-01T00:00:00Z");

            if (finished) {
                status = (excode == 0) ? TASK_FINISHED : TASK_FAILED;
            } else if (dead || killed) {
                status = TASK_FAILED;
            } else if (paused) {
                status = TASK_PAUSED;
            } else {
                status = TASK_RUNNING;
                assert(running);
            }
        }


        mutexTask.lock();

        task.taskExitCode = excode;

        // Update progress
        if (status == TASK_FINISHED) {
            taskData["State"]["Progress"] = "100";
            task.taskEnd = getSimplifiedDateTime(taskData["State"]["FinishedAt"]);
        } else {
            progressValue = updateProgress(progressValue);
            taskData["State"]["Progress"] = LibComm::toStr<int>(floor(progressValue));
        }

        // Additional info
        taskData["TaskAgent"]    = agentName;
        taskData["NameExtended"] = agentName + ":/" + taskData["Name"].asString();
        taskData["NameInternal"] = internalTaskNameIdx;
        taskData["NameOrc"]      = nameFromOrchestrator;

        taskData["Key"]          = dockerId;
        taskData["RegKey"]       = originalRegKey;

        // Set actual start time
        task.taskStart = getSimplifiedDateTime(taskData["State"]["StartedAt"]);

        // Incorporate taskData to task data structure
        task.taskStatus = status;
        taskData["State"]["TaskStatus"] = (int)(status);

        // Send updated task progress and status info
        sendUpdatedInfo();

        mutexTask.unlock();

        childEnded = ((status == TASK_FAILED) ||
                      (status == TASK_FINISHED) ||
                      (status == TASK_STOPPED));
    } while ((!childEnded) && (!endProc));

    CHKOUT;
}

//----------------------------------------------------------------------
// Method: retrieveOutputProducts
// Gets information about output product files for archival purposes
//----------------------------------------------------------------------
void ProcessingElement::retrieveOutputProducts()
{
    CHKIN;
    DBG("Retrieving output products for task: " << originalRegKey);

    //-------------------------------------------------------------------
    // Get output data
    //-------------------------------------------------------------------
    std::vector<std::string> outFiles;
    DIR * dp = NULL;
    struct dirent * dirp;
    if ((dp = opendir(exchgOut.c_str())) == NULL) {
        std::cerr << "Cannot open output directory " << exchgOut << std::endl;
    } else {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name[0] != '.') {
                outFiles.push_back(std::string(dirp->d_name));
            }
        }
        closedir(dp);
    }

    task.outputs.productList.clear();

    URLHandler urlh;
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    for (unsigned int i = 0; i < outFiles.size(); ++i) {
        ProductMetadata m;
        std::string & outFileName = outFiles.at(i);
        if (outFileName.substr(0, 5).compare("EUCL_") == 0) {
            // Set metadata for output product
            m.startTime = task.taskStart;
            m.endTime  = task.taskEnd;
            m.creator = task.taskPath;
            m.instrument = "VIS";
            m.productType = outFileName.substr(5,7);
            m.productSize = 1234;
            m.productStatus = "OK";
            m.productVersion = "1";
            m.productId = outFileName.substr(0, 47);
            m.regTime = LibComm::timeTag();
            m.url = ("file://" +
                     cfgInfo.storage.shared.external_path + "/out/" +
                     outFileName);

            // Place output product at external (output) shared area
            urlh.setProduct(m);
            std::string relFileName = cfgInfo.storage.shared.external_path + "/out/" +
                                      outFileName;
            urlh.relocate(outFileName, relFileName, 0, COPY);
        } else {
            // Set metadata for output product
            m.startTime = task.taskStart;
            m.endTime  = task.taskEnd;
            m.creator = task.taskPath;
            m.instrument = "UNKNOWN_INST";
            m.productType = "UNKNOWN_TYPE";
            m.productSize = 1234;
            m.productStatus = "OK";
            m.productVersion = "1";
            m.productId = ("EUCL_" +
                           m.productType + "_" +
                           m.startTime  + "-" + m.endTime  + "_10");
            m.regTime = LibComm::timeTag();
            m.url = "http://euclid.esa.int/data/" + m.productId + ".zip";
        }

        DBG("  output product: " << outFileName << " => " << m.productId);
        task.outputs.productList[m.productType] = m;
    }

    CHKOUT;
}

//----------------------------------------------------------------------
// Method: sendUpdatedInfo
// Sends updated information about the running task
//----------------------------------------------------------------------
void ProcessingElement::sendUpdatedInfo()
{
    CHKIN;
    static float lastProgress = -1.;

    bool sendMsg = ((status == TASK_FINISHED) ||
                    (status == TASK_FAILED) ||
                    (status == TASK_PAUSED) ||
                    (status == TASK_STOPPED) ||
                    ((status == TASK_RUNNING) &&
                     (progressValue != lastProgress)));

    if (sendMsg) { super->sendTaskResMsg(task); }
    CHKOUT;
}

//----------------------------------------------------------------------
// Method: cleanup
// Finish processing element activities and mark as FINISHED
//----------------------------------------------------------------------
void ProcessingElement::cleanup()
{
    // Cleaning up . . .
    status = TASK_ARCHIVED;
    task.taskStatus = status;
    task.taskData["State"]["TaskStatus"] = (int)(status);
    super->procElemFinished(this);
}

//----------------------------------------------------------------------
// Method: getMonitoringInfo
// Request monitoring information to the running docker container
//----------------------------------------------------------------------
std::string ProcessingElement::getMonitoringInfo(std::string id)
{
    CHKIN;
    std::string cmd("docker inspect " + id);
/*
    std::string info("");
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL) { info += buffer; }
        }
        pclose(pipe);
    }
*/
    std::string info("");
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        char buffer[10240];
        fread(buffer, 1, sizeof(buffer), pipe);
        pclose(pipe);
        info = std::string(buffer);
    }
    if (info.empty()) { info = "ERROR"; }
    CHKOUT;
    return info;
}

//----------------------------------------------------------------------
// Method: updateProgress
// Update progress information
//----------------------------------------------------------------------
float ProcessingElement::updateProgress(float f)
{
    CHKIN;
    float p = f;

    if (status == TASK_RUNNING) {
        p = f + 2;
        if (p > 99.) { p = 99.; }
    }

    CHKOUT;
    return p;
}

//----------------------------------------------------------------------
// Method: getSimplifiedDateTime
// Remove separators from Docker datatime string
//----------------------------------------------------------------------
std::string ProcessingElement::getSimplifiedDateTime(Json::Value v)
{
    std::string s = v.asString();
    return (s.substr(0,4) + s.substr(5,2) +
            s.substr(8,5) +
            s.substr(14,2) + s.substr(17,2));
}

//----------------------------------------------------------------------
// Method: getExpandedDateTime
// Add separators to timeTag date+time string
//----------------------------------------------------------------------
std::string ProcessingElement::getExpandedDateTime(std::string s)
{
    return (s.substr(0,4) + "-" + s.substr(4,2) + "-" +
            s.substr(6,5) + ":" + s.substr(11,2) + ":" + s.substr(13,2) +
            "Z");
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Method: goIdle
// Sleep running thread for a number of microseconds
//----------------------------------------------------------------------
void ProcessingElement::goIdle(int us)
{
    CHKIN;
    std::this_thread::sleep_for(std::chrono::microseconds(us));
    CHKOUT;
}

}
