/******************************************************************************
 * File:    component.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.component
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
#include "config.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"
#include "str.h"
using namespace LibComm;

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

//#define WRITE_MESSAGE_FILES

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

static const int MsgTypeFrm = (const int)(Router2RouterPeer::FRAME_MSG_TYPE);

#define T(a,b)  a ## _Str = std::string( #b )
const std::string TLISTOF_MONIT_RQST_COMMANDS;
#undef T

#define T(a,b)  std::string( #b )
const std::string MonitRqstCommands[] = { TLISTOF_MONIT_RQST_COMMANDS };
#undef T

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Component::Component(const char * name) :
    CommNode(name)
{
    setHeartBeatPeriod();

    // Every component must respond to MONIT_RQST messages (at least the
    // state might be requested)
    canProcessMessage(MSG_MONIT_RQST_IDX);
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
    isPeerLogMng = isPeer("LogMng") && (selfPeer()->name != "LogMng");
    isRemote     = (!ConfigurationInfo::data().isMaster);
    session      = ConfigurationInfo::data().session;

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

    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    fromInitialisedToRunning();

    /* Transition to Running */
    transitTo(RUNNING);
    InfoMsg("New state: " + getStateName(getState()));

    fromRunningToOperational();

    /* Transition to Operational */
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));

    DbgMsg(getCommNodeName() + (isRemote ? " : REMOTE" : " : LOCAL"));

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

                if (cfgInfo.flags.monit.notifyMsgArrival) {
                    DbgMsg("Incoming messsage: " + MessageTypeId[msgIdx]);
                }

                if (canProcess.find(msgIdx) == canProcess.end()) {
                    WarnMsg("Component " + selfPeer()->name +
                            " received an unexpected message: " +
                            MessageTypeId[msgIdx]);
                    msgIdx = -1;
                }

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
                case MSG_CMD_IDX:
                    processCMD();
                    break;
                case MSG_STOP_IDX:
                    processSTOP();
                    break;
                case MSG_UNKNOWN_IDX:
                    processUNKNOWN();
                    break;
                default:
                    break;
                }
            }

        } else {

            // Nothing for the time being
            LibComm::waitForHeartBeat(hbSecs, hbMicroSecs);

        }

        // Send log info to LogMng in case there is something to send
        if ((!logChunk.empty()) && isPeerLogMng && isRemote) { sendLogPacketAsDataInfoMsg(); }

        // Additional loop tasks (entry for additional functionality
        //to be exectued every loop step)
        execAdditonalLoopTasks();

    }

    fromOperationalToRunning();

    // Show Running state (only possible state from OPERATIONAL)
    InfoMsg("New state: " + getStateName(getState()));

    fromRunningToOff();

    // Set state to Offdbhandler
    transitTo(OFF);
    InfoMsg("New state: " + getStateName(getState()));
    flushLog();

    return THR_DONE;
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

    // Look for message type
    int idx;
    for (idx = 0; idx < (int)(MSG_UNKNOWN_IDX); ++idx) {
        if (inPeerMsg.at(MsgTypeFrm).compare(MessageTypeId[idx]) == 0) {
            break;
        }
    }

    // If type not found, return false since message was not processed
    if (idx == (int)(MSG_UNKNOWN_IDX)) { return idx; }

    // Otherwise, process it accordingly

    bool result = true;

    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();
    std::string msgTypeName = MessageTypeId[(int)(idx)];
    bool writeThisMsgToDisk = cfgInfo.flags.monit.msgsToDisk[msgTypeName];
    if (writeThisMsgToDisk) { writeToFile(inPeerMsg); }

    switch (idx) {
    case MSG_START_IDX:
        break;

    case MSG_STOP_IDX:
        break;

    case MSG_INDATA_IDX:
        result = procMsg<Message_INDATA>(inPeerMsg);
        break;

    case MSG_DATA_RQST_IDX:
        result = procMsg<Message_DATA_RQST>(inPeerMsg);
        break;

    case MSG_DATA_INFO_IDX:
        result = procMsg<Message_DATA_INFO>(inPeerMsg);
        break;

    case MSG_MONIT_RQST_IDX:
        result = procMsg<Message_MONIT_RQST>(inPeerMsg);
        break;

    case MSG_MONIT_INFO_IDX:
        result = procMsg<Message_MONIT_INFO>(inPeerMsg);
        break;

    case MSG_TASK_PROC_IDX:
        result = procMsg<Message_TASK_PROC>(inPeerMsg);
        break;

    case MSG_TASK_RES_IDX:
        result = procMsg<Message_TASK_RES>(inPeerMsg);
        break;

    case MSG_CMD_IDX:
        result = procMsg<Message_CMD>(inPeerMsg);
        break;

    default:
        break;
    }

    if (!result) { return BadMsgProcessing; }

    return idx;
}

