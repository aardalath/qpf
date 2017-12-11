/******************************************************************************
 * File:    voshdl.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.VOSHdl
 *
 * Version:  1.0
 *
 * Date:    2017/12/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Provides object implementation for some declarations
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

#include "voshdl.h"

#include "filetools.h"
#include "scopeexit.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace FileTools;

//------------------------------------------------------------
// Constructor
//------------------------------------------------------------
VOSpaceHandler::VOSpaceHandler(std::string url) :
    vOSpaceUrl(url)
{
}

//------------------------------------------------------------
// Destructor
//------------------------------------------------------------
VOSpaceHandler::~VOSpaceHandler()
{
}

//------------------------------------------------------------
// Method: setVOSpaceUrl
// Sets VOSpace service URL
//------------------------------------------------------------
void VOSpaceHandler::setVOSpaceUrl(std::string url) 
{
    vOSpaceUrl = url;
}

//------------------------------------------------------------
// Method: setAuth
// Sets credentials to be used with VOSpace
//------------------------------------------------------------
void VOSpaceHandler::setAuth(std::string & u, std::string & p) 
{
    user = u;
    pwd  = p;
}

//------------------------------------------------------------
// Method: setAuth
// Creates a new file in VOSpace folder with specified content
//------------------------------------------------------------
bool VOSpaceHandler::uploadToFile(std::string folder, std::string file,
                                  std::string content,
                                  std::string contentType)
{
    // Store temporarily in file
    std::ofstream fHdl(file);
    ScopeExit fHdl_close([&file] { unlink(file.c_str()); });
    fHdl << content;
    fHdl.close();

    // Upload file
    return uploadFile(folder, file, contentType);
}

//------------------------------------------------------------
// Method: uploadFile
// Uploads local file to VOSpace folder
//------------------------------------------------------------
bool VOSpaceHandler::uploadFile(std::string folder, std::string file,
                                std::string contentType)
{
    std::string endPoint = vOSpaceUrl + "/service/data/";
    std::string jobId;
    
    RWC rwc;
    rwc.setAuth(user, pwd);
    
    if (! uploadJobRequest(rwc, folder, "pushToVoSpace", jobId)) {
        return false;
    }

    std::string uploadUrl = endPoint + user + "/" + jobId;
    std::string result;
    
    rwc.post(uploadUrl, file, result, contentType);

    std::cerr << "Result: [" << result << "]\n";

    return (result.find("200 OK") != std::string::npos);
}

//------------------------------------------------------------
// Method: downloadFromFile
// Downloads content of file in VOSpace folder to string
//------------------------------------------------------------
bool VOSpaceHandler::downloadFromFile(std::string folder, std::string file,
                                      std::string & content)
{
    std::string tmpFile = generateTmpFile();
    downloadFile(folder, file, tmpFile);
    storeFileIntoString(tmpFile, content);
    unlink(tmpFile.c_str());
}

//------------------------------------------------------------
// Method: downloadFile
// Download file in VOSpace folder as localfile
//------------------------------------------------------------
bool VOSpaceHandler::downloadFile(std::string folder, std::string file,
                                  std::string localFile)
{
    std::string endPoint = vOSpaceUrl + "/service/data/";
    std::string jobId;
    
    RWC rwc;
    rwc.setAuth(user, pwd);
    
    if (! uploadJobRequest(rwc, folder + (file.empty() ? "" : "/" + file),
                           "pullFromVoSpace", jobId)) {
        return false;
    }

    std::string downloadUrl = endPoint + user + "/" + jobId;
    std::string result;
    rwc.get(downloadUrl, localFile, result);

    std::cerr << "Result: [" << result << "]\n";

    return (result.find("200 OK") != std::string::npos);
}

//------------------------------------------------------------
// Method: uploadJobRequest
// Uploads a job request in the form of XML, and launches it
//------------------------------------------------------------
bool VOSpaceHandler::uploadJobRequest(RWC & rwc, std::string folder,
                                      std::string rqstType, std::string & jobId)
{
    std::string txUrl    = vOSpaceUrl + "/servlet/transfers/async?PHASE=RUN";
    std::string endPoint = vOSpaceUrl + "/service/data/";
        
    char a[2048];
    sprintf(a, Tx_XML_File.c_str(), user.c_str(), folder.c_str(), rqstType.c_str());
    std::string txData(a);
    
    std::string result;
    rwc.postContent(txUrl, txData, result);
    std::cerr << "Result: [" << result << "]\n";
    
    // Get redirection URL
    std::string redirection = result.substr(result.find("Location: ") + 10);
    redirection.resize(redirection.find('\n') - 1);

    std::cerr << redirection << '\n';

    std::string phase("");
    do {
        std::string content;
        rwc.getContent(redirection, content, result);
        std::cerr << content << '\n' << result << '\n';
        
        size_t posFrom = content.find("<uws:phase>") + 11;
        size_t posTo   = content.find("</uws:phase>");
        phase = content.substr(posFrom, posTo - posFrom);
        std::cerr << "PHASE: " << phase << '\n';
    } while ((phase != "COMPLETED") && (phase != "ERROR"));

    if (phase != "COMPLETED") { return false; }
    
    jobId = redirection.substr(redirection.rfind('/') + 1);
    return true;
}

// Template for pushTo/pullFromVoSpace request file
const std::string VOSpaceHandler::Tx_XML_File =
    "<vos:transfer xmlns:vos=\"http://www.ivoa.net/xml/VOSpace/v2.0\">\n"
    "    <vos:target>vos://esavo!vospace/%s/%s</vos:target>\n"
    "    <vos:direction>%s</vos:direction>\n"
    "    <vos:view uri=\"vos://esavo!vospace/core#fits\"/>\n"
    "    <vos:protocol uri=\"vos://esavo!vospace/core#httpput\"/>\n"
    "</vos:transfer>\n";

