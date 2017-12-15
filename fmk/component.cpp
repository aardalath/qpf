/******************************************************************************
 * File:    component.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.component
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
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
 *   See <ChangelEUCog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "component.h"
#include "message.h"

#include "dbhdlpostgre.h"
#include "except.h"

//#include "tools.h"
#include "str.h"

#include "config.h"

using Configuration::cfg;

//#include <sys/time.h>
//#include <unistd.h>
//#include <time.h>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

const int HEART_BEAT_STEP_SIZE = 250;

#include <csignal>

void signalHandler( int signum ) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    exit(signum);  
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Component::Component(const char * name, const char * addr, Synchronizer * s)
{
    init(std::string(name), std::string(addr), s);
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
Component::Component(std::string name, std::string addr, Synchronizer * s)
{
    init(name, addr, s);
}

//----------------------------------------------------------------------
// Method: init
// Initialize the component
//----------------------------------------------------------------------
void Component::init(std::string name, std::string addr, Synchronizer * s)
{
    compName    = name;
    compAddress = addr;
    synchro     = s;

    // register signal SIGINT and signal handler  
    signal(SIGABRT, signalHandler);  

    iteration = 0;
    stepSize  = HEART_BEAT_STEP_SIZE;

    // Every component must respond to MONIT_RQST messages (at least the
    // state might be requested)
    //canProcessMessage(MSG_MONIT_RQST_IDX);

    /*
    isPeerLogMng = isPeer("LogMng") && (selfPeer()->name != "LogMng");
    isRemote     = (!ConfigurationInfo::data().isMaster);
    session      = ConfigurationInfo::data().session;
    */
    // Define log output
    //Log::setLogBaseDir(Config::PATHBase + "/log");
    Log::defineLogSystem(compName);

    // Define valid state transitions
    defineValidTransitions();

    // Transit to INITIALISED
    transitTo(INITIALISED);
    InfoMsg("New state: " + getStateName(getState()));

    if (! compAddress.empty()) {
        DbgMsg("Creating thread for " + compName + " in " + compAddress);
        thrId = std::thread(&Component::run, this);
        thrId.detach();
    }
}

//----------------------------------------------------------------------
// Method: addConnection
//----------------------------------------------------------------------
void Component::addConnection(ChannelDescriptor & chnl,
                              ScalabilityProtocolRole * conct)
{
    //TRC(">>> Connection object is 0x" << conct);
    //TRC(">>> Creating connection: " + chnl + " - " + conct->getName() + "/" + conct->getAddress());
    conct->setName(compName);
    connections[chnl] = conct;
}

//----------------------------------------------------------------------
// Method: periodicMsgInChannel
//----------------------------------------------------------------------
void Component::periodicMsgInChannel(ChannelDescriptor chnl, int period, MessageString msg)
{
    periodicMsgs[chnl][period] = msg;
}

