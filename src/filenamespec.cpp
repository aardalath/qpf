/******************************************************************************
 * File:    filenamespec.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.FileNameSpec
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
 *   Implement FileNameSpec class
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

#include "filenamespec.h"

#include "dbg.h"
#include <libgen.h>

//----------------------------------------------------------------------
// Function: split
// Splits a string into tokens separated by a delimites, and stores
// them in a vector of strings
//----------------------------------------------------------------------
std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

//----------------------------------------------------------------------
// Function: split
// Splits a string into tokens separated by a delimites, and stores
// them in a vector of strings
//----------------------------------------------------------------------
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//----------------------------------------------------------------------
// Function: join
// Joins a series of strings in a vector into a single string, with
// a separator in between two strings
//----------------------------------------------------------------------
std::string join(const std::vector<std::string>& vec, const char* sep)
{
    switch (vec.size()) {
    case 0:
        return std::string("");
        break;
    case 1:
        return vec.at(0);
        break;
    default: {
        std::stringstream res;

        std::copy(vec.begin(), vec.end() - 1,
                  std::ostream_iterator<std::string>(res, sep));
        res << *vec.rbegin();
        return res.str();
    } break;
    }
}

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
//namespace QPF {

//----------------------------------------------------------------------
// Method: Constructor
//----------------------------------------------------------------------
FileNameSpec::FileNameSpec()
{
}

FileNameSpec::FileNameSpec(std::string regexp, std::string assign)
{
    setFileNameSpec(regexp, assign);
}

void FileNameSpec::setFileNameSpec(std::string regexp, std::string assign)
{
    setRegEx(regexp);
    setAssignations(assign);
}

void FileNameSpec::setRegEx(std::string regexp)
{
    re = std::regex(regexp);
}

void FileNameSpec::setAssignations(std::string assign)
{
    // Split assignations string
    // Assignation string is in the form:
    // %x=n:m;%y=l;...
    // where x, y, ... is one of:
    //   %S :  signature
    //   %I :  instrument
    //   %T :  productType
    //   %v :  version
    //   %D :  dateRange
    //   %f :  dateStart (from)
    //   %t :  dateEnd   (to)
    // and n, m, l is the number of the sub-expression in the regular expression
    // pattern, n+m means concatenation of subexpressions n and m

    std::stringstream ss(assign);
    std::string a;
    int n;
    std::set<int> sexp;
    while (std::getline(ss, a, ';')) {
        if ((a.at(0) == '%') && (a.at(2) == '=')) {
            sexp.clear();
            std::stringstream sss(a.substr(3));
            std::string numStr;
            while (std::getline(sss, numStr, '+')) {
                std::stringstream ssss(numStr);
                ssss >> n;
                sexp.insert(n);
            }
            assignations[a.at(1)] = sexp;
        }
    }
}

FileNameSpec::FileNameComponents FileNameSpec::parseFileName(std::string fileName)
{
    FileNameComponents c;

    // First, get path name out of the name
    char *dirc, *basec, *bname, *dname;

    dirc = strdup(fileName.c_str());
    basec = strdup(fileName.c_str());
    dname = dirname(dirc);
    bname = basename(basec);

    std::string dirName(dname);
    std::string baseName(bname);

    // Extract the matches of the regex
    std::cmatch m;
    std::regex_match(baseName.c_str(), m, re);

    std::vector<std::string> baseExt;
    split(baseName, '.', baseExt);

    baseName = baseExt.at(0);
    baseExt.erase(baseExt.begin());

    c.dirName   = dirName;
    c.baseName  = baseName;
    c.suffix    = join(baseExt, ".");
    c.extension = baseExt.at(baseExt.size() - 1);

    // Extract fields according to assignations
    for (unsigned int i = 0; i < m.size(); ++i) {
        for (auto & kv : assignations) {
            if (kv.second.find(i) != kv.second.end()) {
                switch (kv.first) {
                case 'S':
                    c.signature   += m[i];
                    break;
                case 'I':
                    c.instrument  += m[i];
                    break;
                case 'T':
                    c.productType += m[i];
                    break;
                case 'v':
                    c.version     += m[i];
                    break;
                case 'D':
                    c.dateRange   += m[i];
                    break;
                case 'f':
                    c.dateStart   += m[i];
                    break;
                case 't':
                    c.dateEnd     += m[i];
                    break;
                default:
                    break;
                }
            }
        }
    }

    return c;
}

//}