//----------------------------------------------------------------------
// Method: processMONIT_RQST
// Processes monit request messages
//----------------------------------------------------------------------
void Component::processMONIT_RQST()
{
    static std::map<std::string, Log::LogLevel> logLevel
        { {"TRACE",    Log::TRACE},
          {"DEBUG",    Log::DEBUG},
          {"INFO",     Log::INFO},
          {"WARNING",  Log::WARNING},
          {"ERROR",    Log::ERROR},
          {"FATAL",    Log::FATAL} };

    // Forward message to TskOrc
    Message_MONIT_RQST * msg = dynamic_cast<Message_MONIT_RQST *>(msgData.msg);
    PList & plist = msg->variables.paramList;
    for (auto & it : plist) {
        DbgMsg("Incoming message with pair <" + it.first + ", " + it.second + ">");
        if (it.first == MONIT_RQST_STATE_Str) {
            it.second = getStateName(getState());
            std::string & recip = msg->header.source;
            MessageData msgToRqstr(new Message_TASK_RES);
            msgToRqstr.msg->setData(msg->getData());
            buildMsgHeader(MSG_MONIT_INFO_IDX, selfPeer()->name, recip, msgToRqstr.msg->header);
            PeerMessage * msgForRqstr = buildPeerMsg(msgToRqstr.msg->header.destination,
                                                     msgToRqstr.msg->getDataString(),
                                                     MSG_MONIT_INFO);
            setTransmissionToPeer(recip, msgForRqstr);
        } else if (it.first == MONIT_RQST_MIN_LOG_LVL_Str) {
            std::string & lvlStr = it.second;
            InfoMsg("Setting log level to " + lvlStr);
            Log::LogLevel & lvl = logLevel[lvlStr];
            Log::setMinLogLevel(lvl);
        } else if (it.first == MONIT_RQST_NEW_CFG_Str) {
            InfoMsg("Applying new configuration . . .");
            std::string & cfgStr = it.second;
            Json::Reader r;
            Json::Value cfg;
            r.parse(cfgStr, cfg);
            Configuration newConfig(cfg);
        }
    }
}

//----------------------------------------------------------------------
// Method: sendMONIT_RQST
// Send a monitoring request to a target (or a set of them)
//----------------------------------------------------------------------
void Component::sendMONIT_RQST(std::string target, std::string what, std::string value)
{
    Message_MONIT_RQST msg;
    buildMsgHeader(MSG_MONIT_RQST_IDX, selfPeer()->name, target, msg.header);
    PList & pl = msg.variables.paramList;
    pl[what] = value;
    PeerMessage * rqstMsg = buildPeerMsg(target, msg.getDataString(), MSG_MONIT_RQST);
    if (target == "*") {
        //registerMsg(selfPeer()->name, *rqstMsg, true);
        broadcast(rqstMsg);
    } else {
        //registerMsg(selfPeer()->name, *rqstMsg);
        setTransmissionToPeer(target, rqstMsg);
    }
}

