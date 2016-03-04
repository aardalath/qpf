/******************************************************************************
 * File:    component.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.Component
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
//   - commnode.h
//------------------------------------------------------------

#include "commnode.h"
using LibComm::CommNode;

//------------------------------------------------------------
// Topic: Project headers
//   - msgtypes.h
//------------------------------------------------------------

#include "msgtypes.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

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
    virtual void processMONIT_RQST() {}

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
    // Method: procInData
    // Process a new MSG_INDATA message
    //----------------------------------------------------------------------
    bool procInData(Router2RouterPeer::PeerMessage & inPeerMsg);

    //----------------------------------------------------------------------
    // Method: procTaskProc
    // Process a new MSG_TASK_PROC message
    //----------------------------------------------------------------------
    bool procTaskProc(Router2RouterPeer::PeerMessage & inPeerMsg);


    //----------------------------------------------------------------------
    // Method: procTaskRes
    // Process a new MSG_TASK_RES message
    //----------------------------------------------------------------------
    bool procTaskRes(Router2RouterPeer::PeerMessage & inPeerMsg);

    //----------------------------------------------------------------------
    // Method: writeToFile
    // Write content of physical message to file
    //----------------------------------------------------------------------
    void writeToFile(Router2RouterPeer::PeerMessage& inPeerMsg);

protected:
    std::set<int> canProcess;
    MessageData   msgData;
};

}

#endif  /* COMPONENT_H */
