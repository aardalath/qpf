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

#ifdef DEBUG_BUILD

#  define DUMPVAR(v) #v << v
#  define DBG(s) std::cerr						\
  << __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__			\
  << ": " << s << std::endl << std::flush

void showBacktrace();

#else

#  define DUMPVAR(v)
#  define DBG(s)

#endif

#endif // DBG_H

