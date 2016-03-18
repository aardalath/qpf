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
#include <iostream>

//------------------------------------------------------------
// Topic: Project dependencies
//------------------------------------------------------------
#include "cfginfo.h"

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
    // Method: getCfgInfo
    // Get entire configuration info structure
    //----------------------------------------------------------------------
    ConfigurationInfo & getCfgInfo();

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
    // Method: reset
    // Restarts iterators to begin of containers
    //----------------------------------------------------------------------
    void reset();

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
                     std::vector<std::string> & out, std::string & pElem,
                     std::string & condition);

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

    //----------------------------------------------------------------------
    // Method: getNumMachines
    // Return number of machines in the network
    //----------------------------------------------------------------------
    int getNumMachines();

    //----------------------------------------------------------------------
    // Method: getMachine
    // Return list of nodes to be deployed in a machine
    //----------------------------------------------------------------------
    void getMachine(std::string & machine,
                    std::vector<std::string> & vec);

    //----------------------------------------------------------------------
    // Method: getConnectionsForNode
    // Return list of nodes connecting to a given one
    //----------------------------------------------------------------------
    void getConnectionsForNode(std::string nodeName,
                               std::vector<std::string> & vec);

    //----------------------------------------------------------------------
    // Method: getEnvVar
    // Returns the content of an environment variable
    //----------------------------------------------------------------------
    std::string getEnvVar(std::string const & key) const;

private:

    //----------------------------------------------------------------------
    // Method: getExternalStorage
    // Stores the values of a StorageExternal strucure from configuration
    // into the appropriate data structure
    //----------------------------------------------------------------------
    void getExternalStorage(const Json::Value & section,
                            StorageExternal & ext);

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

    //----------------------------------------------------------------------
    // Method: processConfiguration
    // Convert data in cfg (Json) to cfgInfo structure
    //----------------------------------------------------------------------
    void processConfiguration();

private:
    std::string           cfgFileName;
    std::string           cfgFilePath;
    Json::Value           cfg;

    Json::Value::iterator ruleIt;
    Json::Value::iterator procIt;
    Json::Value::iterator nodeIt;
    Json::Value::iterator machIt;

    bool hmiPresent;

public:
    static std::string DBHost;
    static std::string DBPort;
    static std::string DBName;
    static std::string DBUser;
    static std::string DBPwd;

    static std::string PATHBase;
    static std::string PATHBin;
    static std::string PATHRun;
    static std::string PATHLog;
    static std::string PATHRlog;
    static std::string PATHTmp;
    static std::string PATHTsk;
    static std::string PATHMsg;

    static mode_t      PATHMode;

};

//----------------------------------------------------------------------
// Method: isLE1Product
// Returns TRUE if the product type corresponds to a LE1 product file
//----------------------------------------------------------------------
bool isLE1Product(std::string const & p);

//----------------------------------------------------------------------
// Method: isLE1Metadata
// Returns TRUE if the product type corresponds to a LE1 metadata file
//----------------------------------------------------------------------
bool isLE1Metadata(std::string const & p);

//----------------------------------------------------------------------
// Method: isValidExtension
// Returns TRUE if the file extension provided is one of the valid ones
//----------------------------------------------------------------------
bool isValidExtension(std::string const & e);

}

#endif  /* CONFIG_H */
