/******************************************************************************
 * File:    datamng.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.datamng
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
 *   Implement DataMng class
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

#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <utility>

#include "log.h"
#include "tools.h"

#include "filenamespec.h"
#include "dbhdlpostgre.h"
#include "except.h"
#include "config.h"
#include "dbg.h"
#include "str.h"
#include "qdtrephdl.h"
#include "urlhdl.h"
#include "channels.h"
#include "message.h"
#include "hostinfo.h"
#include "launcher.h"

#include "config.h"

using Configuration::cfg;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
DataMng::DataMng(const char * name, const char * addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Method: fromInitialisedToRunning
//----------------------------------------------------------------------
DataMng::DataMng(std::string name, std::string addr, Synchronizer * s)
    : Component(name, addr, s)
{
}

//----------------------------------------------------------------------
// Method: storeTskRegData
//----------------------------------------------------------------------
void DataMng::storeTskRegData(json & tskRepData)
{
    for (Json::ValueIterator itr = tskRepData.begin();
         itr != tskRepData.end(); ++itr) {

        TaskInfo task(*itr);
        
        std::string taskName  = task.taskName();
        TaskStatus taskStatus = TaskStatus(task.taskStatus());
        
        TraceMsg("DataMng: Processing TaskReport: " + taskName
                 + " has status " + TaskStatusName[taskStatus]);
        
        saveTaskToDB(task);
    }
}

//----------------------------------------------------------------------
// Method: initializeDB
// Initialize the DB
// (currently just an INI text file)
//----------------------------------------------------------------------
void DataMng::initializeDB()
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
// Method: saveTaskToDB
// Save the information on generated output products to the archive
//----------------------------------------------------------------------
void DataMng::saveTaskToDB(MessageString & m, bool initialStore)
{
    // Ensure 0-ended char arrays
    if (m.at(m.size() - 1) < 32) { m[m.size() - 1] = 0; }

    Message<MsgBodyTSK> msg(m);
    MsgBodyTSK & body = msg.body;
    TaskInfo taskInfo(body["info"]);

    saveTaskToDB(taskInfo, initialStore);
}

//----------------------------------------------------------------------
// Method: saveTaskToDB
// Save the information on generated output products to the archive
//----------------------------------------------------------------------
void DataMng::saveTaskToDB(TaskInfo & taskInfo, bool initialStore)
{
    // Save task information in task_info table
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    TRC("TaskStatus = " + TaskStatusName[(TaskStatus)(taskInfo.taskStatus())]);
    
    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();

        // Try to store the task data into the DB
        if (initialStore) { dbHdl->storeTask(taskInfo); }
        else              { dbHdl->updateTask(taskInfo); }

    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();

    // In case the task has finished, save output products metadata
    if ((taskInfo.taskStatus() == TASK_FINISHED) || (taskInfo.taskStatus() == TASK_FAILED)) {

        DBG("TASK FINISHED : Storing outputs into local archive...");

        URLHandler urlh;

        // Check version of products in gateway against DB
        TraceMsg("Will try to sanitize product versions:");
        sanitizeProductVersions(taskInfo.outputs);

        // Try to process the QDT report and get the issues found
        for (auto & m : taskInfo.outputs.products) {
            if ((m.procTargetType() == UA_NOMINAL) && 
                (m.procFunc() == "QLA") && 
                (m.fileType() == "JSON")) {
                QDTReportHandler qdtRep(m.url().substr(7));
                if (!qdtRep.read()) continue;
                std::vector<Alert*> issues;
                qdtRep.getIssues(issues);
                for (auto & v : issues) { RaiseDiagAlert(*v); }
            }
        }
        
        // Move products to local archive or to final destination
        for (auto & m : taskInfo.outputs.products) {
            urlh.setProduct(m);
            if (m.procTargetType() == UA_NOMINAL) {
                m = urlh.fromGateway2LocalArch();
            } else {
                try {
                    m = urlh.fromGateway2FinalDestination();
                } catch(...) {
                    RaiseSysAlert(Alert(Alert::System,
                                        Alert::Warning,
                                        Alert::Resource,
                                        std::string(__FILE__ ":" Stringify(__LINE__)),
                                        "Cannot copy the output product to target " + m.procTarget(),
                                        0));            
                }
            }
        }

        int flags = taskInfo.taskFlags();
        if (flags & OpenIPython) {
            // Launch IPython session
            IPythonLauncher * ipy = new IPythonLauncher(cfg.connectivity.ipython.cmd(),
                                                        cfg.connectivity.ipython.path());
            ipy->exec();
        } else if (flags & OpenJupyterLab) {
            // TODO Analyze product with Jupyter Lab
        }
            
        InfoMsg("Saving outputs...");
        saveProductsToDB(taskInfo.outputs);
        
        InfoMsg("Sending message to register outputs at Orchestrator catalogue");
        
        //Config & cfg = Config::_();
        if (cfg.flags.sendOutputsToMainArchive()) {
            InfoMsg("Archiving/Registering data at DSS/EAS");
            //std::thread(&DataMng::archiveDSSnEAS, this, std::ref(taskInfo..outputs)).detach();
            archiveDSSnEAS(taskInfo.outputs);
        }
    }
}

//----------------------------------------------------------------------
// Method: sanitizeProductVersions
// Make sure that there is no product with the same signature (and version)
// in local archive, changing the version if needed
//----------------------------------------------------------------------
void DataMng::sanitizeProductVersions(ProductList & prodList)
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {

        // Check that connection with the DB is possible
        dbHdl->openConnection();

        // Check signature for each product
        std::string ver;
        FileNameSpec fs;

        for (auto & m : prodList.products) {
            std::string sgnt  = m.signature();
            std::string ptype = m.productType();
            m.dump();
            TraceMsg("Checking signature " + sgnt + 
                     ", product type " + ptype + " and version " + ver);
            if (dbHdl->checkSignature(sgnt, ptype, ver)) {
                // Version exists: change minor version number
                std::string origVer = m.productVersion();
                std::string newVer  = fs.incrMinorVersion(origVer);

                std::string url(m.url());
                std::string oldFile(str::mid(url,7,1000));

                std::string s("Found in database:" + sgnt + " [" + ver +
                              "], changing " + origVer + " with " + newVer);
                WarnMsg(s);
                CreateSysAlert(Log::WARNING, Alert::Warning, s);

                std::vector<std::string> svec {m.url(), m.baseName(), m.productId(), m.baseName(), sgnt };
                for (auto & s: svec) { str::replaceAll(s, origVer, newVer); }

                int i = 0;
                m["url"]            = svec.at(i++);
                m["baseName"]       = svec.at(i++);
                m["productId"]      = svec.at(i++);
                i++;
                m["signature"]      = svec.at(i++);

                m["productVersion"] = newVer;

                url = m.url();
                std::string newFile(str::mid(url,7,1000));
                if (rename(oldFile.c_str(), newFile.c_str()) != 0) {
                    WarnMsg("ERROR: Cannot rename file " + oldFile + " to " + newFile);
                }
            }
        }

    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        for (auto & v: prodList.products) { ErrMsg(v.str()); }
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: saveProductsToDB
// Save the information of a new (incoming) product to the DB
// (currently just an INI text file)
//----------------------------------------------------------------------
void DataMng::saveProductsToDB(ProductList & productList)
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        // Try to store the data into the DB
        dbHdl->storeProducts(productList);
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        for (auto & v: productList.products) { ErrMsg(v.str()); }
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: storeTaskStatusSpectra
// Store task agent spectra in DB
//----------------------------------------------------------------------
void DataMng::storeTaskStatusSpectra(json & fmkInfoValue)
{
    TskStatTable tssSet;
    
    for (Json::ValueIterator itr = fmkInfoValue["hostsInfo"].begin();
         itr != fmkInfoValue["hostsInfo"].end(); ++itr) {
        std::string key = itr.key().asString();
        json hInfo = *itr;
        for (Json::ValueIterator ittr = hInfo["agentsInfo"].begin();
             ittr != hInfo["agentsInfo"].end(); ++ittr) {
            std::string agNme = ittr.key().asString();
            json ag = (*ittr)["counts"];
            TskStatSpectra tss(ag["running"].asInt(),
                               ag["scheduled"].asInt(),
                               ag["paused"].asInt(),
                               ag["stopped"].asInt(),
                               ag["failed"].asInt(),
                               ag["finished"].asInt());
            tssSet.push_back(std::make_pair(agNme, tss));
        }
    }
    
    for (Json::ValueIterator itr = fmkInfoValue["swarmInfo"].begin();
         itr != fmkInfoValue["swarmInfo"].end(); ++itr) {
        std::string key = itr.key().asString();
        json sw = (*itr)["counts"];
        TskStatSpectra tss(sw["running"].asInt(),
                           sw["scheduled"].asInt(),
                           sw["paused"].asInt(),
                           sw["stopped"].asInt(),
                           sw["failed"].asInt(),
                           sw["finished"].asInt());
        tssSet.push_back(std::make_pair(sw["name"].asString(), tss));
    }
    
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        // Store task status spectra in DB
        for (auto & p : tssSet) {                   
            dbHdl->saveTaskStatusSpectra(p.first, p.second);
        }
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();
}

//----------------------------------------------------------------------
// Method: retrieveTaskStatusSpectra
// Retrieve task agent spectra from DB
//----------------------------------------------------------------------
void DataMng::retrieveTaskStatusSpectra(TskStatTable & tssSet)
{
    std::unique_ptr<DBHandler> dbHdl(new DBHdlPostgreSQL);

    std::vector< std::vector<std::string> > table;
    try {
        // Check that connection with the DB is possible
        dbHdl->openConnection();
        if (! dbHdl->getTable("task_status_spectra", table)) {
            RaiseSysAlert(Alert(Alert::System,
                                Alert::Warning,
                                Alert::Resource,
                                std::string(__FILE__ ":" Stringify(__LINE__)),
                                "Cannot read status spectra table from DB",
                                0));            
        }
    } catch (RuntimeException & e) {
        ErrMsg(e.what());
        return;
    }

    // Close connection
    dbHdl->closeConnection();

    // Place table results into task status spectra table
    tssSet.clear();
    for (auto & row : table) {
        TskStatSpectra tss(std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[3]),
                           std::stoi(row[4]), std::stoi(row[5]), std::stoi(row[6]));
        tssSet.push_back(std::make_pair(row[0], tss));
    }
}

//----------------------------------------------------------------------
// Method: archiveDSSnEAS
// Sends the information to the area where the corresponding daemon is
// looking for data to be sent to DSS/EAS
//----------------------------------------------------------------------
void DataMng::archiveDSSnEAS(ProductList & productList)
{
/*
    static bool firstTime = true;
    static std::string proxyUser;
    static std::string proxyHost;
    static std::string proxyDropbox;
    static std::string proxyUrl;

    if (firstTime) {
        Config & cfg = Config::_();
        proxyUser    = cfg.storage.archive.user;
        proxyHost    = cfg.storage.archive.address;
        proxyDropbox = cfg.storage.archive.path;
        proxyUrl     = proxyUser + "@" + proxyHost + ":" + proxyDropbox + "/";

        firstTime = false;
    }

    return;

    for (auto & kv : productList.productList) {
        ProductType & prodType = const_cast<ProductType&>(kv.first);
        ProductMetadata & md = kv.second;

        // Execute task driver
        pid_t pid = fork();
        switch (pid) {
        case -1:
            // Error, failed to fork()
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: {
            // We are the child
            TRC(md.url);
            std::string fileName(md.url.substr(7, md.url.length()-7));
            std::string subBox;
            if      (isLE1Product(prodType)) { subBox = "data/"; }
            else if (isLE1Metadata(prodType)) { subBox = "meta/"; }
            proxyUrl += subBox;

            // Hack to send small files instead of actual products
            std::string source(fileName);
            std::string target(proxyUrl);
//            if ((md.productType == "VIS_RAW") || (md.productType == "VIS_LE1")) {
//                source = "/tmp/dummy_zeros";
//                target = proxyUrl + md.productId + ".fits";
//            }
            char *procTaskCmdLine[] = { (char*)("/usr/bin/scp"),
                                        (char*)(source.c_str()),
                                        (char*)(target.c_str()), NULL };
            std::string cmdLine = "scp " + source + " " + target;
            DBG("Trying to execute: '" << cmdLine << "'");
            char *env[] = { (char*)("LD_LIBRARY_PATH="),
                            (char*)("PATH=/usr/bin:/bin:/opt/bin"),
                            NULL };
            execve(procTaskCmdLine[0], procTaskCmdLine, env);
            perror("scp");
            exit(EXIT_FAILURE);
        }
        default: {
            // We are the parent process
            int status;
            if (waitpid(pid, &status, 0) < 0 ) {
                perror("wait");
                exit(254);
            }
            if (WIFEXITED(status)) {
                status = WEXITSTATUS(status);
                if (status == 0) {
                    InfoMsg("Successful arrival at DSS/EAS proxy host");
                } else {
                    InfoMsg("Problems with arrival of data to DSS/EAS proxy host");
                }
            }
            if (WIFSIGNALED(status)) {
                printf("Process %d killed: signal %d%s\n",
                       pid, WTERMSIG(status),
                       WCOREDUMP(status) ? " - core dumped" : "");
                exit(1);
            }
        }
        }
    }
*/
}

//----------------------------------------------------------------------
// Method: isProductAvailable
// Returns TRUE if there is a product type like the requested in the DB
//----------------------------------------------------------------------
bool DataMng::isProductAvailable(std::string prodType)
{
    //TODO  // return (ini.exists("ProductsIdx", prodType));
    UNUSED(prodType);

    return true;
}

//----------------------------------------------------------------------
// Method: getProductLatest
// Returns TRUE if there is a product type like the requested in the DB
//----------------------------------------------------------------------
bool DataMng::getProductLatest(std::string prodType,
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
        if (prodList.products.size() > 0) {
            prodMetadata = prodList.products.at(0).val();
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



//----------------------------------------------------------------------
// Method: processInDataMsg
//----------------------------------------------------------------------
void DataMng::txInDataToLocalArch(ProductList & inData)
{
    // Transfer to local archive
    URLHandler urlh;
    for (auto & m : inData.products) {
        urlh.setProduct(m);
        m = urlh.fromInbox2LocalArch();
    }

    // Save to DB
    saveProductsToDB(inData);
}
