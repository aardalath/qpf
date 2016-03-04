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
#include "str.h"
#include <libgen.h>
#include <cstring>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

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
    initialized = true;
}

void FileNameSpec::setRegEx(std::string regexp)
{
    reStr = regexp;
    re = std::regex(regexp);
}

void FileNameSpec::setAssignations(std::string assign)
{
    // Split assignations string
    // Assignation string is in the form:
    // %x=n:m;%y=l;...
    // where x, y, ... is one of:
    //   %M :  mission
    //   %S :  signature
    //   %I :  instrument
    //   %T :  productType
    //   %v :  version
    //   %D :  dateRange
    //   %f :  dateStart (from)
    //   %t :  dateEnd   (to)
    // and n, m, l is the number of the sub-expression in the regular expression
    // pattern, n+m means concatenation of subexpressions n and m

    assignationsStr = assign;
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

void FileNameSpec::setProductIdTpl(std::string tpl)
{
    productIdTpl = tpl;
}

FileNameSpec::FileNameComponents FileNameSpec::parseFileName(std::string fileName)
{
    if (! initialized) { setFileNameSpec(reStr, assignationsStr); }

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
    str::split(baseName, '.', baseExt);

    baseName = baseExt.at(0);
    baseExt.erase(baseExt.begin());

    c.dirName   = dirName;
    c.baseName  = baseName;
    c.mission   = baseName.substr(0, 3);
    c.suffix    = str::join(baseExt, ".");
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

    c.productId = buildProductId(c);

    return c;
}

std::string FileNameSpec::buildProductId(FileNameSpec::FileNameComponents c)
{
    return buildProductId(c.mission,
                          c.dateStart, c.dateEnd,
                          c.productType, c.signature,
                          c.version);
}

std::string FileNameSpec::buildProductId(std::string mission,
                                         std::string sDate, std::string eDate,
                                         std::string prodType, std::string sign,
                                         std::string ver)
{
    std::string id(productIdTpl);

    //   %M :  mission
    //   %S :  signature
    //   %I :  instrument
    //   %T :  productType
    //   %v :  version
    //   %D :  dateRange
    //   %f :  dateStart (from)
    //   %t :  dateEnd   (to)

    str::replaceAll(id, "%M", mission);
    str::replaceAll(id, "%T", prodType);
    str::replaceAll(id, "%S", sign);
    str::replaceAll(id, "%f", sDate);
    str::replaceAll(id, "%t", eDate);
    str::replaceAll(id, "%v", ver);

    return id;
}

std::string                       FileNameSpec::reStr = "EUC_([A-Z]+)(_[A-Z0-9_]+)_([^_]+)_([0-9T]+\.[0-9]Z)[_]*([0-9\.]*)";
std::regex                        FileNameSpec::re;
std::string                       FileNameSpec::assignationsStr = "%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5";
std::map< char, std::set<int> >   FileNameSpec::assignations;
std::string                       FileNameSpec::productIdTpl = "%M_%T_%S_%f_%v";

bool                              FileNameSpec::initialized = false;

}
