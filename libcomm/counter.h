/******************************************************************************
 * File:    counter.h
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Counter
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
 *   Define Counter main class
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
 *   See <ChangeCounter>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef COUNTER_H
#define COUNTER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - vector
//   - string
//------------------------------------------------------------
#include <vector>
#include <string>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------

//============================================================
// Group: Public Declarations
//============================================================

//------------------------------------------------------------
// Topic: Macros
//------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//==========================================================================
// Class: Counter
//==========================================================================
class Counter {

public:
    Counter(unsigned int s = 4, unsigned long initialValue = 0);

    void reset();

    void incr(unsigned int n = 1);
    void decr(unsigned int n = 1);

    void encode();
    void decode();

    unsigned long getValue();
    void setValue(unsigned long v);
    std::string getString();
    void setString(std::string s);

private:
    unsigned long toValue();
    void toFields();

private:
    unsigned int size;
    std::vector<unsigned char> fields;
    unsigned long value;
    std::string valueStr;

    unsigned long barrier;
    bool asComponents;

    static const unsigned char BASE64;
    static const unsigned char ENCODING_OFFSET;
};

}

#endif  /* COUNTER_H */
