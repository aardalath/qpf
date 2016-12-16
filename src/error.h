/******************************************************************************
 * File:    error.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.error
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
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef ERROR_H
#define ERROR_H

//------------------------------------------------------------
// Topic: System headers
//------------------------------------------------------------

#include <iostream>
#include <cerrno>

/* #define DEBUG_BUILD */

#ifdef DEBUG_BUILD
#  define DBG(s) std::cerr						\
  << __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__			\
  << ": " << s << std::endl << std::flush; std::cerr << std::flush 
#else
#  define DBG(s)
#endif

#endif  /* ERROR_H */

