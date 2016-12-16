/******************************************************************************
 * File:    component.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.Component
 *
 * Version:  1.1
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare Component class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   MsgTypes, CommNode
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

#ifndef COMPONENT_H
#define COMPONENT_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - set
//------------------------------------------------------------
#include <set>

//------------------------------------------------------------
// Topic: External packages
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - propdef.h
//   - cfginfo.h
//   - alert.h
//------------------------------------------------------------
#include "propdef.h"
#include "cfginfo.h"
#include "alert.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

#define RaiseSysAlert(a)  raise(a, Alert::System)
#define RaiseDiagAlert(a) raise(a, Alert::Diagnostics)
#define RaiseAlert(a)     raise(a)

//==========================================================================
// Class: Component
//==========================================================================
class Component : public CommNode {

public:
    Component(const char * name = 0);
    virtual ~Component() {}

protected:

    //----------------------------------------------------------------------
    // Method: init
    //----------------------------------------------------------------------
    virtual void init();

    //----------------------------------------------------------------------
    // Method: run
    //----------------------------------------------------------------------
    virtual int run();

    //----------------------------------------------------------------------
    // Method: fromInitialisedToRunning
    //----------------------------------------------------------------------
    virtual void fromInitialisedToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

    //----------------------------------------------------------------------
    // Method: execAdditonalLoopTasks
    //----------------------------------------------------------------------
    virtual void execAdditonalLoopTasks() {}

protected:

    //----------------------------------------------------------------------
    // Method: processSTART
    //----------------------------------------------------------------------
    virtual void processSTART() {}

    //----------------------------------------------------------------------
    // Method: processINDATA
    //----------------------------------------------------------------------
    virtual void processINDATA() {}

    //----------------------------------------------------------------------
    // Method: processDATA_RQST
    //----------------------------------------------------------------------
    virtual void processDATA_RQST() {}

    //----------------------------------------------------------------------
    // Method: processDATA_INFO
    //----------------------------------------------------------------------
    virtual void processDATA_INFO() {}

    //----------------------------------------------------------------------
    // Method: processMONIT_RQST
    //----------------------------------------------------------------------
    virtual void processMONIT_RQST();

    //----------------------------------------------------------------------
    // Method: processMONIT_INFO
    //----------------------------------------------------------------------
    virtual void processMONIT_INFO() {}

    //----------------------------------------------------------------------
    // Method: processTASK_PROC
    //----------------------------------------------------------------------
    virtual void processTASK_PROC() {}

    //----------------------------------------------------------------------
    // Method: processTASK_RES
    //----------------------------------------------------------------------
    virtual void processTASK_RES() {}

    //----------------------------------------------------------------------
    // Method: processCMD
    //----------------------------------------------------------------------
    virtual void processCMD() {}

    //----------------------------------------------------------------------
    // Method: processSTOP
    //----------------------------------------------------------------------
    virtual void processSTOP() {}

    //----------------------------------------------------------------------
    // Method: processUNKNOWN
    //----------------------------------------------------------------------
    virtual void processUNKNOWN() {}

    //----------------------------------------------------------------------
    // Method: canProcessMessage
    // Activates the component as able to process the specified message
    //----------------------------------------------------------------------
    void canProcessMessage(MessageId id);

protected:

    //----------------------------------------------------------------------
    // Method: sendMONIT_RQST
    //----------------------------------------------------------------------
    void sendMONIT_RQST(std::string target, std::string what, std::string value);

    //----------------------------------------------------------------------
    // Method: sendCMD
    //----------------------------------------------------------------------
    void sendCMD(std::string target, std::string what, std::string value);

    //----------------------------------------------------------------------
    // Method: is
    //----------------------------------------------------------------------
    bool is(Router2RouterPeer::PeerMessage & inPeerMsg,
            MessageTypeIdx msgType);

    //----------------------------------------------------------------------
    // Method: is
    // Check type of msg
    //----------------------------------------------------------------------
    bool is(Router2RouterPeer::PeerMessage & inPeerMsg,
            std::string msgType);

    //----------------------------------------------------------------------
    // Method: process
    //----------------------------------------------------------------------
    int process(Router2RouterPeer::Peer & inPeer,
                Router2RouterPeer::PeerMessage & inPeerMsg);

    //----------------------------------------------------------------------
    // Method: buildMsgHeader
    //----------------------------------------------------------------------
    bool buildMsgHeader(MessageId msgId,
                        NodeName source,
                        NodeName destination,
                        MessageHeader & hdr);

    //----------------------------------------------------------------------
    // Method: buildMsgINDATA
    //----------------------------------------------------------------------
    bool buildMsgINDATA(MessageHeader & hdr,
                        ProductCollection    prdMetadata,
                        Message_INDATA & msg);

    //----------------------------------------------------------------------
    // Method: buildMsgTASKPROC
    // Builds the TASK_PROC message
    //----------------------------------------------------------------------
    bool buildMsgTASKPROC(MessageHeader & hdr,
                          TaskInfo & task,
                          std::string rule,
                          Message_TASK_PROC & msg);

    //----------------------------------------------------------------------
    // Method: setForwardTo
    // Modify msg header to forward msg to another destination
    //----------------------------------------------------------------------
    bool setForwardTo(NodeName fwdDestination,
                      MessageHeader & header);

    //----------------------------------source------------------------------------
    // Method: convertTo
    // Modify msg header to forward msg to another destination
    //----------------------------------------------------------------------
    bool convertTo(MessageTypeIdx newIdx,
                   NodeName fwdDestination,
                   Message & msg);

protected:

    //----------------------------------------------------------------------
    // Method: registerMsg
    // Store content of message into database
    //----------------------------------------------------------------------
    void registerMsg(std::string from,
                     Router2RouterPeer::PeerMessage & inPeerMsg,
                     bool isBroadcast = false);

    //----------------------------------------------------------------------
    // Method: Method: raise
    // Raise alert, shows it in the log, and stored in DB
    //----------------------------------------------------------------------
    void raise(Alert a, Alert::Group grp = Alert::Undefined);

    //----------------------------------------------------------------------
    // Method: procMsg
    // Process a new T message
    //----------------------------------------------------------------------
    template<class T>
    bool procMsg(Router2RouterPeer::PeerMessage & inPeerMsg);

    //----------------------------------------------------------------------
    // Method: writeToFile
    // Write content of physical message to file
    //----------------------------------------------------------------------
    void writeToFile(Router2RouterPeer::PeerMessage& inPeerMsg);

    //----------------------------------------------------------------------
    // Method: sendLogPacketAsDataInfoMsg
    //----------------------------------------------------------------------
    void sendLogPacketAsDataInfoMsg();

    //----------------------------------------------------------------------
    // Method: setHeartBeatPeriod
    // Sets number of seconds and microseconds for HeartBeat period
    //----------------------------------------------------------------------
    void setHeartBeatPeriod(int s = 2, int us = 0);

    //----------------------------------------------------------------------
    // Method: afterTransition
    //----------------------------------------------------------------------
    virtual void afterTransition(int fromState, int toState);

protected:
    std::set<int> canProcess;
    std::string session;
    MessageData   msgData;
    int hbSecs;
    int hbMicroSecs;
    bool isPeerLogMng;
    bool isRemote;
};

}

#endif  /* COMPONENT_H */
