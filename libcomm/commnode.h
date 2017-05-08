/******************************************************************************
 * File:    commnode.h
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
 *   Declare CommNode class
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

#ifndef COMMNODE_H
#define COMMNODE_H

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

//------------------------------------------------------------
// Topic: Project headers
//   - statem.h
//   - log.h
//   - r2rpeer.h
//   - thread.h
//------------------------------------------------------------

#include "statem.h"
#include "log.h"
#include "r2rpeer.h"
#include "thread.h"

//============================================================
// Group: Public Declarations
//============================================================

//------------------------------------------------------------
// Topic: Macros
//   - LogMsg(l, s) - Generic macro to call logMsg method
//   - DbgMsg(s)    - Macro to call logMsg for DEBUG msgs
//   - InfoMsg(s)   - Macro to call logMsg for INFO msgs
//   - WarnMsg(s)   - Macro to call logMsg for WARNING msgs
//   - ErrMsg(s)    - Macro to call logMsg for ERROR msgs
//   - FatalMsg(s)  - Macro to call logMsg for FATAL msgs
//------------------------------------------------------------

#ifndef PRESERVE_LOG_SYSTEM
#ifdef LogMsg

#undef  LogMsg
#undef  DbgMsg
#undef  InfoMsg
#undef  WarnMsg
#undef  ErrMsg
#undef  FatalMsg

#define LogMsg(l, s)  logMsg(s, l)
#define DbgMsg(s)     logMsg(s, Log::DEBUG)
#define InfoMsg(s)    logMsg(s, Log::INFO)
#define WarnMsg(s)    logMsg(s, Log::WARNING)
#define ErrMsg(s)     logMsg(s, Log::ERROR)
#define FatalMsg(s)   logMsg(s, Log::FATAL)

#endif
#endif

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//==========================================================================
// Class: CommNode
//==========================================================================
class CommNode :
    public Thread,
    public StateMachine,
    public Router2RouterPeer {

    Property(CommNode, std::string, commNodeName, CommNodeName)
    Property(CommNode, int, sizeOfLogBuffer, SizeOfLogBuffer)

    // Valid Manager states
    static const int ERROR        = -1;
    static const int OFF          =  0;
    static const int INITIALISED  =  1;
    static const int RUNNING      =  2;
    static const int OPERATIONAL  =  3;

public:

    //----------------------------------------------------------------------
    // Constructor: CommNode
    //----------------------------------------------------------------------
    CommNode(const char * name = 0);

    //----------------------------------------------------------------------
    // Destructor: ~CommNode
    // Virtual destructor
    //----------------------------------------------------------------------
    virtual ~CommNode();

    //----------------------------------------------------------------------
    // Method: dump
    // Dump internal state
    //----------------------------------------------------------------------
    void dump();

    //----------------------------------------------------------------------
    // Method: initialize
    // Public way to call initialisation method
    //----------------------------------------------------------------------
    inline void initialize() { init(); }

protected:

    //----------------------------------------------------------------------
    // Method: init
    // Initializes the node
    //----------------------------------------------------------------------
    virtual void init();

    //----------------------------------------------------------------------
    // Method: run
    // Launches the underlying entity in its own thread
    //----------------------------------------------------------------------
    virtual int run();

    //----------------------------------------------------------------------
    // Method: defineValidTransitions
    // Define the valid state transitions for the node
    //----------------------------------------------------------------------
    virtual void defineValidTransitions();

    //----------------------------------------------------------------------
    // Method: logMsg
    // Shows a log message with a given criticality, and sends periodically
    // (flush) the messages to a pre-defined entity
    //
    // Parameters:
    //   s     - (I) The message string
    //   lvl   - (I) The criticality level of the message
    //   flush - (I) Indicates whether to flush the output after msg display
    //----------------------------------------------------------------------
    virtual void logMsg(std::string s, Log::LogLevel lvl = Log::INFO,
            bool flush = true);

    //----------------------------------------------------------------------
    // Method: flushLog
    // Send bunch of last messages to a pre-defined entity
    //----------------------------------------------------------------------
    void flushLog();

protected:

private:

    //----------------------------------------------------------------------
    // Assignment operator
    // Operator to clone a node
    //----------------------------------------------------------------------
    void operator=(CommNode const &);

    //----------------------------------------------------------------------
    // Copy constructor: CommNode
    //----------------------------------------------------------------------
    CommNode(CommNode const &);

protected:
    std::string logChunk;
    int numOfLogMsgsInBuffer;
};

}

#endif  /* COMMNODE_H */
