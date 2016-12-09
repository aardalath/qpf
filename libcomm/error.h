/******************************************************************************
 * File:    error.h
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.error
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
 *   Definition of debug macro
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

#ifndef ERROR_H
#define ERROR_H

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

/* #define DEBUG_BUILD */

#ifdef DEBUG_BUILD
#  define DBG(s) std::cerr						\
  << __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__			\
  << ": " << s << std::endl << std::flush; std::cerr << std::flush
#else
#  define DBG(s)
#endif

#endif  /* ERROR_H */
