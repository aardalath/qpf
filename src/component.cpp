/******************************************************************************
 * File:    component.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.component
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
 *   Implement Component class
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

#include "component.h"

#include "dbhdlpostgre.h"
#include "except.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#define WRITE_MESSAGE_FILES

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

#define T(a) MSG_ ## a = std::string( #a )
const std::string TLIST_MESSAGE_TYPES;
#undef T

#define T(a) std::string( #a )
const std::string MessageTypeId[] = { TLIST_MESSAGE_TYPES };
#undef T

#define T(a,b) { TASK_ ## a , #a }
std::map<TaskStatus, std::string> TaskStatusName = { TLIST_TASK_STATUS };
#undef T

#define T(a,b) { #a , TASK_ ## a }
std::map<std::string, TaskStatus> TaskStatusValue = { TLIST_TASK_STATUS };
#undef T
const int BadMsgProcessing = -1;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Component::Component(const char * name) :
    CommNode(name)
{
    setHeartBeatPeriod();
}

//----------------------------------------------------------------------
// Method: fromInitialisedToRunning
//----------------------------------------------------------------------
void Component::fromInitialisedToRunning()
{
    // Establish communications with other peer
    waitForStartSignal();
    establishCommunications();

    if (startSignalReceived()) {
        InfoMsg("COMMUNICATION ESTABLISHED!!");
    } else {
        ErrMsg("COMMUNICATION REJECTED!!");
    }
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void Component::fromRunningToOperational()
{
    // nothing needed
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void Component::fromOperationalToRunning()
{
    // nothing needed
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void Component::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: init
// Initialize the component
//----------------------------------------------------------------------
void Component::init()
{
    // Define log output
    Log::defineLogSystem(getCommNodeName(),
                         Log::getLogBaseDir() + "/log/" +
                         getCommNodeName() + ".log");

    // Define valid state transitions
    defineValidTransitions();
    transitTo(INITIALISED);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: run
// (Virtual) method executed when the thread is created
//----------------------------------------------------------------------
int Component::run()
{
    Peer inPeer;
    PeerMessage inPeerMsg;

    fromInitialisedToRunning();

    /* Transition to Running */
    transitTo(RUNNING);
    InfoMsg("New state: " + getStateName(getState()));

    fromRunningToOperational();

    /* Transition to Operational */
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));

    /* MAIN LOOP */
    while (getState() == OPERATIONAL) {

        // Process incoming messages
        if (thereArePendingTransmissions()) {

            // Get new message
            getNewTransmission(inPeer, inPeerMsg);

            // First check: is STOP message?
            if (is(inPeerMsg, MSG_STOP)) { break; }

            int msgIdx = process(inPeer, inPeerMsg);
            if (msgIdx > 0) {

                InfoMsg("Incoming messsage: " + MessageTypeId[msgIdx]);

                if (canProcess.find(msgIdx) == canProcess.end()) { msgIdx = -1; }

                switch (msgIdx) {
                case MSG_START_IDX:
                    processSTART();
                    break;
                case MSG_INDATA_IDX:
                    processINDATA();
                    break;
                case MSG_DATA_RQST_IDX:
                    processDATA_RQST();
                    break;
                case MSG_DATA_INFO_IDX:
                    processDATA_INFO();
                    break;
                case MSG_MONIT_RQST_IDX:
                    processMONIT_RQST();
                    break;
                case MSG_MONIT_INFO_IDX:
                    processMONIT_INFO();
                    break;
                case MSG_TASK_PROC_IDX:
                    processTASK_PROC();
                    break;
                case MSG_TASK_RES_IDX:
                    processTASK_RES();
                    break;
                case MSG_STOP_IDX:
                    processSTOP();
                    break;
                case MSG_UNKNOWN_IDX:
                    processUNKNOWN();
                    break;
                default:
                    WarnMsg("Component " + selfPeer()->name +
                            " received an unexpected message: " +
                            MessageTypeId[msgIdx]);
                    break;
                }
            }

        } else {

            // Nothing for the time being
            LibComm::waitForHeartBeat(hbSecs, hbMicroSecs);
            //InfoMsg("Heart Beat!");

        }

        // Send log info to LogMng in case there is something to send
        if (!fileToSend.empty()) {
            sendLogPacketAsDataInfoMsg();
        }

        // Additional loop tasks (entry for additional functionality
        //to be exectued every loop step)
        execAdditonalLoopTasks();

    }

    fromOperationalToRunning();

    // Set state to Running
    transitTo(RUNNING);
    InfoMsg("New state: " + getStateName(getState()));

    fromRunningToOff();

    // Set state to Offdbhandler
    transitTo(OFF);
    InfoMsg("New state: " + getStateName(getState()));
    flushLog();

    return 0;
}

