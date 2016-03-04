/******************************************************************************
 * File:    str.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.str
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
 *   Implement str class, wrapper around libstr functions
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

#include "str.h"

#include <iterator>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Namespace: str
// Wrapper around string related functions
//==========================================================================
namespace str {

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

//----------------------------------------------------------------------
// Function: replaceAll
// Replace a with b within s
//----------------------------------------------------------------------
std::string replaceAll(std::string & s, std::string a, std::string b)
{
    size_t idx = s.find(a, 0);
    while (idx != std::string::npos) {
        s.replace(idx, a.length(), b);
        idx = s.find(a, idx + b.length());
    }
    return s;
}

}

}
