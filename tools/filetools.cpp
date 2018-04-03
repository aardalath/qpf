/******************************************************************************
 * File:    filetools.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.FileTools
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
 *   Provides object implementation for FileTools namespace functions
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

#include "filetools.h"

#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include <iostream>
#include <fstream>

#include "dbg.h"

//======================================================================
// Namespace: FileTools
// File releated utility functions
//======================================================================
namespace FileTools {
    
//------------------------------------------------------------
// Method: generateTmpFile
// Generate a temporary file with unique file name in /tmp
//------------------------------------------------------------
std::string generateTmpFile()
{
    // Prepare temporary file
    char buff[L_tmpnam];
    tmpnam(buff);
    return std::string(buff);
}
    
//------------------------------------------------------------
// Method: storeFileIntoString
// Handy short function to store the entire content of a file
// into a string
//------------------------------------------------------------
void storeFileIntoString(std::string & iFile, std::string & s)
{
    FILE* f = fopen(iFile.c_str(), "rb");

    // Determine file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);
        
    char * where = new char[size];        
    fread(where, sizeof(char), size, f);
        
    s.assign((const char*)(where), size);
        
    delete[] where;
}

//----------------------------------------------------------------------
// Method: copyfile
//----------------------------------------------------------------------
int copyfile(std::string & sFrom, std::string & sTo)
{
    int source = open(sFrom.c_str(), O_RDONLY, 0);
    int dest = open(sTo.c_str(), O_WRONLY | O_CREAT, 0644);

    // struct required, rationale: function stat() exists also
    struct stat stat_source;
    fstat(source, &stat_source);

    TRC("Local copying: " + sFrom + " => " + sTo);
    sendfile(dest, source, 0, stat_source.st_size);

    close(source);
    close(dest);

    return 0;
}

//----------------------------------------------------------------------
// Method: rcopyfile
//----------------------------------------------------------------------
int rcopyfile(std::string & sFrom, std::string & sTo,
              std::string & remoteHost, bool toRemote)
{
    static std::string scp("/usr/bin/scp");
    std::string cmd;
    if (toRemote) {
        cmd = scp + " " + remoteHost + ":" + sFrom + " " + sTo;
    } else {
        cmd = scp + " " + sFrom + " " + remoteHost + ":" + sTo;
    }
    int res = system(cmd.c_str());
    TRC("Remote copying: " + cmd);
    (void)(res);

    return 0;
}

//----------------------------------------------------------------------
// Method: runlink
//----------------------------------------------------------------------
    int runlink(std::string & f, std::string & remoteHost)
{
    std::string cmd;
    cmd = "ssh " + remoteHost + " rm " + f;
    int res = system(cmd.c_str());
    TRC("Remote unlinking: " + cmd);
    (void)(res);

    return 0;
}

}
