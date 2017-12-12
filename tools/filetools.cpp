/******************************************************************************
 * File:    filetools.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.FileTools
 *
 * Version:  1.2
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

#include <iostream>
#include <fstream>

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

}
