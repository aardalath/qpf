/******************************************************************************
 * File:    logmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.logmng
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
 *   Implement LogManager class
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

#include "logmng.h"

#include "log.h"
using LibComm::Log;

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
LogManager::LogManager(const char * name) :
    Component(name)
{
    canProcessMessage(MSG_DATA_INFO_IDX);
    Log::defineSizeOfLogBuffer(name, 0);
}

//----------------------------------------------------------------------
// Method: processDATA_INFO
//----------------------------------------------------------------------
void LogManager::processDATA_INFO()
{
    // Check the product type as input for any rule
    Message_DATA_INFO * msg = dynamic_cast<Message_DATA_INFO *>(msgData.msg);
    msg->toFields();
    std::string & logChunk = msg->variables.paramList["contents"];
    std::string logFileName(Log::getLogBaseDir() + "/rlog/" +
                            msg->header.source + ".log");
    std::ofstream logOut;
    logOut.open(logFileName, std::ofstream::out | std::ofstream::app);
    if (logOut) {
        logOut << logChunk;
        logOut.close();
    }
}

//----------------------------------------------------------------------
// Method: execAdditonalLoopTasks
//----------------------------------------------------------------------
void LogManager::execAdditonalLoopTasks()
{

}

}
