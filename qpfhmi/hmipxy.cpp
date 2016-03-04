/******************************************************************************
 * File:    hmipxy.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.hmipxy
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
 *   Implement HMIProxy class
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

#include "hmipxy.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {   

//#define DUMP_TASK_CONTENT_TO_LOG

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
HMIProxy::HMIProxy(const char * name) :
    Component(name)
{
    startSignalFromHMIReceived = false;
    taskResInfo.clear();

    canProcessMessage(MSG_TASK_RES_IDX);
}

//----------------------------------------------------------------------
// Method: getStartSignal
// Propagate START signal to node
//----------------------------------------------------------------------
void HMIProxy::getStartSignal()
{
    startSignalFromHMIReceived = true;
}

//----------------------------------------------------------------------
// Method: concurrentRun
// Method executed when the thread is created (for QtConcurrent)
//----------------------------------------------------------------------
int HMIProxy::concurrentRun()
{
    return this->run();
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void HMIProxy::fromOperationalToRunning()
{
    // nothing yet
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void HMIProxy::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: processTASK_RES
//----------------------------------------------------------------------
void HMIProxy::processTASK_RES()
{
    Message_TASK_RES * msg = dynamic_cast<Message_TASK_RES *>(msgData.msg);
    registerTaskRes(msg->getData());
}

//----------------------------------------------------------------------
// Method: send
// Method to send messages to Event Manager
//----------------------------------------------------------------------
void HMIProxy::sendInData(ProductMetadata metadata)
{
    MessageHeader hdr;
    buildMsgHeader(MSG_INDATA_IDX, "QPFHMI", "EvtMng", hdr);

    ProductCollection products;
    products.productList[metadata.productType] = metadata;

    Message_INDATA msg;
    buildMsgINDATA(hdr, products, msg);

    // Send message
    PeerMessage * pmsg = buildPeerMsg(hdr.destination, msg.getDataString(), MSG_INDATA);
    registerMsg(selfPeer()->name, *pmsg);
    setTransmissionToPeer(hdr.destination, pmsg);
}

//----------------------------------------------------------------------
// Method: requestStop
// Send STOP request to Event Manager
//----------------------------------------------------------------------
void HMIProxy::requestStop()
{
    InfoMsg("Requesting STOP . . .");
    PeerMessage * msg = buildPeerMsg("EvtMng", "Please, shut down!", MSG_STOP);
    registerMsg(selfPeer()->name, *msg);
    setTransmissionToPeer("EvtMng", msg);
}

bool checkForCorruption(HMIProxy::TaskResultsInfo & m)
{
    int sum = 0;
    for (auto & kv : m) {
        std::string key = kv.first;
        sum += (int)(key[0]);
    }
    return true;
}

//----------------------------------------------------------------------
// Method: registerTaskRes
// Send STOP request to Event Manager
//----------------------------------------------------------------------
void HMIProxy::registerTaskRes(Json::Value jsonValue)
{
    if (!checkForCorruption(taskResInfo)) {
        FatalMsg("taskResInfo got corrupted!!");
    }

#ifdef DUMP_TASK_CONTENT_TO_LOG
    InfoMsg("Registering results:\n" + jsonValue.toStyledString());
#endif
    static int idx = 0;
    std::string msgFileName("/tmp/");
    char baseName[100];
    sprintf(baseName, "v.%04d.json", ++idx);
    msgFileName += baseName;
    std::ofstream msgOut(msgFileName);
    Json::StyledWriter writer;
#define K(x) "\"" << #x << "\": " << x

    msgOut << writer.write(jsonValue) << "\n";
    msgOut << "{ "
           << K(jsonValue.isNull()) << ",\n  "
           << K(jsonValue.isArray()) << ",\n  "
           << K(jsonValue.isObject()) << " }\n";
    Json::Value & v = jsonValue["task"]["taskData"];
    msgOut << writer.write(v) << "\n";
    msgOut << "{ "
           << K(v.isNull()) << ",\n  "
           << K(v.isArray()) << ",\n  "
           << K(v.isObject()) << " }\n";
    if (v.isNull()) {
        // Set minimum information
        v["TaskAgent"] = "-";
        v["Args"][0] = "-";
        v["State"]["StartedAt"] = "-";
        v["State"]["FinishedAt"] = "-";
        v["State"]["Progress"] = "-";
        v["State"]["ExitCode"] = 0;
        v["State"]["Running"] = false;
        v["State"]["Paused"] = true;
    }
    msgOut << writer.write(v) << "\n";
    msgOut << "{ "
           << K(v.isNull()) << ",\n  "
           << K(v.isArray()) << ",\n  "
           << K(v.isObject()) << " }\n";
    msgOut.close();

    if (v.isObject()) {
        std::string key = v["NameExtended"].asString();
        if (key.empty()) return;
        if (taskResInfo.find(key) == taskResInfo.end()) {
            taskResInfo[key] = v;
            thereIsTaskResInfo = true;
            InfoMsg("There are TaskRes information chunks");
        }
    }
}

//----------------------------------------------------------------------
// Method: isThereTaskResInfo
// Provides a flag to poll for new Task Res msgs
//----------------------------------------------------------------------
bool HMIProxy::isThereTaskResInfo()
{
    return thereIsTaskResInfo;
}

//----------------------------------------------------------------------
// Method: getTaskResInfo
// Provides a vector with the new task res info
//----------------------------------------------------------------------
int HMIProxy::getTaskResInfo(TaskResultsInfo & newTaskInfo)
{
    newTaskInfo = taskResInfo;
    taskResInfo.clear();
    thereIsTaskResInfo = false;
    return newTaskInfo.size();
}

}


