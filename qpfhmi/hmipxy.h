/******************************************************************************
 * File:    hmipxy.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.HMIProxy
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
 *   Declare HMIProxy class
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

#ifndef HMIPXY_H
#define HMIPXY_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

#include <mutex>

//------------------------------------------------------------
// Topic: Project headers
//   - component.h
//------------------------------------------------------------

#include "component.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: HMIProxy
//==========================================================================
class HMIProxy : public Component {

public:
    typedef std::map<std::string, Json::Value> TaskResultsInfo;
    typedef std::map<std::string, Json::Value>::iterator TaskResultsInfoIt;

    HMIProxy(const char * name = 0);

    //----------------------------------------------------------------------
    // Method: concurrentRun
    // Method executed when the thread is created (for QtConcurrent)
    //----------------------------------------------------------------------
    int concurrentRun();

    //----------------------------------------------------------------------
    // Method: send
    // Method to send messages to Event Manager
    //----------------------------------------------------------------------
    void sendInData(ProductMetadata metadata);

    //----------------------------------------------------------------------
    // Method: getStartSignal
    // Propagate START signal to node
    //----------------------------------------------------------------------
    void getStartSignal();

    //----------------------------------------------------------------------
    // Method: requestStop
    // Send STOP request to Event Manager
    //----------------------------------------------------------------------
    void requestStop();

    //----------------------------------------------------------------------
    // Method: isThereTaskResInfo
    // Provides a flag to poll for new Task Res msgs
    //----------------------------------------------------------------------
    bool isThereTaskResInfo();

    //----------------------------------------------------------------------
    // Method: getTaskResInfo
    // Provides a vector with the new task res info
    //----------------------------------------------------------------------
    int getTaskResInfo(TaskResultsInfo & newTaskInfo);

protected:

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

    //----------------------------------------------------------------------
    // Method: processTASK_RES
    //----------------------------------------------------------------------
    virtual void processTASK_RES();

    //----------------------------------------------------------------------
    // Method: registerTaskRes
    // Send STOP request to Event Manager
    //----------------------------------------------------------------------
    void registerTaskRes(Json::Value jsonValue);


private:
    TaskResultsInfo taskResInfo;

    std::atomic<bool> thereIsTaskResInfo;
    std::mutex registeringTaskMutex;
    std::atomic<bool> startSignalFromHMIReceived;
};

}

#endif  /* HMIPXY_H */

