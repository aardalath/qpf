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

#include "dbhdlpostgre.h"
#include "except.h"
#include "urlhdl.h"
#include "config.h"
#include "filenamespec.h"

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

    Alert a1(Alert::System, Alert::Warning, Alert::Resource,
             Alert::here(__func__, __LINE__),
             "Sample alert");

    Alert a2(Alert::System, Alert::Warning, Alert::Resource,
             Alert::here(__FILE__, __LINE__),
             "Another sample alert",
             Alert::createNewVar("myvar", 93.0f, 0.0f, 50.0f));

    RaiseAlert(a1);
    RaiseAlert(a2);
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
    if (dw->nextEvent(e)) {
        std::cout << e.path << "/" << e.name << (e.isDir ? " DIR " : " ") << e.mask << std::endl;

        // Process only files
        // TODO: Process directories that appear at inbox
        if (! e.isDir) {
            std::string file(e.path + "/" + e.name);
            FileNameSpec fs;
            FileNameSpec::FileNameComponents c = fs.parseFileName(file);

            // Set new content for InData Message
            ProductMetadata m;
            m.startTime      = c.dateStart;
            m.endTime        = c.dateEnd;
            m.creator        = "UNKNOWN";
            m.instrument     = c.instrument;
            m.productId      = c.productId;
            m.productType    = c.productType;
            m.productVersion = c.version;
            m.productStatus  = "OK";
            m.productSize    = e.size;
            m.signature      = c.signature;
            m.url            = "file://" + file;
            m.urlSpace       = InboxSpace;

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
            InfoMsg("Requesting STOP to all components. . .");
            PeerMessage * msg = buildPeerMsg(selfPeer()->name, "Please, shut down!", MSG_STOP);
            registerMsg(selfPeer()->name, *msg);
            setTransmissionToPeer(selfPeer()->name, msg);
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
/*
    MessageData msgToHMI(new Message_TASK_RES);
    msgToHMI.msg->setData(msg->getData());
    setForwardTo("QPFHMI", msgToHMI.msg->header);
    PeerMessage * msgForHMI = buildPeerMsg(msgToHMI.msg->header.destination,
                                           msgToHMI.msg->getDataString(),
                                           MSG_TASK_RES);
    registerMsg(selfPeer()->name, *msgForHMI);
    setTransmissionToPeer("QPFHMI", msgForHMI);
*/
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
}

}
