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
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016 Euclid SOC Team @ ESAC
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

#include "filetransfer.h"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/easy.h>

#include "dbg.h"
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
FileTransfer::FileTransfer() : retrievalMode(RETRIEVAL_AS_FILE)
{
}

bool FileTransfer::download(std::string url, std::string localFileName)
{
    bool result = false;
    CURLcode res;
    CURL * curl = curl_easy_init();
    if (curl) {
        if (retrievalMode == FileTransfer::RETRIEVAL_AS_FILE) {
            fileStream.open(localFileName.c_str(), std::ofstream::out | std::ofstream::binary);
        } else if (retrievalMode == FileTransfer::RETRIEVAL_AS_STRING) {
            strStream.str("");
        } else {
            return result;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileTransfer::writeData);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            perror("curl_easy_perform");
        }
        curl_easy_cleanup(curl);

        if (retrievalMode == FileTransfer::RETRIEVAL_AS_FILE) {
            fileStream.close();
        }

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

std::string FileTransfer::str()
{
    return strStream.str();
}

FileTransfer::Mode FileTransfer::mode()
{
    return retrievalMode;
}

void FileTransfer::setMode(FileTransfer::Mode m)
{
    retrievalMode = m;
}

std::streampos FileTransfer::writeData(char * data, size_t size, size_t nmemb, void * obj)
{
    return static_cast<FileTransfer*>(obj)->writeDataImpl(data, size, nmemb);
}

std::streampos FileTransfer::writeDataImpl(char * data, size_t size, size_t nmemb)
{
    std::streampos written;
    std::streampos pos0;

    if (retrievalMode == FileTransfer::RETRIEVAL_AS_FILE) {
        pos0 = fileStream.tellp();
        fileStream.write(data, size * nmemb);
        written = fileStream.tellp() - pos0;
    } else if (retrievalMode == FileTransfer::RETRIEVAL_AS_STRING) {
        pos0 = strStream.tellp();
        strStream.write(data, size * nmemb);
        written = strStream.tellp() - pos0;
    } else {
        written = 0;
    }
    return written;
}

}
