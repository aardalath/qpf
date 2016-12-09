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
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016 Euclid SOC Team @ ESAC
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
    setHeartBeatPeriod(5, 500000);
    canProcessMessage(MSG_MONIT_INFO_IDX);
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
// Method: init
// Initialize the component
//----------------------------------------------------------------------
void HMIProxy::init()
{
    isPeerLogMng = false;
    isRemote     = false;
    session      = ConfigurationInfo::data().session;
}

//----------------------------------------------------------------------
// Method: execAdditonalLoopTasks
//----------------------------------------------------------------------
void HMIProxy::execAdditonalLoopTasks()
{
    MessageHeader hdr;
    buildMsgHeader(MSG_MONIT_RQST_IDX, "QPFHMI", "", hdr);
    Message_MONIT_RQST msg;
    msg.header = hdr;
    msg.variables.paramList["state"] = "";

    std::cerr << "Requesting states at " << LibComm::sessionTag() << std::endl;
    
    PeerMessage * stateMsg = buildPeerMsg("", msg.getDataString(), MSG_MONIT_RQST);
    broadcast(stateMsg);
}

//----------------------------------------------------------------------
// Method: processTASK_RES
//----------------------------------------------------------------------
void HMIProxy::processMONIT_INFO()
{
    Message_MONIT_RQST * msg = dynamic_cast<Message_MONIT_RQST *>(msgData.msg);
    
    std::map<std::string, std::string>::iterator it = msg->variables.paramList.find("state");
    if (it != msg->variables.paramList.end()) {
        nodeStates[msg->header.source] = (*it).second;
        std::cerr << "State of " << msg->header.source << " is " << (*it).second << std::endl;
    }
}

}
