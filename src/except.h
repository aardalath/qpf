/******************************************************************************
 * File:    except.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.except
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
 *   Declare exceptions hierarchy
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

#ifndef EXCEPT_H
#define EXCEPT_H

//------------------------------------------------------------
// Topic: System dependencies
//------------------------------------------------------------
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

// Exceptions (and errors) here defined derive from base
// std::runtime_error

class Exception : public std::exception {
public:
    Exception(std::string const & msg, int i = -1);
    virtual ~Exception() noexcept;
    virtual char const* what() const noexcept;
    virtual int code() const noexcept;
protected:
    std::string const msg_;
    int code_;
};

class RuntimeException : public Exception {
public:
    RuntimeException(std::string const & msg);
};

class LogicException : public Exception {
public:
    LogicException(std::string const & msg);
};

class Error : public Exception {
public:
    Error(std::string const & msg, int i);
};


}

#endif  /* EXCEPT_H */

