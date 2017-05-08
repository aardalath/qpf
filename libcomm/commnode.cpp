/******************************************************************************
 * File:    commnode.cpp
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.CommNode
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
 *   Implement CommNode class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   Thread, StateMachine, Router2RouterPeer
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - cstdio
//   - cstdlib
//   - cstring
//   - fstream
//------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - common.h
//   - commnode.h
//------------------------------------------------------------

#include "common.h"
#include "commnode.h"
#include "tools.h"
using namespace LibComm;

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//----------------------------------------------------------------------
// Constructor: CommNode
//----------------------------------------------------------------------
CommNode::CommNode(const char * name) :
    commNodeName(std::string(name)),
    sizeOfLogBuffer(50),
    numOfLogMsgsInBuffer(0)
{
    Log::defineSizeOfLogBuffer(commNodeName, getSizeOfLogBuffer());
}

//----------------------------------------------------------------------
// Destructor: ~CommNode
// Virtual destructor
//----------------------------------------------------------------------
CommNode::~CommNode()
{
}

//----------------------------------------------------------------------
// Method: dump
// Dump stored information
//----------------------------------------------------------------------
void CommNode::dump()
{
}

//----------------------------------------------------------------------
// Method: init
// Initialize the component
//----------------------------------------------------------------------
void CommNode::init()
{
}

//----------------------------------------------------------------------
// Method: run
// (Virtual) method executed when the thread is created
//----------------------------------------------------------------------
int CommNode::run()
{
    while (true) { /* endless loop */ }
    return THR_DONE;
}

//----------------------------------------------------------------------
// Method: logMsg
// Generate log message and provides periodically the set of generated
// messages to the Manager node
//----------------------------------------------------------------------
void CommNode::logMsg(std::string s, Log::LogLevel lvl, bool flush)
{
    Log::log(commNodeName, lvl, s);
    ++numOfLogMsgsInBuffer;

    if (sizeOfLogBuffer > 0) {
        if ((numOfLogMsgsInBuffer >= sizeOfLogBuffer) || (flush)) {
            flushLog();
        }
    }
}

//----------------------------------------------------------------------
// Method: flushLog
// Send the content of the log buffer
//----------------------------------------------------------------------
void CommNode::flushLog()
{
    if (sizeOfLogBuffer == 0) { return; }

    // Get last bunch of messages
    std::vector<std::string> lastMsgs;
    Log::getLastLogMsgs(commNodeName, lastMsgs);
    logChunk = join(lastMsgs, "\n");

    // Reset list of messages
    lastMsgs.clear();
    numOfLogMsgsInBuffer = 0;
    Log::defineSizeOfLogBuffer(commNodeName, getSizeOfLogBuffer());
}

//----------------------------------------------------------------------
// Method: defineValidTransitions
// Define valid state transitions
//----------------------------------------------------------------------
void CommNode::defineValidTransitions()
{
    defineState(ERROR,        "ERROR");
    defineState(OFF,          "OFF");
    defineState(INITIALISED,  "INITIALISED");
    defineState(RUNNING,      "RUNNING");
    defineState(OPERATIONAL,  "OPERATIONAL");

    defineValidTransition(ERROR,        OFF);
    defineValidTransition(OFF,          INITIALISED);
    defineValidTransition(INITIALISED,  RUNNING);
    defineValidTransition(INITIALISED,  OFF);
    defineValidTransition(INITIALISED,  ERROR);
    defineValidTransition(RUNNING,      OPERATIONAL);
    defineValidTransition(RUNNING,      OFF);
    defineValidTransition(RUNNING,      ERROR);
    defineValidTransition(OPERATIONAL,  RUNNING);
    defineValidTransition(OPERATIONAL,  OFF);
    defineValidTransition(OPERATIONAL,  ERROR);

    setState(OFF);
}

}
