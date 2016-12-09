/******************************************************************************
 * File:    counter.cpp
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
 *   Implement Counter class
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

#include "counter.h"

#include <cassert>

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//----------------------------------------------------------------------
// Constructor: Counter
// This constructor is made protected to make a singleton
//----------------------------------------------------------------------
Counter::Counter(unsigned int s, unsigned long initialValue)
    : size(s), value(initialValue)
{
    fields.resize(s);
    reset();
    asComponents = false;
    barrier = BASE64 << (6 * (size - 1));
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::reset()
{
    value = 0;
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::incr(unsigned int n)
{
    value += n;
    if (value >= barrier) { value = value - barrier; }
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::decr(unsigned int n)
{
    if (value < n) { value = barrier - (n - value); }
    else           { value -= n; }
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::encode()
{
    valueStr.assign(size, ' ');
    unsigned long r = value;
    for (unsigned int i = 0; (i < size) && (r > 0); ++i) {
        fields[i] = r % BASE64;
        valueStr[i] = (unsigned char)(fields[i] + ENCODING_OFFSET);
        r = (r - fields[i]) / BASE64;
    }
    assert(r == 0);
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::decode()
{
    value = 0;
    unsigned long b = 1;
    for (unsigned int i = 0; i < size; ++i) {
        fields[i] = valueStr[i] - ENCODING_OFFSET;
        value += fields[i] * b;
        b *= BASE64;
    }
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
unsigned long Counter::getValue()
{
    return value;
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::setValue(unsigned long v)
{
    value = v;
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
std::string Counter::getString()
{
    encode();
    return valueStr;
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::setString(std::string s)
{
    valueStr = s;
    decode();
}

//----------------------------------------------------------------------
// Method: counter
// Put message into counter stream
//----------------------------------------------------------------------
void Counter::toFields()
{
}

const unsigned char Counter::BASE64 = 64;
const unsigned char Counter::ENCODING_OFFSET = 32;
}
