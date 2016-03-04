/******************************************************************************
 * File:    datamng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.datamng
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
 *   Implement DataManager class
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

#include "datamng.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include "dbhdlpostgre.h"
#include "except.h"
#include "config.h"

#include <sys/time.h>
#include <unistd.h>

#include <utility>

//#define DUMP_TASK_CONTENT_TO_LOG

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
DataManager::DataManager(const char * name) :
    Component(name)
{
    canProcessMessage(MSG_INDATA_IDX);
    canProcessMessage(MSG_TASK_PROC_IDX);
    canProcessMessage(MSG_TASK_RES_IDX);
}

//----------------------------------------------------------------------
// Method: fromInitialisedToRunning
//----------------------------------------------------------------------
void DataManager::fromRunningToOperational()
{
    initializeDB();
}

//----------------------------------------------------------------------
// Method: fromOperationalToRunning
//----------------------------------------------------------------------
void DataManager::fromOperationalToRunning()
{
    // nothing yet
}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void DataManager::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: processINDATA
//----------------------------------------------------------------------
void DataManager::processINDATA()
{
    // Save to DB
    Message_INDATA * msg = dynamic_cast<Message_INDATA *>(msgData.msg);
    saveToDB(msg);

    // Send InData message to TaskOrc
    std::array<std::string,1> fwdRecip = {"TskOrc"};
    for (std::string & recip : fwdRecip) {
        // Forward to recipient
        MessageData msgDataToRecip(new Message_INDATA);
        msgDataToRecip.msg->setData(msg->getData());
        Json::StyledWriter w;
        setForwardTo(recip, msgDataToRecip.msg->header);
        PeerMessage * msgForRecip = buildPeerMsg(msgDataToRecip.msg->header.destination,
                                                 msgDataToRecip.msg->getDataString(),
                                                 MSG_INDATA);
        registerMsg(selfPeer()->name, *msgForRecip);
        setTransmissionToPeer(recip, msgForRecip);
    }
}

//----------------------------------------------------------------------
// Method: processTASK_PROC
//----------------------------------------------------------------------
void DataManager::processTASK_PROC()
{
    Message_TASK_Processing * msg = dynamic_cast<Message_TASK_Processing *>(msgData.msg);
    saveTaskToDB(msg, true);
}

//----------------------------------------------------------------------
// Method: processTASK_RES
//----------------------------------------------------------------------
void DataManager::processTASK_RES()
{
    Message_TASK_Processing * msg = dynamic_cast<Message_TASK_Processing *>(msgData.msg);
    saveTaskToDB(msg);
}

//----------------------------------------------------------------------
// Method: initializeDB
// Initialize the DB
// (currently just an INI text file)
//----------------------------------------------------------------------
void DataManager::initializeDB()
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    // Check that connection with the DB is possible
    try {
        dbHdl->openConnection();
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: saveToDB
// Save the information of a new (incoming) product to the DB
// (currently just an INI text file)
//----------------------------------------------------------------------
void DataManager::saveToDB(Message_INDATA * msg)
{
    InfoMsg("Saving inputs...");
    saveProductsToDB(msg->productsMetadata);
}

//----------------------------------------------------------------------
// Method: saveTaskToDB
// Save the information on generated output products to the archive
//----------------------------------------------------------------------
void DataManager::saveTaskToDB(Message_TASK_Processing * msg, bool initialStore)
{
    // Save task information in task_info table
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();

        // Try to store the task data into the DB
        if (initialStore) {
            dbHdl->storeTask(msg->task);
        } else {
            dbHdl->updateTask(msg->task);
        }
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();

    // In case the task has finished, save output products metadata
    if (!msg->task.taskEnd.empty()) {
        InfoMsg("Saving outputs...");
        saveProductsToDB(msg->task.outputs);
    }
}

//----------------------------------------------------------------------
// Method: saveProductsToDB
// Save the information of a new (incoming) product to the DB
// (currently just an INI text file)
//----------------------------------------------------------------------
void DataManager::saveProductsToDB(ProductCollection & productList) //Json::Value & prodMetadata)
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();

        // Try to store the data into the DB
        dbHdl->storeProducts(productList);
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        ErrMsg(productList.getDataString());
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: isProductAvailable
// Returns TRUE if there is a product type like the requested in the DB
//----------------------------------------------------------------------
bool DataManager::isProductAvailable(std::string prodType)
{
    //TODO  // return (ini.exists("ProductsIdx", prodType));
    UNUSED(prodType);

    return true;
}

//----------------------------------------------------------------------
// Method: getProductLatest
// Returns TRUE if there is a product type like the requested in the DB
//----------------------------------------------------------------------
bool DataManager::getProductLatest(std::string prodType,
                                   Json::Value & prodMetadata)
{
    bool retVal = true;
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();

        // Try to store the data into the DB
        ProductList prodList;
        std::string criteria = "WHERE p.product_type = '" + prodType +
                               "' LIMIT 1";
        dbHdl->retrieveProducts(prodList, criteria);
        if (prodList.productList.size() > 0) {
            prodMetadata = prodList.productList.at(0).getData();
            retVal = false;
        }
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        retVal = false;
    }

    // Close connection
    dbHdl->closeConnection();

    return retVal;
}

}
