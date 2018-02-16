/******************************************************************************
 * File:    evtmng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.evtmng
 *
 * Version:  2.0
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
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
    hmiActive = false;
    
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

            {
                std::lock_guard<std::mutex> lock(mtxInData);
                m["urlSpace"] = InboxSpace;
                inboxProducts.products.push_back(m);
            }
        }
    }

    bool sendInit = ((iteration + 1) == 100);
    if (sendInit) {
        Message<MsgBodyCMD> msg;
        msg.buildHdr(ChnlCmd, MsgCmd, CHNLS_IF_VERSION,
                     compName, "*",
                     "", "", "");
        MsgBodyCMD body;
        body["iteration"] = std::to_string(iteration);
        body["cmd"] = CmdInit;
        body["sessionId"] = cfg.sessionId;

        msg.buildBody(body);
        this->send(ChnlCmd, msg.str());
    }
}

//----------------------------------------------------------------------
// Method: processHMICmdMsg
//----------------------------------------------------------------------
void EvtMng::processHMICmdMsg(ScalabilityProtocolRole* c, MessageString & m)
{
    hmiActive = true;
    
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

        ProductList prodList(msg.body["products"]);
        TRC("Received CmdReproc with " +
            std::to_string(prodList.products.size()) + " products");
        std::lock_guard<std::mutex> lock(mtxReproc);
        reprocProducts.products = std::move(prodList.products);

    } else if (cmd == CmdQuit) { // Quit request

        requestQuit = true;
        return;

    }

    msg.buildBody(body);

    this->send(ChnlHMICmd, msg.str());
}

//----------------------------------------------------------------------
// Method: getInData
// Store in argument variables the INDATA products
//----------------------------------------------------------------------
bool EvtMng::getInData(ProductList & inData, std::string & space)
{
    bool retVal = inboxProducts.products.size() > 0;
    if (retVal) {
        std::lock_guard<std::mutex> lock(mtxInData);
        inData.products = std::move(inboxProducts.products);
        space = inData.products.at(0).urlSpace();
    }
    return (retVal);
}

//----------------------------------------------------------------------
// Method: getReprocData
// Store in argument variables the REPROCDATA products
//----------------------------------------------------------------------
bool EvtMng::getReprocData(ProductList & reprocData)
{
    bool retVal = reprocProducts.products.size() > 0;
    if (retVal) {
        std::lock_guard<std::mutex> lock(mtxReproc);
        reprocData.products = std::move(reprocProducts.products);
    }
    return (retVal);
}

//----------------------------------------------------------------------
// Method: isHMIActive
//----------------------------------------------------------------------
bool EvtMng::isHMIActive()
{
    return hmiActive;
}

//----------------------------------------------------------------------
// Method: isQuitRequested
//----------------------------------------------------------------------
bool EvtMng::isQuitRequested()
{
    return requestQuit;
}

//----------------------------------------------------------------------
// Method: isQuitRequested
//----------------------------------------------------------------------
void EvtMng::quit()
{
    // Send QUIT messages
    Message<MsgBodyCMD> msg;
    msg.buildHdr(ChnlCmd, MsgCmd, CHNLS_IF_VERSION,
                 compName, "*",
                 "", "", "");

    MsgBodyCMD body;
    body["iteration"] = std::to_string(iteration);
    body["cmd"] = CmdQuit;

    msg.buildBody(body);
    this->send(ChnlCmd, msg.str());

    // Transit to RUNNING (no way back)
    transitTo(RUNNING); 
}

//----------------------------------------------------------------------
// Method: sendProcFmkInfoUpdate
// Send an update on the ProcessingFrameworkInfo structure
//----------------------------------------------------------------------
void EvtMng::sendProcFmkInfoUpdate(json & fmkInfoValue)
{
    std::unique_lock<std::mutex> ulck(mtxHostInfo);

    // Prepare message and send it
    Message<MsgBodyTSK> msg;
    MsgBodyTSK body;
    body["info"] = fmkInfoValue;
    msg.buildBody(body);

    // Set message header
    msg.buildHdr(ChnlFmkMon, MsgFmkMon, CHNLS_IF_VERSION,
                 compName, "HMIProxy", "", "", "");

    // Send msg
    this->send(ChnlFmkMon, msg.str());        
}

