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
#include <cassert>

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
    if (re == 0) { std::cerr << "PROBLEM!!" << std::endl; }
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

#ifdef USE_CX11_REGEX
    re = std::regex(regexp);
#else
    re = new PCRegEx(regexp);
    assert(re != 0);
#endif
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
#ifdef USE_CX11_REGEX
    std::cmatch m;
    std::regex_match(baseName.c_str(), m, re);
#else
    std::vector<std::string> m;
    if (re->match(baseName)) { re->get(m); }
#endif

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
#ifdef USE_CX11_REGEX
    unsigned int count = m.size();
#else
    // PCRE2 returns in m[0] the whole matched string
    // substrings start at m[1]
    unsigned int count = m.size() - 1;
#endif

    for (unsigned int i = 0; i < count; ++i) {
        // Extract the matches of the regex
#ifdef USE_CX11_REGEX
        int k = i;
#else
        // PCRE2 returns in m[0] the whole matched string
        // substrings start at m[1]
        int k = i + 1;
#endif

        for (auto & kv : assignations) {
            // indices in assignation string start with 1
            unsigned int idx = i + 1;
            std::string fld("ZZZ");
            if (idx <= count + 1) { fld = m[k]; }
            if (kv.second.find(idx) != kv.second.end()) {
                switch (kv.first) {
                case 'S':
                    c.signature   += fld;
                    break;
                case 'I':
                    c.instrument  += fld;
                    break;
                case 'T':
                    c.productType += fld;
                    break;
                case 'v':
                    if (fld.length() < 5) { fld = "01.00"; }
                    c.version     += fld;
                    break;
                case 'D':
                    c.dateRange   += fld;
                    break;
                case 'f':
                    c.dateStart   += fld;
                    break;
                case 't':
                    c.dateEnd     += fld;
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

#ifdef USE_CX11_REGEX
std::regex                        FileNameSpec::re;
#else
PCRegEx *                         FileNameSpec::re = 0;
#endif

std::string                       FileNameSpec::reStr = "EUC_([A-Z]+)(_[A-Z0-9_]+)_([^_]+)_([0-9T]+[\\.0-9]*Z)[_]*([0-9\\.]*)";
std::string                       FileNameSpec::assignationsStr = "%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5";
std::map< char, std::set<int> >   FileNameSpec::assignations;
std::string                       FileNameSpec::productIdTpl = "%M_%T_%S_%f_%v";

bool                              FileNameSpec::initialized = false;

}
