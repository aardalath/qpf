/******************************************************************************
 * File:    master.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.master
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement Master class
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

#include "master.h"

#include "filenamespec.h"
#include "message.h"
#include "hostinfo.h"

#include "config.h"

using Configuration::cfg;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Master::Master(const char * name, const char * addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Master::Master(std::string name, std::string addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Method: init
//----------------------------------------------------------------------
void Master::init()
{
    // Create rest of components
    datMng = new DataMng ("DatMng", compAddress, synchro);
    logMng = new LogMng  ("LogMng", compAddress, synchro);
    tskOrc = new TskOrc  ("TskOrc", compAddress, synchro);

    subComponents = {evtMng, datMng, logMng, tskOrc, tskMng};
    
    requestQuit = false;
}

//----------------------------------------------------------------------
// Method: setEvtMng
//----------------------------------------------------------------------
void Master::setEvtMng(EvtMng * obj)
{
    evtMng = obj;
}

//----------------------------------------------------------------------
// Method: setTskMng
//----------------------------------------------------------------------
void Master::setTskMng(TskMng * obj)
{
    tskMng = obj;
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void Master::fromRunningToOperational()
{
    // Retrieve task status spectra table
    datMng->retrieveTaskStatusSpectra(tssSet);
    sleep(1);
    tskMng->initializeTaskStatusSpectra(tssSet);
    
    // Go to OPERATIONAL
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void Master::fromOperationalToRunning()
{
    // Transit from Running to OFF
    InfoMsg("New state: " + getStateName(getState()));
    transitTo(OFF);
}

//----------------------------------------------------------------------
// Method: runEachIteration
//----------------------------------------------------------------------
void Master::runEachIteration()
{
    // 1. Check input products
    std::vector<TaskInfo> tasks;

    ProductList inData;
    std::string space;
    if (evtMng->getInData(inData, space)) {

        // There are input products
        TRC("There are input products!");
        
        // a. If not reprocessing, store them in local archive and save
        //    metadata to DB
        if (space != ReprocessingSpace) {
            datMng->txInDataToLocalArch(inData);
            TRC("Archiving inbox products");
        }

        // b. Generate tasks for processing these products
        tskOrc->createTasks(inData, tasks);

    }

    ProductList reprocData;
    if (evtMng->getReprocData(reprocData)) {

        // There are input products
        TRC("There are products to be reprocessed!");
        
        // b. Generate tasks for processing these products
        tskOrc->createTasks(reprocData, tasks);

    }

    if (tasks.size() > 0) {
        
        TRC("Created " + std::to_string(tasks.size()) + "tasks");
        
        // c. Store tasks information into DB (initialStore = true)
        TRC("Archiving tasks info");
        for (auto & task: tasks) { datMng->saveTaskToDB(task, true); }

        // d. Schedule tasks (Task Manager will send them to requesters)
        TRC("Scheduling tasks");
        for (auto & task: tasks) { tskMng->scheduleTask(task); }
        
    }

    // 2. Check QUIT request
    if (evtMng->isQuitRequested()) {
        evtMng->quit();
        transitTo(RUNNING);
    }

    // 3. Retrieve and store Task reports
    json tskRepData;
    if (tskMng->getTskRepUpdate(tskRepData)) {
        datMng->storeTskRegData(tskRepData);
    }
    
    // 4. Retrieve and send FMK monitoring information
    if (evtMng->isHMIActive()) {
        json fmkInfoValue;
        tskMng->getProcFmkInfoUpdate(fmkInfoValue);
        evtMng->sendProcFmkInfoUpdate(fmkInfoValue);
        datMng->storeTaskStatusSpectra(fmkInfoValue);
    }   
}

