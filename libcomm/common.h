/******************************************************************************
 * File:    common.h
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.common
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
 *   Common definitions and macros
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
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   + STL
//     - iostream
//     - string
//     - vector
//     - map
//   + Other
//     - cassert
//     - sys/stat.h
//     - sys/types.h
//     - unistd.h
//------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - error.h
//------------------------------------------------------------

#include "error.h"

//============================================================
// Group: Public Declarations
//============================================================

//------------------------------------------------------------
// Topic: Constants
//   - CRC32_INITIAL_VALUE - Initial value for CRC32 computation
//------------------------------------------------------------

#define CRC32_INITIAL_VALUE  0x7fffffff

//------------------------------------------------------------
// Topic: Macros
//   - MSGASSERT(condition, message) - Extended assert macro
//                                     with custom message
//------------------------------------------------------------

#define MSGASSERT(condition, message) \
    do { if (! (condition)) { \
        std::cerr << "Assertion '" #condition "' failed in " \
                 << __FILE__ << ":" << __LINE__ << ": " \
                 << message << std::endl; \
        abort(); \
    } } while (false);

#endif // COMMON_H
