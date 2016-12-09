/******************************************************************************
 * File:    common.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.common
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
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

//------------------------------------------------------------
// Topic: System headers
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
// Topic: Project headers
//------------------------------------------------------------

#include "error.h"

//------------------------------------------------------------
// Topic: Declarations
//------------------------------------------------------------

#define CRC32_INITIAL_VALUE  0x7fffffff

#define MSGASSERT(condition, message) \
    do { if (! (condition)) { \
        std::cerr << "Assertion '" #condition "' failed in " \
                 << __FILE__ << ":" << __LINE__ << ": " \
                 << message << std::endl; \
        abort(); \
    } } while (false);

#endif // COMMON_H
