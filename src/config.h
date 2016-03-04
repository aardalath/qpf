/******************************************************************************
 * File:    config.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.Configuration
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
 *   Declare Configuration class
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

#ifndef CONFIG_H
#define CONFIG_H

//------------------------------------------------------------
// Topic: System dependencies
//------------------------------------------------------------
#include <string>
#include <map>

//------------------------------------------------------------
// Topic: Project dependencies
//------------------------------------------------------------
#include "propdef.h"

#include "r2rpeer.h"
using LibComm::Router2RouterPeer;

#include "json/json.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {
  
//==========================================================================
// Class: Configuration
//==========================================================================
class Configuration {

public:
    Configuration(std::string fName = std::string());
    Configuration(const char * fName = 0);

public:

    //----------------------------------------------------------------------
    // Method: getGeneralInfo
    // Get general parameters from the configuration
    //----------------------------------------------------------------------
    void getGeneralInfo(std::string & appName, std::string appVer, std::string & last);
    
    //----------------------------------------------------------------------
    // Method: setLastAccess
    // Updates the date of last access to the configuration
    //----------------------------------------------------------------------
    void setLastAccess(std::string last);

    //----------------------------------------------------------------------
    // Method: getProductTypes
    // Return vector of product type identifiers
    //----------------------------------------------------------------------
    void getProductTypes(std::vector<std::string> & vec);

    //----------------------------------------------------------------------
    // Method: getNumOrchRules
    // Return number of orchestration rules
    //----------------------------------------------------------------------
    int getNumOrchRules();

    //----------------------------------------------------------------------
    // Method: getOrchRule
    // Return orchestration rule parameters
    //----------------------------------------------------------------------
    void getOrchRule(std::string & name, std::vector<std::string> & in,
                     std::vector<std::string> & out, std::string & pElem);
    
    //----------------------------------------------------------------------
    // Method: getNumProcs
    // Return number of processors
    //----------------------------------------------------------------------
    int getNumProcs();

    //----------------------------------------------------------------------
    // Method: getProc
    // Return processor parameters
    //----------------------------------------------------------------------
    void getProc(std::string & name, std::string & exe,
                 std::string & in, std::string & out);
    
    //----------------------------------------------------------------------
    // Method: getNumNodes
    // Return number of nodes
    //----------------------------------------------------------------------
    int getNumNodes();

    //----------------------------------------------------------------------
    // Method: getNode
    // Return node parameters
    //----------------------------------------------------------------------
    void getNode(std::string & name, std::string & type,
                 std::string & cAddr, std::string & sAddr);

    //----------------------------------------------------------------------
    // Method: getNodeByName
    // Return node parameters for a node with the name specified
    //----------------------------------------------------------------------
    void getNodeByName(std::string name, std::string & type,
                       std::string & cAddr, std::string & sAddr);

    //----------------------------------------------------------------------
    // Method: getHMINodeName
    // Return HMI node name
    //----------------------------------------------------------------------
    std::string getHMINodeName();

private:

    //----------------------------------------------------------------------
    // Method: setConfigFile
    // Set configuration file name
    //----------------------------------------------------------------------
    void setConfigFile(std::string fName);

    //----------------------------------------------------------------------
    // Method: applyNewConfig
    // Get new configuration information from an external (JSON) file
    //----------------------------------------------------------------------
    void applyNewConfig(std::string fName);

    //----------------------------------------------------------------------
    // Method: readConfiguration
    // Loads the configuration file content into memory
    //----------------------------------------------------------------------
    void readConfigurationFromFile();
    
    //----------------------------------------------------------------------
    // Method: readConfigurationFromDB
    // Loads the configuration from the DB into memory
    //----------------------------------------------------------------------
    void readConfigurationFromDB();

    //----------------------------------------------------------------------
    // Method: saveConfigurationToDB
    // Store the configuration into the DB
    //----------------------------------------------------------------------
    void saveConfigurationToDB();

private:
    std::string cfgFileName;
    Json::Value cfg;

public:
    static std::string DBHost;
    static std::string DBPort;
    static std::string DBName;
    static std::string DBUser;
    static std::string DBPwd;

};

}

#endif  /* CONFIG_H */