//----------------------------------------------------------------------
// Method: sendCMD
// Send a command message to a target (or a set of them)
//----------------------------------------------------------------------
void Component::sendCMD(std::string target, std::string what, std::string value)
{
    Message_CMD msg;
    buildMsgHeader(MSG_CMD_IDX, selfPeer()->name, target, msg.header);
    PList & pl = msg.variables.paramList;
    pl[what] = value;
    PeerMessage * rqstMsg = buildPeerMsg(target, msg.getDataString(), MSG_CMD);
    if (target == "*") {
        registerMsg(selfPeer()->name, *rqstMsg, true);
        broadcast(rqstMsg);
    } else {
        registerMsg(selfPeer()->name, *rqstMsg);
        setTransmissionToPeer(target, rqstMsg);
    }
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
// Method: procMsg
//----------------------------------------------------------------------
template<class T>
bool Component::procMsg(Router2RouterPeer::PeerMessage & inPeerMsg)
{
    msgData.assign(new T);
    msgData.msg->setDataString(inPeerMsg.at(Router2RouterPeer::FRAME_MSG_CONTENT));
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
    // Look for message type, and check if we are configured to store in DB
    int idx;
    for (idx = 0; idx < (int)(MSG_UNKNOWN_IDX); ++idx) {
        if (inPeerMsg.at(MsgTypeFrm).compare(MessageTypeId[idx]) == 0) {
            break;
        }
    }
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();
    std::string msgTypeName = MessageTypeId[(int)(idx)];
    bool writeThisMsgToDB = cfgInfo.flags.monit.msgsToDB[msgTypeName];
    if (!writeThisMsgToDB) { return; }

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
    Message_DATA_INFO msg;
    buildMsgHeader(MSG_DATA_INFO_IDX, selfPeer()->name, "LogMng", msg.header);
    msg.variables.paramList["contents"] = logChunk;
    PeerMessage * dataInfoMsg = buildPeerMsg("LogMng", msg.getDataString(), MSG_DATA_INFO);
    // DataInfo messages are not registered (TODO Change this as config)
    // registerMsg(selfPeer()->name, *dataInfoMsg);
    setTransmissionToPeer("LogMng", dataInfoMsg);
    logChunk = "";
}

//----------------------------------------------------------------------
// Method: Method: raise
// Raise alert, shows it in the log, and stored in DB
//----------------------------------------------------------------------
void Component::raise(Alert a, Alert::Group grp)
{
    if (grp != Alert::Undefined) { a.setGroup(grp); }

    std::string alertMsg = a.dump();

    // Store alert in DB
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    DBHandler * db = dbHdl.get();

    // Check that connection with the DB is possible
    try {
        db->openConnection();

        std::stringstream ss;
        ss << "INSERT INTO alerts "
           << "(alert_id, creation, grp, sev, typ, origin, msgs";
        if (a.getVar() != 0) { ss << ", var"; }
        ss << ") VALUES ( nextval('alerts_alert_id_seq'), "
           << str::quoted(a.timeStampString()) << ", "
           << str::quoted(Alert::GroupName[a.getGroup()]) << ", "
           << str::quoted(Alert::SeverityName[a.getSeverity()]) << ", "
           << str::quoted(Alert::TypeName[a.getType()]) << ", "
           << str::quoted(a.getOrigin()) << ", "
           << str::quoted(a.allMessages());
        if (a.getVar() != 0) { ss << ", " << a.varAsTuple(); }
        ss << ");";

        db->runCmd(ss.str());
    } catch (RuntimeException & e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    // Close connection
    db->closeConnection();

    //  Store alert msg in log file
    Log::LogLevel lvl = Log::WARNING;
    if (a.getGroup() == Alert::Diagnostics) {
        if (a.getSeverity() > Alert::Warning) { lvl = Log::ERROR; }
    } else {
        if (a.getSeverity() == Alert::Error) {
            lvl = Log::ERROR;
        } else if (a.getSeverity() > Alert::Fatal) {
            lvl = Log::FATAL;
        }
    }
    logMsg(alertMsg, lvl);
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

//----------------------------------------------------------------------
// Method: afterTransition
//----------------------------------------------------------------------
void Component::afterTransition(int fromState, int toState)
{
    // Save task information in task_info table
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        // Store new state
        dbHdl->storeState(session, selfPeer()->name, getStateName(toState));
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}


}
