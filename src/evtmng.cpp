/******************************************************************************
 * File:    evtmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.evtmng
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
 *   Implement EventManager class
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

#include "evtmng.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>
#include <unistd.h>
#include <array>

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
EventManager::EventManager(const char * name) :
    Component(name), waitingForGO(true)
{
    canProcessMessage(MSG_INDATA_IDX);
    canProcessMessage(MSG_TASK_RES_IDX); // TODO: Deprecate this for EvtMng in favour of DB

    setHeartBeatPeriod(0, 10000);
}

//----------------------------------------------------------------------
// Method: go
//----------------------------------------------------------------------
void EventManager::go()
{
    waitingForGO = false;
}

//----------------------------------------------------------------------
// Method: fromInitialisedToRunning
//----------------------------------------------------------------------
void EventManager::fromInitialisedToRunning()
{
    // Establish communications with other peer
    establishCommunications();
    while (waitingForGO) {}

    // Broadcast START message
    InfoMsg("Broadcasting START message . . .");
    PeerMessage * startMsg = buildPeerMsg("", "Wake up!", MSG_START);
    registerMsg(selfPeer()->name, *startMsg, true);
    broadcast(startMsg);
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void EventManager::fromOperationalToRunning()
{
    // Broadcast STOP message
    InfoMsg("Broadcasting STOP message . . .");
    PeerMessage * stopMsg = buildPeerMsg("", "Shut down!", MSG_STOP);
    registerMsg(selfPeer()->name, *stopMsg, true);
    broadcast(stopMsg);
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void EventManager::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: processINDATA
//----------------------------------------------------------------------
void EventManager::processINDATA()
{
    // Send InData message to DataMng
    std::array<std::string,1> fwdRecip = {"DataMng"};
    for (std::string & recip : fwdRecip) {
        // Forward to recipient
        MessageData msgDataToRecip(new Message_INDATA);
        msgDataToRecip.msg->setData(msgData.msg->getData());
        Json::StyledWriter w;
        setForwardTo(recip, msgDataToRecip.msg->header);
        PeerMessage * msgForRecip = buildPeerMsg(msgDataToRecip.msg->header.destination,
                                                 msgDataToRecip.msg->getDataString(),
                                                 MSG_INDATA);
        registerMsg(selfPeer()->name, *msgForRecip);
        setTransmissionToPeer(recip, msgForRecip);
    }
}

//----------------------------------------------------------------------
// Method: processTASK_RES
//----------------------------------------------------------------------
void EventManager::processTASK_RES()
{
    // TODO: Remove this connection, make TaskRes available to QPFHMI through DB
    // Send a TaskResMsg to the HMI
    Message_TASK_RES * msg = dynamic_cast<Message_TASK_RES *>(msgData.msg);
    MessageData msgToHMI(new Message_TASK_RES);
    msgToHMI.msg->setData(msg->getData());
    setForwardTo("QPFHMI", msgToHMI.msg->header);
    PeerMessage * msgForHMI = buildPeerMsg(msgToHMI.msg->header.destination,
                                           msgToHMI.msg->getDataString(),
                                           MSG_TASK_RES);
    registerMsg(selfPeer()->name, *msgForHMI);
    setTransmissionToPeer("QPFHMI", msgForHMI);

    if (msg->task.taskData["State"]["Progress"].asString() == "100") {
        InfoMsg("RECEIVED NOTIFICATION OF TASK " + msg->task.taskName +
                " FINISHED AT " + msg->task.taskEnd);
    }
}

}