//----------------------------------------------------------------------
// Method: fromInitialisedToRunning
//----------------------------------------------------------------------
void Component::fromInitialisedToRunning()
{
    transitTo(RUNNING);
    InfoMsg("New state: " + getStateName(getState()));
    for (auto & kv: connections) {
        const ChannelDescriptor & chnl = kv.first;
        ScalabilityProtocolRole * conn = kv.second;
        std::string ch(kv.first);
        InfoMsg("Connection " + ch + " - " + conn->getName() + " @ " + conn->getAddress());
    }
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void Component::fromRunningToOperational()
{
    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void Component::fromOperationalToRunning()
{
    InfoMsg("New state: " + getStateName(getState()));
    transitTo(OFF);
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void Component::fromRunningToOff()
{
    InfoMsg("New state: " + getStateName(getState()));
    InfoMsg("Ending . . . ");
    synchro->notify();
}

//----------------------------------------------------------------------
// Method: updateConnections
//----------------------------------------------------------------------
void Component::updateConnections()
{
    for (auto & kv: connections) { kv.second->update(); }
}

//----------------------------------------------------------------------
// Method: processIncommingMessages
//----------------------------------------------------------------------
void Component::processIncommingMessages()
{
    MessageString m;
    Message_Tag incommMsgTag;
    
    for (auto & kv: connections) {
        const ChannelDescriptor & chnl = kv.first;
        ScalabilityProtocolRole * conn = kv.second;
        while (conn->next(m)) {
            MessageBase msg(m);
            std::string tgt(msg.header.target());
            if ((tgt != "*") && (tgt != compName)) { continue; }
            std::string type(msg.header.type());
            DbgMsg("(FROM component.cpp:) "  + compName + " received the message [" + m + "]");

            if      (chnl == ChnlCmd)      { incommMsgTag = Tag_ChnlCmd; }
            else if (chnl == ChnlEvtMng)   { incommMsgTag = Tag_ChnlEvtMng; }
            else if (chnl == ChnlHMICmd)   { incommMsgTag = Tag_ChnlHMICmd; }
            else if (chnl == ChnlInData)   { incommMsgTag = Tag_ChnlInData; }
            else if (chnl == ChnlTskSched) { incommMsgTag = Tag_ChnlTskSched; }
            else if (chnl == ChnlTskReg)   { incommMsgTag = Tag_ChnlTskReg; }
            else if (chnl == ChnlFmkMon)   { incommMsgTag = Tag_ChnlFmkMon; }
            else if (type == MsgTskRqst)   { incommMsgTag = Tag_MsgTskRqst; }
            else if (type == MsgTskProc)   { incommMsgTag = Tag_MsgTskProc; }
            else if (type == MsgTskRep)    { incommMsgTag = Tag_MsgTskRep; }
            else if (type == MsgHostMon)   { incommMsgTag = Tag_MsgHostMon; }
            else                           { incommMsgTag = Tag_UNKNOWN; }

            switch (incommMsgTag) {
            case Tag_ChnlCmd:      processCmdMsg(conn, m);      break;
            case Tag_ChnlEvtMng:   processEvtMngMsg(conn, m);   break;
            case Tag_ChnlHMICmd:   processHMICmdMsg(conn, m);   break;
            case Tag_ChnlInData:   processInDataMsg(conn, m);   break;
            case Tag_ChnlTskSched: processTskSchedMsg(conn, m); break;
            case Tag_ChnlTskReg:   processTskRegMsg(conn, m);   break;
            case Tag_ChnlFmkMon:   processFmkMonMsg(conn, m);   break;
            case Tag_MsgTskRqst:   processTskRqstMsg(conn, m);  break;
            case Tag_MsgTskProc:   processTskProcMsg(conn, m);  break;
            case Tag_MsgTskRep:    processTskRepMsg(conn, m);   break;
            case Tag_MsgHostMon:   processHostMonMsg(conn, m);  break;
            default:
                WarnMsg("Message from unidentified channel " + chnl);
                RaiseSysAlert(Alert(Alert::System,
                                    Alert::Warning,
                                    Alert::Comms,
                                    std::string(__FILE__ ":" Stringify(__LINE__)),
                                    "Message from unidentified channel " + chnl,
                                    0));
            }

            if (cfg.writeMsgsToDisk &&
                ((static_cast<int>(incommMsgTag) & cfg.writeMsgsMask) != 0)) {
                writeMsgToFile(Recv, chnl, m);
            }
        }
    }
}

//----------------------------------------------------------------------
// Method: sendPeriodicMsgs
//----------------------------------------------------------------------
void Component::sendPeriodicMsgs()
{
    for (auto & kv: periodicMsgs) {
        ChannelDescriptor chnl = kv.first;
        for (auto & kkv: kv.second) {
            int period = kkv.first;
            if (((iteration + 1) % period) == 0) {
                MessageString & msg = kkv.second;
                this->send(chnl, msg);
            }
        }
    }
}

//----------------------------------------------------------------------
// Method: runEachIteration
//----------------------------------------------------------------------
void Component::runEachIteration()
{
}

//----------------------------------------------------------------------
// Method: step
//----------------------------------------------------------------------
void Component::step()
{
    // Sleep until next second
    /*
    std::time_t tt = system_clock::to_time_t(system_clock::now());
    struct std::tm * ptm = std::localtime(&tt);
    ptm->tm_sec++;
    std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
    */
    // Sleep for 'stepSize' milliseconds
    //auto start = std::chrono::high_resolution_clock::now();
    long int msnow =
        static_cast<long int>(
           std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    long int ms2wait = stepSize - ((msnow + stepSize) % stepSize);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms2wait));
}

//----------------------------------------------------------------------
// Method: send
//----------------------------------------------------------------------
void Component::send(ChannelDescriptor chnl, MessageString m)
{
    std::map<ChannelDescriptor, ScalabilityProtocolRole*>::iterator
        it = connections.find(chnl);
    if (it != connections.end()) {
        ScalabilityProtocolRole * conn = it->second;
        conn->setMsgOut(m);
    } else {
        WarnMsg("Couldn't send message via channel " + chnl);
        RaiseSysAlert(Alert(Alert::System,
                            Alert::Warning,
                            Alert::Comms,
                            std::string(__FILE__ ":" Stringify(__LINE__)),
                            "Couldn't send message via channel: " + chnl,
                            0));

    }
}

//----------------------------------------------------------------------
// Method: sendBodyElem<T>
//----------------------------------------------------------------------
template<class T>
void Component::sendBodyElem(ChannelDescriptor chnl,
                             ChannelDescriptor actualChnl, MessageDescriptor tag,
                             std::string from, std::string to,
                             std::string bodyElem, std::string elemContent,
                             std::string initialMsgStr = std::string())
{
    // Prepare message and send it
    Message<T> msg(initialMsgStr);
    JValue jstrValue(elemContent);

    T & body = msg.body;
    body[bodyElem] = jstrValue.val();

    msg.buildHdr(chnl, tag, CHNLS_IF_VERSION, from, to, "", "", "");
    msg.buildBody(body);

    this->send(actualChnl, msg.str());
}

// explicit instantiation
template
void Component::sendBodyElem<MsgBodyTSK>(ChannelDescriptor chnl,
                                         ChannelDescriptor actualChnl, MessageDescriptor tag,
                                         std::string from, std::string to,
                                         std::string bodyElem, std::string elemContent,
                                         std::string initialMsgStr);

//----------------------------------------------------------------------
// Method: run
//----------------------------------------------------------------------
void Component::run()
{
    // Wait for synchronisation signal
    synchro->wait();

    // Show connections
    for (auto & kv: connections) {
        ChannelDescriptor chnl = kv.first;
        ScalabilityProtocolRole * role = kv.second;
        TraceMsg(role->getName() + " in Channel " + chnl + " with address " +
                 role->getAddress() + " - " + role->getClassName());
    }

    // State: Initialised
    // Transition to: Running
    fromInitialisedToRunning();

    // State: Initialised
    // Transition to: Running
    fromRunningToOperational();

    do {
        ++iteration;
        updateConnections();
        processIncommingMessages();
        sendPeriodicMsgs();
        runEachIteration();
        step();
    } while (getState() == OPERATIONAL);

    // State: Initialised
    // Transition to: Running
    fromOperationalToRunning();

    // State: Initialised
    // Transition to: Running
    fromRunningToOff();
}

//----------------------------------------------------------------------
// Method: setStep
//----------------------------------------------------------------------
void Component::setStep(int s)
{
    std::unique_lock<std::mutex> ulck(mtxStepSize);
    stepSize = s;
}

//----------------------------------------------------------------------
// Method: processCmdMsg
//----------------------------------------------------------------------
void Component::processCmdMsg(ScalabilityProtocolRole * c, MessageString & m)
{
    Message<MsgBodyCMD> msg(m);
    std::string cmd = msg.body["cmd"].asString();

    if (cmd == CmdQuit) {

        transitTo(RUNNING);

    } else if (cmd == CmdInit) {

        std::string sessId = msg.body["sessionId"].asString();
        if (sessId != cfg.sessionId) {
            cfg.synchronizeSessionId(sessId);
        }

    } else if (cmd == CmdConfig) { // This should be any component

        std::string newConfigString = msg.body["config"].asString();
        cfg.fromStr(newConfigString);
        cfg.processConfig();        
        TraceMsg("New configuration applied");

    } else if (cmd == CmdProcHdl) {

        processSubcmdMsg(m);

    } else {

        WarnMsg("Unknown command " + cmd + " at channel " + ChnlCmd);
        RaiseSysAlert(Alert(Alert::System,
                            Alert::Warning,
                            Alert::Comms,
                            std::string(__FILE__ ":" Stringify(__LINE__)),
                            "Unknown command " + cmd + " at channel " + ChnlCmd,
                            0));

    }
}

//----------------------------------------------------------------------
// Method: processEvtMngMsg
//----------------------------------------------------------------------
void Component::processEvtMngMsg(ScalabilityProtocolRole * c, MessageString & m)
{
    Message<MsgBodyCMD> msg(m);
    std::string cmd = msg.body["cmd"].asString();

    if (cmd == CmdPing) { // This is any component but EvtMng

        TraceMsg(compName + " received a " + CmdPing + " and sends " + getStateName(getState()));
        MsgBodyCMD body;
        msg.buildHdr(ChnlEvtMng, MsgEvtMng, CHNLS_IF_VERSION,
                     compName, msg.header.source(),
                     "", "", "");
        body["cmd"]     = CmdStates;
        body["state"]   = getStateName(getState());
        body["host"]    = compAddress;
        body["logs"]    = Config::PATHLog;
            
        msg.buildBody(body);
        this->send(ChnlEvtMng, msg.str());

    } else if (cmd == CmdStates) { // This should be EvtMng

        cfg.nodeStates[msg.header.source()] = msg.body["state"].asString();
        logFolders[msg.body["host"].asString()] = msg.body["logs"].asString();
       
        TraceMsg(compName + " received from " + msg.header.source() + " from " + compName);

    } else {

        WarnMsg("Unknown command " + cmd + " at channel " + ChnlCmd);
        RaiseSysAlert(Alert(Alert::System,
                            Alert::Warning,
                            Alert::Comms,
                            std::string(__FILE__ ":" Stringify(__LINE__)),
                            "Unknown command " + cmd + " at channel " + ChnlCmd,
                            0));

    }
}

//----------------------------------------------------------------------
// Method: sendAns
// Send basic answer as a REP node
//----------------------------------------------------------------------
void Component::sendAns(ChannelDescriptor chnl, MessageDescriptor msgd, 
                        std::string to, std::string ans)
{
    // Send reply
    Message<MsgBodyTSK> msgAns;
    msgAns.buildHdr(chnl, msgd, CHNLS_IF_VERSION,
                   compName, to,
                   "", "", "");
    MsgBodyTSK bodyAns;
    bodyAns["ans"] = ans;
    msgAns.buildBody(bodyAns);
    this->send(chnl, msgAns.str());
}

//----------------------------------------------------------------------
// Method: defineValidTransitions
// Define the valid state transitions for the node
//----------------------------------------------------------------------
void Component::defineValidTransitions()
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

//----------------------------------------------------------------------
// Method: afterTransition
//----------------------------------------------------------------------
void Component::afterTransition(int fromState, int toState)
{
/*
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
*/
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
        ErrMsg(e.what());
        DBG(e.what());
        return;
    }

    // Close connection
    db->closeConnection();

    //  Store alert msg in log file
    Log::LogLevel lvl = Log::WARNING;
    if (a.getGroup() == Alert::Diagnostics) {
        if (a.getSeverity() > Alert::Warning) {
            lvl = Log::ERROR;
        }
    } else {
        if (a.getSeverity() == Alert::Error) {
            lvl = Log::ERROR;
        } else if (a.getSeverity() > Alert::Fatal) {
            lvl = Log::FATAL;
        }
    }
    Log::log(compName, lvl, alertMsg);
}

//----------------------------------------------------------------------
// Method: step
//----------------------------------------------------------------------
void Component::writeMsgToFile(SendOrRecv sor,
                               ChannelDescriptor chnl, MessageString m)
{
    struct timespec timesp;
    if (clock_gettime(CLOCK_REALTIME_COARSE, &timesp) != 0) {
        perror("clock_gettime");
        exit(1);
    }

    char fileName[256];
    sprintf(fileName, "%s/%lld.%09ld_%s_%c_%s.mson",
            Config::PATHSession.c_str(),
            (long long)timesp.tv_sec, timesp.tv_nsec,
            compName.c_str(), 
            (sor == Send ? 'S' : 'R'), chnl.c_str());
    FILE * fHdl = fopen(fileName, "w");
    fprintf(fHdl, m.c_str());
    fclose(fHdl);
}

//}
