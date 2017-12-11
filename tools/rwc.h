/******************************************************************************
 * File:    rwc.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.RWC
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
 *   Declaration of RWC (Request With Curl) class
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
#ifndef RWC_H
#define RWC_H

#include <string>

#include <curl/curl.h>

//======================================================================
// Class: RWC
// RWC - Requests With Curl
// Simple wrapper for libCurl calls.
// For the time being, only the current methods are available:
// - getContent  - GET request, result as a string
// - get         - GET requers, result stored as local file
// - post        - POST request, posts a user specified file
// - postContent - POST request, posts the content as a file
//======================================================================
class RWC {

    //== Ancillary classes and structures
    struct PostData {
        const char *ptr;
        size_t size;
    };

    struct MemoryStruct {
        char *memory;
        size_t size;
    };

public:
    
    //------------------------------------------------------------
    // Constructor
    // Generate a temporary file with unique file name in /tmp
    //------------------------------------------------------------
    RWC();
    
    //------------------------------------------------------------
    // Method: getContent
    // Function to perform a GET request and obtain the result in
    // a string
    //------------------------------------------------------------
    void getContent(std::string url, std::string & content,
                    std::string & result);
    
    //------------------------------------------------------------
    // Method: get
    // Function to perform a GET request and obtain the result in
    // the form of an output file
    //------------------------------------------------------------
    void get(std::string url, std::string localFile,
             std::string & result);

    //------------------------------------------------------------
    // Mwthod: post_
    // Function to POST the content of a file
    //------------------------------------------------------------
    void post(std::string url, std::string localFile, std::string & result,
              std::string contentType = std::string("application/xml"));
    
    //------------------------------------------------------------
    // Mwthod: postContent
    // Function to POST a string
    //------------------------------------------------------------
    void postContent(std::string url, std::string & content,
                     std::string & result);

    //------------------------------------------------------------
    // Method: setAuth
    // Set authorisation parameters
    //------------------------------------------------------------
    void setAuth(std::string & u, std::string & p,
                 long aut = (long)CURLAUTH_BASIC);
    
 private:

    size_t (*readFunc)(void *ptr, size_t size,
                       size_t nmemb, void *userp);
    size_t (*fwriteFunc)(void *ptr, size_t size,
                         size_t nmemb, void *userp);
    size_t (*mwriteFunc)(void *ptr, size_t size,
                         size_t nmemb, void *userp);

    std::string user;
    std::string pwd;
    long authType;
    bool isAuthSet;

};

#endif // RWC_H
