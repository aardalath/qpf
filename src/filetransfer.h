/******************************************************************************
 * File:    urlhdl.h
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
 *   Declare FileTransfer class
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

#ifndef FILETRANSFER_H
#define FILETRANSFER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//  - cstdio
//------------------------------------------------------------
//#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

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
    enum Mode {
        RETRIEVAL_AS_FILE,
        RETRIEVAL_AS_STRING,
    };

    bool download(std::string url, std::string localFileName);
    bool download(std::string protocol,
                  std::string user, std::string passwd,
                  std::string addr, int port,
                  std::string remoteFileName, std::string localFileName);
    std::string str();

    Mode mode();
    void setMode(Mode m);

private:
    static std::streampos writeData(char * data, size_t size, size_t nmemb, void * obj);
    std::streampos writeDataImpl(char * data, size_t size, size_t nmemb);

private:
    std::ofstream       fileStream;
    std::stringstream   strStream;
    Mode                retrievalMode;
};

}

#endif  /* FILETRANSFER_H */
