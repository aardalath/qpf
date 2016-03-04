/******************************************************************************
 * File:    taskagent.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskagent
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
 *   Implement TaskAgent class
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

#include "taskagent.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

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
TaskAgent::TaskAgent(const char * name) :
    Component(name), workDir("/home/jcgonzalez/ws/docker-data/tools/")

{
    canProcessMessage(MSG_TASK_PROC_IDX);
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TaskAgent::fromRunningToOperational()
{
    stopTasks = false;
    numTasks = 0;
    numRunningTasks = 0;
    maxRunningTasks = 3;
    numWaitingTasks = 0;
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void TaskAgent::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
    flushLog();

    stopTasks = true;
    /*
    for (unsigned int i = 0; i < procTasks.size(); ++i) {
        procTasks.at(i)->join();
    }
    */
}

//----------------------------------------------------------------------
// Method: processTASK_PROC
//----------------------------------------------------------------------
void TaskAgent::processTASK_PROC()
{
    // Check the product type as input for any rule
    Message_TASK_PROC * msg = dynamic_cast<Message_TASK_PROC *>(msgData.msg);
    InfoMsg("Processor to be launched: " + msg->task.taskPath);

    // Create new processing element with the task information
    ProcessingElement * pe = new ProcessingElement();

    ++numTasks;
    ++numWaitingTasks;

    pe->setAgentName(selfPeer()->name);
    pe->setTaskInfo(msg->task);
    pe->setNumTask(numTasks);
    pe->setWorkingDir(workDir);

    // Store it at the PEs vector
    processingElements.push_back(pe);

    // Launch the PE
    pe->exec();

    // Send first TASK_RES message
    sendTaskResMsg(pe->getTask());
}

//----------------------------------------------------------------------
// Method: execAdditonalLoopTasks
//----------------------------------------------------------------------
void TaskAgent::execAdditonalLoopTasks()
{
    checkProcessingElements();
}

//----------------------------------------------------------------------
// Method: checkProcessingElements
// Check list of processing elements
//----------------------------------------------------------------------
void TaskAgent::checkProcessingElements()
{
    for (unsigned int i = 0; i < processingElements.size(); ++i) {

        bool sendMsg        = false;
        bool markAsArchived = false;

        ProcessingElement * pe = processingElements.at(i);

        TaskStatus status = pe->getStatus();

        switch (status) {

        case TASK_ARCHIVED:
            // Task already finished, and final information was sent to be
            // registered in the DB, so the pe can be deleted
            InfoMsg("Removing proc.elem. for task " + pe->getTask().taskName);

            delete pe;
            pe = 0;
            processingElements.erase(processingElements.begin() + i);
            sendMsg = false;
            break;

        case TASK_FINISHED:
            // Send latest information for completed task
            // And decrease the number of tasks running
            --numRunningTasks;
            sendMsg = true;
            break;

        case TASK_SCHEDULED:
            // The number of running tasks was equal to or above the threshold
            // Check if this PE can be brought to life
            if (numRunningTasks < maxRunningTasks) {
                pe->start();
                ++numRunningTasks;
                --numWaitingTasks;
                InfoMsg("Starting proc.elem. " + selfPeer()->name +
                        "-" + LibComm::toStr<int>(i + 1));
            }
            // It could be that the task is just starting, se we do not
            // send message about stated task, but wait until next iteration
            sendMsg = false;
            break;

        case TASK_FAILED:
            // Send latest information for task
            // ... and set processing element status to ARCHIVED
            markAsArchived = true;
            sendMsg = true;
            break;

        case TASK_PAUSED:
            // Task is idle, do nothing, just report its state

            break;

        case TASK_STOPPED:
            // Task has been explicitly stopped
            // Send latest information for task
            // ... and set processing element status to ARCHIVED
            markAsArchived = true;
            sendMsg = true;
            break;

        case TASK_RUNNING:
            // Update information about the processing element
            // and send it to the managing components
            sendMsg = true;
            break;

        default:
            break;
        }

        if (sendMsg) {
            sendTaskResMsg(pe->getTask());
        }

        if (markAsArchived) {
            pe->markAsArchived();
        }
    }
}

//----------------------------------------------------------------------
// Method: sendTaskResMsg
// Send a TaskResultMsg to the Task Manager, with information on the
// processing task at hand
//----------------------------------------------------------------------
bool TaskAgent::sendTaskResMsg(TaskInfo & task)
{
    sendMsgsMutex.lock();
    Message_TASK_RES msg;
    buildMsgHeader(MSG_TASK_RES_IDX, selfPeer()->name, "TskMng", msg.header);
    msg.task.setData(task.getData());
    PeerMessage * taskResMsg = buildPeerMsg("TskMng", msg.getDataString(), MSG_TASK_RES);
    registerMsg(selfPeer()->name, *taskResMsg);
    setTransmissionToPeer("TskMng", taskResMsg);
    sendMsgsMutex.unlock();

    return true;
}

}
