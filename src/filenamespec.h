/******************************************************************************
 * File:    filenamespec.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.FileNameSpec
 *
 * Version:  1.1
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
 *   Declare FileNameSpec class
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

#ifndef FILENAMESPEC_H
#define FILENAMESPEC_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//  - cstdio
//------------------------------------------------------------
//#include <cstdio>
#include <string>
#include <set>
#include <vector>
#include <map>

#define USE_CX11_REGEX
#ifdef USE_CX11_REGEX
#  include <regex>

#include "cfginfo.h"
#else
#  include "PCRegEx.h"
#endif

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
// Class: FileNameSpec
//==========================================================================
class FileNameSpec {

public:
    FileNameSpec();
    FileNameSpec(std::string regexp, std::string assign);

    struct FileNameComponents {
        std::string dirName;
        std::string baseName;
        std::string suffix;
        std::string extension;
        std::string mission;      // %M
        std::string signature;    // %S
        std::string instrument;   // %I
        std::string productType;  // %T
        std::string productId;
        std::string version;      // %v
        std::string dateRange;    // %D
        std::string dateStart;    // %f
        std::string dateEnd;      // %t
#ifdef DEBUG_BUILD
        void dump(std::ostream & os) {
            os << "dirName     : " << this->dirName << std::endl;
            os << "baseName    : " << this->baseName << std::endl;
            os << "suffix      : " << this->suffix << std::endl;
            os << "extension   : " << this->extension << std::endl;
            os << "id          : " << this->productId << std::endl;
            os << "mission     : " << this->mission << std::endl;
            os << "productType : " << this->productType << std::endl;
            os << "signature   : " << this->signature << std::endl;
            os << "instrument  : " << this->instrument << std::endl;
            os << "version     : " << this->version << std::endl;
            os << "dateRange   : " << this->dateRange << std::endl;
            os << "dateStart   : " << this->dateStart << std::endl;
            os << "dateEnd     : " << this->dateEnd << std::endl;
        }
#endif
    };

    void setFileNameSpec(std::string regexp, std::string assign);
    void setRegEx(std::string regexp);
    void setAssignations(std::string assign);
    void setProductIdTpl(std::string tpl);

    bool parseFileName(std::string fileName, 
                       ProductMetadata & m,
                       std::string space = QPF::InboxSpace,
                       std::string creator = "EUCLID");
    FileNameComponents parseFileName(std::string fileName);

    std::string buildProductId(FileNameComponents c);
    std::string buildProductId(std::string mission = "EUC",
                               std::string sDate = std::string("20010101T000000.0Z"),
                               std::string eDate = std::string(""),
                               std::string prodType = "LE1",
                               std::string sign = "",
                               std::string ver = "01.00");

private:

#ifdef USE_CX11_REGEX
    static std::regex                        re;
#else
    static PCRegEx *                         re;
#endif

    static std::string                       reStr;
    static std::string                       assignationsStr;
    static std::map< char, std::set<int> >   assignations;
    static std::string                       productIdTpl;

    static bool                              initialized;
};

}

#endif  /* FILENAMESPEC_H */
