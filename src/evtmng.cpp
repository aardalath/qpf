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
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
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

#include "dbhdlpostgre.h"
#include "except.h"
#include "urlhdl.h"
#include "config.h"
#include "filenamespec.h"

#include <csignal>
#include <cstdlib>
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
    canProcessMessage(MSG_MONIT_INFO_IDX);
    canProcessMessage(MSG_CMD_IDX);

    setHeartBeatPeriod(0, 200000);
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
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void EventManager::fromRunningToOperational()
{
    ConfigurationInfo & cfgInfo = ConfigurationInfo::data();

    // Install DirWatcher at inbox folder
    dw = new DirWatcher(cfgInfo.storage.inbox.path);

    /* Sample System Alerts
    Alert a1(Alert::System, Alert::Warning, Alert::Resource,
             Alert::here(__func__, __LINE__),
             "Sample alert");

    Alert a2(Alert::System, Alert::Warning, Alert::Resource,
             Alert::here(__FILE__, __LINE__),
             "Another sample alert",
             Alert::createNewVar("myvar", 93.0f, 0.0f, 50.0f));

    RaiseAlert(a1);
    RaiseAlert(a2);
    */
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void EventManager::fromOperationalToRunning()
{
    // stop watching inbox folder
    dw->stop();

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
// Method: execAdditonalLoopTasks
//----------------------------------------------------------------------
void EventManager::execAdditonalLoopTasks()
{  
    // Check DirWatcher events from inbox folder
    DirWatcher::DirWatchEvent e;
    while (dw->nextEvent(e)) {
        //std::cout << e.path << "/" << e.name << (e.isDir ? " DIR " : " ") << e.mask << std::endl;

        // Process only files
        // TODO: Process directories that appear at inbox
        if (! e.isDir) {
            std::string file(e.path + "/" + e.name);
            // Set new content for InData Message
            FileNameSpec fs;
            ProductMetadata m;
            fs.parseFileName(file, m);
            /*
            std::cerr << "mission        : " << m.mission << "\n";
            std::cerr << "creator        : " << m.creator << "\n";
            std::cerr << "origin         : " << m.origin << "\n";
            std::cerr << "procFunc       : " << m.procFunc << "\n";
            std::cerr << "params         : " << m.params << "\n";
            std::cerr << "instrument     : " << m.instrument << "\n";
            std::cerr << "obsIdSt        : " << m.obsIdStr << "\n";
            std::cerr << "obsId          : " << m.obsId << "\n";
            std::cerr << "obsMode        : " << m.obsMode << "\n";
            std::cerr << "expos          : " << m.expos << "\n";
            std::cerr << "productType    : " << m.productType << "\n";
            std::cerr << "signature      : " << m.signature << "\n";
            std::cerr << "productId      : " << m.productId << "\n";
            std::cerr << "productVersion : " << m.productVersion << "\n";
            std::cerr << "productStatus  : " << m.productStatus << "\n";
            std::cerr << "startTime      : " << m.startTime << "\n";
            std::cerr << "endTime        : " << m.endTime << "\n";
            std::cerr << "regTime        : " << m.regTime << "\n";
            std::cerr << "productSize    : " << m.productSize << "\n";
            std::cerr << "fileType       : " << m.fileType << "\n";
            std::cerr << "url            : " << m.url << "\n";
            std::cerr << "urlSpace       : " << m.urlSpace << "\n";
            */
            ProductCollection products;
            products.productList[m.productType] = m;

            // Create message and send it to appropriate targets
            std::array<std::string,1> fwdRecip = {"DataMng"};
            for (std::string & recip : fwdRecip) {
                MessageHeader hdr;
                buildMsgHeader(MSG_INDATA_IDX, "EvtMng", recip, hdr);

                Message_INDATA msg;
                buildMsgINDATA(hdr, products, msg);

                // Send message
                PeerMessage * pmsg = buildPeerMsg(hdr.destination, msg.getDataString(), MSG_INDATA);
                registerMsg(selfPeer()->name, *pmsg);
                setTransmissionToPeer(hdr.destination, pmsg);
            }
        }
    }

    // 2. Check possible commands in DB
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    std::string cmdSource;
    std::string cmdContent;
    int cmdId;
    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        // Store new state
        dbHdl->getICommand(selfPeer()->name, cmdId, cmdSource, cmdContent);

        if (cmdContent == "QUIT") {
            InfoMsg("Leaving OPERATIONAL state triggered by an iCommand...");
            transitTo(RUNNING);
            // Mark command as executed
            dbHdl->markICommandAsDone(cmdId);
        } else if (cmdContent == "PING") {
            InfoMsg("QPFHMI requests answer. . .");
            dbHdl->removeICommand(cmdId);
            // Add answer command
            dbHdl->addICommand(cmdSource, selfPeer()->name, "PONG");
        }
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }
    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: processINDATA
//----------------------------------------------------------------------
void EventManager::processINDATA()
{
    // If INDATA messages is coming from the outside, download the files
    // into the INBOX and adapt the URLs;
    // otherwise, just link the files from their folder into the INBOX

    Message_INDATA * currMsg = dynamic_cast<Message_INDATA*>(msgData.msg);
    std::map<ProductType, ProductMetadata>::iterator it =
            currMsg->productsMetadata.productList.begin();
    ProductMetadata & m = it->second;
    bool syntheticMsg = (m.url.substr(0,6) == "file:/");

    URLHandler urlh;

    if (syntheticMsg) {
        // Synthetic INDATA messages, that means reading products from folder
        for (auto & md : currMsg->productsMetadata.productList) {
            urlh.setProduct(md.second);
            md.second = urlh.fromFolder2Inbox();
        }
    } else {
        // Non-synthetic, that means external INDATA messages, that is,
        // external products that must be downloaded

        // TODO: download external products into inbox
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

    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);
    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        if (!dbHdl->updateTask(msg->task)) {
            WarnMsg("Could not update task information in DB for task " +
                    msg->task.taskData["NameExtended"].asString());
        }
    } catch (...) {
        ErrMsg("Error when trying to update db info of task " +
                msg->task.taskData["NameExtended"].asString());
    }
    // Check that connection with the DB is possible
    dbHdl->closeConnection();

    if (msg->task.taskData["State"]["Progress"].asString() == "100") {
        InfoMsg("RECEIVED NOTIFICATION OF TASK " + msg->task.taskName +
                " FINISHED AT " + msg->task.taskEnd);
    }

}

//----------------------------------------------------------------------
// Method: processMONIT_INFO
//----------------------------------------------------------------------
void EventManager::processMONIT_INFO()
{
    Message_MONIT_INFO * msg = dynamic_cast<Message_MONIT_INFO *>(msgData.msg);

    NodeName & source = msg->header.source;
    PList & v = msg->variables.paramList;

    for (auto & it : v) {
        if (it.first == "state") {
            DbgMsg("STATUS of " + source + " is " + it.second);
        }
    }
}

//----------------------------------------------------------------------
// Method: processCMD
//----------------------------------------------------------------------
void EventManager::processCMD()
{
    InfoMsg("Processing CMD message...");

    Message_CMD * msg = dynamic_cast<Message_CMD *>(msgData.msg);

    NodeName & source = msg->header.source;
    PList & v = msg->variables.paramList;

    for (auto & it : v) {
        if (it.first == "quit") {
            InfoMsg("Leaving OPERATIONAL state triggered by a CMD message...");
            transitTo(RUNNING);
        }
    }
}

}
