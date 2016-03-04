/******************************************************************************
 * File:    filetransfer.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.FileTransfer
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
 *   Implement FileTransfer class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   libCurl
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

#include "filetransfer.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/easy.h>

//#include "dbg.h"
#include <functional>
#include <cerrno>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

using namespace std::placeholders;

//----------------------------------------------------------------------
// Method: Constructor
//----------------------------------------------------------------------
FileTransfer::FileTransfer()
{
}

bool FileTransfer::download(std::string url, std::string localFileName)
{
    bool result = false;
    CURLcode res;
    CURL * curl = curl_easy_init();
    if (curl) {
        fileStream = fopen(localFileName.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileTransfer::writeData);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) { perror("curl_easy_perform"); }
        curl_easy_cleanup(curl);
        fclose(fileStream);
        result = true;
    }
    return result;
}

bool FileTransfer::download(std::string protocol,
                            std::string user, std::string passwd,
                            std::string addr, int port,
                            std::string remoteFileName,
                            std::string localFileName)
{
    char sPortNum[10];
    sprintf(sPortNum, ":%d", port);
    std::string credentials = ((user.empty() ? "" : user) +
                               (passwd.empty() ? "" : ":" + passwd));
    std::string url = (protocol + "://" +
                       (credentials.empty() ? "" : credentials + "@") +
                       addr + (port != 0 ? sPortNum : "") +
                       remoteFileName);
    std::cerr << url << std::endl;
    return this->download(url, localFileName);
}

size_t FileTransfer::writeData(char * data, size_t size, size_t nmemb, void * obj)
{
    return static_cast<FileTransfer*>(obj)->writeDataImpl(data, size, nmemb);
}

size_t FileTransfer::writeDataImpl(char * data, size_t size, size_t nmemb)
{
    size_t written;
    written = fwrite((void*)(data), size, nmemb, fileStream);
    return written;
}

}
