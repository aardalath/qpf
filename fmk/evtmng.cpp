/******************************************************************************
 * File:    evtmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.evtmng
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
 *   Implement EvtMng class
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

#include "filenamespec.h"
#include "message.h"
#include "hostinfo.h"

#include "config.h"

using Configuration::cfg;

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
EvtMng::EvtMng(const char * name, const char * addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
EvtMng::EvtMng(std::string name, std::string addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void EvtMng::fromRunningToOperational()
{
    requestQuit = false;

    // Install DirWatcher at inbox folder
    dw = new DirWatcher(Config::PATHBase + "/data/inbox");
    InfoMsg("DirW2tcher installed at " + Config::PATHBase + "/data/inbox");

    transitTo(OPERATIONAL);
    InfoMsg("New state: " + getStateName(getState()));
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void EvtMng::fromOperationalToRunning()
{
    InfoMsg("New state: " + getStateName(getState()));

    // stop watching inbox folder
    dw->stop();

    transitTo(OFF);
}

//----------------------------------------------------------------------
// Method: runEachIteration
//----------------------------------------------------------------------
void EvtMng::runEachIteration()
{
    // 1. Check DirWatcher events from inbox folder
    DirWatcher::DirWatchEvent e;
    while (dw->nextEvent(e)) {

        TRC("New DirWatchEvent: " + e.path + "/" + e.name
                 + (e.isDir ? " DIR " : " ") + std::to_string(e.mask));

        // Process only files
        // TODO: Process directories that appear at inbox
        if (! e.isDir) {
            std::string file(e.path + "/" + e.name);
            // Set new content for InData Message
            FileNameSpec fs;
            ProductMetadata m;
            if (!fs.parseFileName(file, m)) {
                WarnMsg("Problem while trying to parse filename with regex");
                continue;
            }

            Message<MsgBodyINDATA> msg;
            msg.buildHdr(ChnlInData, MsgInData, CHNLS_IF_VERSION,
                         compName, "*",
                         "", "", "");

            MsgBodyINDATA body;
            body["products"][0] = m.val();
            msg.buildBody(body);

            this->send(ChnlInData, msg.str());
        }
    }

    bool sendInit = ((iteration + 1) == 100);
    bool sendPing = ((iteration % 200) == 0);
    bool sendQuit = requestQuit; //(iteration > 1000) || requestQuit;

    if (sendInit || sendPing || sendQuit) {
        Message<MsgBodyCMD> msg;

        if (sendPing) {
            msg.buildHdr(ChnlEvtMng, MsgEvtMng, CHNLS_IF_VERSION,
                         compName, "*",
                         "", "", "");
        } else {
            msg.buildHdr(ChnlCmd, MsgCmd, CHNLS_IF_VERSION,
                         compName, "*",
                         "", "", "");
        }

        MsgBodyCMD body;
        body["iteration"] = std::to_string(iteration);

        if (sendInit) {
            body["cmd"] = CmdInit;
            body["sessionId"] = cfg.sessionId;
        } else if (sendPing) {
            body["cmd"] = CmdPing;
        } else if (sendQuit) {
            body["cmd"] = CmdQuit;
        }

        msg.buildBody(body);

        this->send(sendPing ? ChnlEvtMng : ChnlCmd, msg.str());

        if (sendQuit) { transitTo(RUNNING); }
    }
}

//----------------------------------------------------------------------
// Method: processHMICmdMsg
//----------------------------------------------------------------------
void EvtMng::processHMICmdMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    Message<MsgBodyCMD> msg(m);
    MsgBodyCMD body;
    std::string cmd = msg.body["cmd"].asString();

    if (cmd == CmdStates) { // Component states request

        msg.buildHdr(ChnlHMICmd, MsgHMICmd, CHNLS_IF_VERSION,
                     compName, "*",
                     "", "", "");
        body["cmd"] = CmdStates;
        json states;
        states[compName] = getStateName(getState());
        for(auto & kv : cfg.nodeStates) {
            TraceMsg(kv.first + " => " + kv.second);
            states[kv.first] = kv.second;
        }
        body["states"] = states;

        json logs;
        for (auto & kv : logFolders) {
            TraceMsg(kv.first + " => " + kv.second);
            logs[kv.first] = kv.second;
        }
        body["logs"] = logs;

    } else if (cmd == CmdSession) { // Session id. request

        msg.buildHdr(ChnlHMICmd, MsgHMICmd, CHNLS_IF_VERSION,
                     compName, "*",
                     "", "", "");
        body["cmd"] = CmdSession;
        body["sessionId"] = cfg.sessionId;

    } else if (cmd == CmdConfig) { // Embedded is message to TskAgents

        // Send relay message
        Message<MsgBodyCMD> relayMsg(m);
        relayMsg.buildHdr(ChnlCmd, MsgCmd, CHNLS_IF_VERSION,
                          compName, "*",
                          "", "", "");
        this->send(ChnlCmd, relayMsg.str());
                    
        // Build HMICmd answer
        msg.buildHdr(ChnlHMICmd, MsgHMICmd, CHNLS_IF_VERSION,
                     compName, "HMIProxy",
                     "", "", "");
        body["ans"] = "OK";
        
    } else if (cmd == CmdProcHdl) { // Embedded is message to TskAgents

        // Send relay message
        Message<MsgBodyCMD> relayMsg(m);
        relayMsg.buildHdr(ChnlCmd, MsgCmd, CHNLS_IF_VERSION,
                          compName, "*",
                          "", "", "");
        this->send(ChnlCmd, relayMsg.str());
                    
        // Build HMICmd answer
        msg.buildHdr(ChnlHMICmd, MsgHMICmd, CHNLS_IF_VERSION,
                     compName, "HMIProxy",
                     "", "", "");
        body["ans"] = "OK";
        
    } else if (cmd == CmdReproc) { // Reprocessing products request

    } else if (cmd == CmdQuit) { // Quit request

        requestQuit = true;
        return;

    }

    msg.buildBody(body);

    this->send(ChnlHMICmd, msg.str());
}

//}
