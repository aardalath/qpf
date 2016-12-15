/******************************************************************************
 * File:    datamng.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.DataManager
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
 *   Declare DataManager class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   Component
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

#ifndef DATAMNG_H
#define DATAMNG_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - component.h
//------------------------------------------------------------

#include "component.h"

namespace Json {
class Value;
}

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: DataManager
//==========================================================================
class DataManager : public Component {

public:
    DataManager(const char * name = 0);

    //----------------------------------------------------------------------
    // Method: isProductAvailable
    // Returns TRUE if there is a product type like the requested in the DB
    //----------------------------------------------------------------------
    bool isProductAvailable(std::string prodType);

    //----------------------------------------------------------------------
    // Method: getProductLatest
    // Returns TRUE if there is a product type like the requested in the DB
    //----------------------------------------------------------------------
    bool getProductLatest(std::string prodType,
                          Json::Value & prodMetadata);

protected:

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

protected:

    //----------------------------------------------------------------------
    // Method: processINDATA
    //----------------------------------------------------------------------
    virtual void processINDATA();

    //----------------------------------------------------------------------
    // Method: processTASK_PROC
    //----------------------------------------------------------------------
    virtual void processTASK_PROC();

    //----------------------------------------------------------------------
    // Method: processTASK_RES
    //----------------------------------------------------------------------
    virtual void processTASK_RES();

protected:

    //----------------------------------------------------------------------
    // Method: initializeDB
    // Initialize the DB
    // (currently just an INI text file)
    //----------------------------------------------------------------------
    void initializeDB();

    //----------------------------------------------------------------------
    // Method: saveToDB
    // Save the information of a new (incoming) product to the DB
    // (currently just an INI text file)
    //----------------------------------------------------------------------
    void saveToDB(Message_INDATA * msg);

    //----------------------------------------------------------------------
    // Method: saveTaskToDB
    // Save the information on generated output products to the archive
    //----------------------------------------------------------------------
    void saveTaskToDB(Message_TASK_Processing * msg, bool initialStore = false);

    //----------------------------------------------------------------------
    // Method: saveProductsToDB
    // Save the information of a new (incoming) product to the DB
    // (currently just an INI text file)
    //----------------------------------------------------------------------
    void saveProductsToDB(ProductCollection & productList); //Json::Value & prodMetadata);

    //----------------------------------------------------------------------
    // Method: archiveDSSnEAS
    // Sends the information to the area where the corresponding daemon is
    // looking for data to be sent to DSS/EAS
    //----------------------------------------------------------------------
    void archiveDSSnEAS(ProductCollection & productList);

private:
    std::string dbFileName;

};

}

#endif  /* DATAMNG_H */
