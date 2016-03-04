/******************************************************************************
 * File:    dbg.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.dbg
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
 *   Just some macros for debugging
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

#ifndef DBG_H
#define DBG_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - iostream
//   - cerrno
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   none
//------------------------------------------------------------

#include <iostream>
#include <cerrno>

//============================================================
// Group: Public Declarations
//============================================================

//------------------------------------------------------------
// Topic: Macros
//   - DEBUG_BUILD - Conditional macro (see DBG(s))
//   - DBG(s)      - Shows a debug message on StdErr
//------------------------------------------------------------

#define DEBUG_BUILD
//#define DEBUG_CHECKINOUT

#ifdef DEBUG_BUILD

#  define DUMPVAR(v) #v << ": " << v << ' '

#  define DBG(s) std::cerr << clock() << ":" \
    << __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__			\
    << ": " << s << std::endl << std::flush

#  ifdef DEBUG_CHECKINOUT
#    define CHKIN do { \
        chkLevel++; \
        std::cerr << __FILE__ << ':' << __LINE__	\
                  << ": " << spaces.substr(0, chkLevel * 2) \
                  << "Entering " << __FUNCTION__ << std::endl << std::flush; \
      } while(0)
#    define CHKOUT do { \
        std::cerr << __FILE__ << ':' << __LINE__	\
                << ": " << spaces.substr(0, chkLevel * 2) \
                << "Exiting " << __FUNCTION__ << std::endl << std::flush; \
        chkLevel--; \
      } while(0)

static std::string spaces("                                                "
                          "                                                "
                          "                                                "
                          "                                             ...");
#else
#    define CHKIN
#    define CHKOUT
#endif

void showBacktrace();

#else

#  define DUMPVAR(v)
#  define DBG(s)
#  define CHKIN
#  define CHKOUT

#endif

#include <string>

inline std::string mid(std::string s, size_t from, size_t howmany = 1000) {
    try {
        return s.substr(from, howmany);
    } catch (...) {
        std::cerr << "ERROR: Trying to extract from '" << s
                  << "' string " << howmany
                  << " chars, starting at pos " << from << std::endl;
        abort();
    }
}


#endif // DBG_H

