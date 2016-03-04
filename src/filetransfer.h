/******************************************************************************
 * File:    filetransfer.h
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
 *   Declare FileTransfer class
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

#ifndef FILETRANSFER_H
#define FILETRANSFER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//  - cstdio
//------------------------------------------------------------
#include <cstdio>
#include <string>

//------------------------------------------------------------
// Topic: External packages
//  none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//  none
//------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: FileTransfer
//==========================================================================
class FileTransfer {

public:
    FileTransfer();

public:
    bool download(std::string url, std::string localFileName);
    bool download(std::string protocol,
                  std::string user, std::string passwd,
                  std::string addr, int port,
                  std::string remoteFileName, std::string localFileName);
private:
    static size_t writeData(char * data, size_t size, size_t nmemb, void * obj);
    size_t writeDataImpl(char * data, size_t size, size_t nmemb);

    FILE *fileStream;

};

}

#endif  /* FILETRANSFER_H */
