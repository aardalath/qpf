/******************************************************************************
 * File:    voshdl.h
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
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of several datavoshdl for QPF HMI
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
#ifndef VOSPACEHANDLER_H
#define VOSPACEHANDLER_H

#include <string>

#include "rwc.h"

//======================================================================
// Class: VOSpaceHandler
// Methods for the upload/download of files to/from VOSpace account
//======================================================================
class VOSpaceHandler {

    static const std::string Tx_XML_File;
    
public:
    //------------------------------------------------------------
    // Constructor
    //------------------------------------------------------------
    explicit VOSpaceHandler(std::string url = std::string());

    //------------------------------------------------------------
    // Destructor
    //------------------------------------------------------------
    ~VOSpaceHandler();

    //------------------------------------------------------------
    // Method: setVOSpaceUrl
    // Sets VOSpace service URL
    //------------------------------------------------------------
    void setVOSpaceUrl(std::string url);

    //------------------------------------------------------------
    // Method: setAuth
    // Sets credentials to be used with VOSpace
    //------------------------------------------------------------
    void setAuth(std::string & u, std::string & p);

    //------------------------------------------------------------
    // Method: setAuth
    // Creates a new file in VOSpace folder with specified content
    //------------------------------------------------------------
    bool uploadToFile(std::string folder, std::string file,
                      std::string content,
                      std::string contentType = std::string("application/octet-stream"));

    //------------------------------------------------------------
    // Method: uploadFile
    // Uploads local file to VOSpace folder
    //------------------------------------------------------------
    bool uploadFile(std::string folder, std::string file,
                    std::string contentType = std::string("application/octet-stream"));

    //------------------------------------------------------------
    // Method: downloadFromFile
    // Downloads content of file in VOSpace folder to string
    //------------------------------------------------------------
    bool downloadFromFile(std::string folder, std::string file,
                          std::string & content);

    //------------------------------------------------------------
    // Method: downloadFile
    // Download file in VOSpace folder as localfile
    //------------------------------------------------------------
    bool downloadFile(std::string folder, std::string file,
                      std::string localFile);

protected:
    //------------------------------------------------------------
    // Method: uploadJobRequest
    // Uploads a job request in the form of XML, and launches it
    //------------------------------------------------------------
    bool uploadJobRequest(RWC & rwc, std::string folder,
                          std::string rqstType, std::string & jobId);

private:
    std::string vOSpaceUrl;
    std::string user;
    std::string pwd;
};

#endif // VOSPACEHANDLER_H