//----------------------------------------------------------------------
// Method: canProcessMessage
// Activates the component as able to process the specified message
//----------------------------------------------------------------------
void Component::canProcessMessage(MessageId id)
{
   canProcess.insert(id);
}

//----------------------------------------------------------------------
// Method: is
// Check type of msg
//----------------------------------------------------------------------
bool Component::is(Router2RouterPeer::PeerMessage & inPeerMsg,
                        MessageTypeIdx msgType)
{
    return (inPeerMsg.at(Router2RouterPeer::FRAME_MSG_TYPE) ==
            MessageTypeId[(int)(msgType)]);
}

//----------------------------------------------------------------------
// Method: is
// Check type of msg
//----------------------------------------------------------------------
bool Component::is(Router2RouterPeer::PeerMessage & inPeerMsg,
                        std::string msgType)
{
    return (inPeerMsg.at(Router2RouterPeer::FRAME_MSG_TYPE) == msgType);
}

//----------------------------------------------------------------------
// Method: process
// Process a new message
//----------------------------------------------------------------------
int Component::process(Router2RouterPeer::Peer & inPeer,
                       Router2RouterPeer::PeerMessage & inPeerMsg)
{
    UNUSED(inPeer);

    static const int MsgTypeFrm = (const int)(Router2RouterPeer::FRAME_MSG_TYPE);

    // Look for message type
    int idx;
    for (idx = 0; idx < (int)(MSG_UNKNOWN_IDX); ++idx) {
        if (inPeerMsg.at(MsgTypeFrm).compare(MessageTypeId[idx]) == 0) {
            break;
        }
    }

    // If type not found, return false since message was not processed
    if (idx == (int)(MSG_UNKNOWN_IDX)) { return idx; }

#ifdef WRITE_MESSAGE_FILES
    writeToFile(inPeerMsg);
#endif

    // Otherwise, process it accordingly
    switch (idx) {
    case MSG_START_IDX:
        break;

    case MSG_STOP_IDX:
        break;

    case MSG_INDATA_IDX:
        if (!procInData(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_DATA_RQST_IDX:
        if (!procDataRqst(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_DATA_INFO_IDX:
        if (!procDataInfo(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_MONIT_RQST_IDX:
        if (!procMonitRqst(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_MONIT_INFO_IDX:
        if (!procMonitInfo(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_TASK_PROC_IDX:
        if (!procTaskProc(inPeerMsg)) { return BadMsgProcessing; }
        break;

    case MSG_TASK_RES_IDX:
        if (!procTaskRes(inPeerMsg)) { return BadMsgProcessing; }
        break;

    default:
        break;
    }

    return idx;
}

//----------------------------------------------------------------------
// Method: buildMsgHeader
// Build a message header
//----------------------------------------------------------------------
bool Component::buildMsgHeader(MessageId msgId,
                               NodeName source,
                               NodeName destination,
                               MessageHeader & hdr)
{
    hdr.msgId = msgId;
    hdr.msgVersion = 1;
    hdr.source = source;
    hdr.destination = destination;
    hdr.path.items.clear();
    hdr.path.items.push_back(source);
    hdr.path.items.push_back(destination);
    hdr.dateCreation = LibComm::timeTag();
    hdr.crc = 0;
    return true;
}

//----------------------------------------------------------------------
// Method: buildMsgINDATA
// Builds the INDATA message
//----------------------------------------------------------------------
bool Component::buildMsgINDATA(MessageHeader & hdr,
                               ProductCollection prdMetadata,
                               Message_INDATA & msg)
{
    msg.header = hdr;
    msg.productsMetadata = prdMetadata;
    return true;
}

//----------------------------------------------------------------------
// Method: buildMsgTASKPROC
// Builds the TASK_PROC message
//----------------------------------------------------------------------
bool Component::buildMsgTASKPROC(MessageHeader & hdr,
                                 TaskInfo & task,
                                 std::string rule,
                                 Message_TASK_PROC & msg)
{
    msg.header = hdr;
    msg.task = task;
    msg.rule = rule;
    return true;
}

//----------------------------------------------------------------------
// Method: assignMsgDataString
// Sets msg content in msgData to incoming msg content frame
//----------------------------------------------------------------------
void Component::assignMsgDataString(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.msg->setDataString(inPeerMsg.at(Router2RouterPeer::FRAME_MSG_CONTENT));
}

//----------------------------------------------------------------------
// Method: procInData
// Process a new MSG_INDATA message
//----------------------------------------------------------------------
bool Component::procInData(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_INDATA);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procDataRqst
// Process a new MSG_DATA_RQST message
//----------------------------------------------------------------------
bool Component::procDataRqst(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_DATA_RQST);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procDataInfo
// Infoess a new MSG_DATA_INFO message
//----------------------------------------------------------------------
bool Component::procDataInfo(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_DATA_INFO);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procMonitRqst
// Process a new MSG_MONIT_RQST message
//----------------------------------------------------------------------
bool Component::procMonitRqst(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_MONIT_RQST);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procMonitInfo
// Infoess a new MSG_MONIT_INFO message
//----------------------------------------------------------------------
bool Component::procMonitInfo(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_MONIT_INFO);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procTaskProc
// Process a new MSG_TASK_PROC message
//----------------------------------------------------------------------
bool Component::procTaskProc(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_TASK_PROC);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: procTaskRes
// Process a new MSG_TASK_RES message
//----------------------------------------------------------------------
bool Component::procTaskRes(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new Message_TASK_RES);
    assignMsgDataString(inPeerMsg);
    return true;
}

//----------------------------------------------------------------------
// Method: setForwardTo
// Modify msg header to forward msg to another destination
//----------------------------------------------------------------------
bool Component::setForwardTo(NodeName fwdDestination,
                             MessageHeader & header)
{
    header.source = header.destination;
    header.destination = fwdDestination;
    header.path.items.push_back(fwdDestination);
    header.crc = 0;
    return true;
}

//----------------------------------------------------------------------
// Method: convertTo
// Modify msg header to forward msg to another destination
//----------------------------------------------------------------------
bool Component::convertTo(MessageTypeIdx newIdx,
                          NodeName fwdDestination,
                          Message & msg)
{
    msg.header.msgId = newIdx;
    msg.header.msgVersion = 1;
    msg.header.source = msg.header.destination;
    msg.header.destination = fwdDestination;
    msg.header.path.items.push_back(fwdDestination);
    msg.header.crc = 0;
    return true;
}

//----------------------------------------------------------------------
// Method: writeToFile
// Write content of physical message to file
//----------------------------------------------------------------------
void Component::writeToFile(Router2RouterPeer::PeerMessage& inPeerMsg)
{
    Json::Value root;
    Json::Value content;
    Json::Reader reader;
    Json::StyledWriter writer;
    root["sender"] = inPeerMsg.at(Router2RouterPeer::FRAME_PEER_ID);
    root["msgtype"] = inPeerMsg.at(Router2RouterPeer::FRAME_MSG_TYPE);
    if (reader.parse(inPeerMsg.at(Router2RouterPeer::FRAME_MSG_CONTENT), content)) {
        root["content"] = content;
    } else {
        root["content"] = "<<<" + inPeerMsg.at(Router2RouterPeer::FRAME_MSG_CONTENT) + ">>>";
    }

    // Time timestamp
    std::string msgFileName(Log::getLogBaseDir() + "/msg/");
    msgFileName += (inPeerMsg.at(Router2RouterPeer::FRAME_PEER_ID) + "_" +
                    inPeerMsg.at(Router2RouterPeer::FRAME_MSG_TYPE) + "_" +
                    LibComm::preciseTimeTag() + ".msg");
    std::ofstream msgOut(msgFileName);
    msgOut << writer.write(root);
    msgOut.close();
}

//----------------------------------------------------------------------
// Method: registerMsg
// Store content of message into database
//----------------------------------------------------------------------
void Component::registerMsg(std::string from,
                                 Router2RouterPeer::PeerMessage & inPeerMsg,
                                 bool isBroadcast)
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    DBHandler * db = dbHdl.get();

    // Check that connection with the DB is possible
    try {
        db->openConnection();
    } catch (RuntimeException & e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    // Store message at DB
    db->storeMsg(from, inPeerMsg, isBroadcast);

    // Close connection
    db->closeConnection();
}

//----------------------------------------------------------------------
// Method: sendLogPacketAsDataInfoMsg
//----------------------------------------------------------------------
void Component::sendLogPacketAsDataInfoMsg()
{
    if (selfPeer()->name == "LogMng") { return; }

    Message_DATA_INFO msg;
    buildMsgHeader(MSG_DATA_INFO_IDX, selfPeer()->name, "LogMng", msg.header);
    msg.variables.paramList["contents"] = logChunk;
    PeerMessage * dataInfoMsg = buildPeerMsg("LogMng", msg.getDataString(), MSG_DATA_INFO);
    // DataInfo messages are not registered
    // registerMsg(selfPeer()->name, *dataInfoMsg);
    setTransmissionToPeer("LogMng", dataInfoMsg);
    logChunk = "";
}

//----------------------------------------------------------------------
// MethoComponent::d: setHeartBeatPeriod
// Sets number of seconds and microseconds for HeartBeat period
//----------------------------------------------------------------------
void Component::setHeartBeatPeriod(int s, int us)
{
    hbSecs = s;
    hbMicroSecs = us;
}


}
