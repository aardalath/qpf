/******************************************************************************
 * File:    filetools.h
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
 *   Declaration of FileTools namespace utility functions
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
#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <string>

//======================================================================
// Namespace: FileTools
// File releated utility functions
//======================================================================
namespace FileTools {

    //------------------------------------------------------------
    // Mwthod: generateTmpFile
    // Generate a temporary file with unique file name in /tmp
    //------------------------------------------------------------
    std::string generateTmpFile();
    
    //------------------------------------------------------------
    // Mwthod: storeFileIntoString
    // Handy short function to store the entire content of a file
    // into a string
    //------------------------------------------------------------
    void storeFileIntoString(std::string & iFile, std::string & s);

}

#endif // FILETOOLS_H
