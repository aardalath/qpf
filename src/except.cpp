/******************************************************************************
 * File:    except.cpp
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
 *   Implement exception classes methods
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

#include "except.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

Exception::Exception(std::string const & msg, int i) : msg_(msg), code_(i)
{}

Exception::~Exception() noexcept
{}

char const* Exception::what() const noexcept
{
    return msg_.c_str();
}

int Exception::code() const noexcept
{
    return code_;
}

RuntimeException::RuntimeException(std::string const & msg) : Exception(msg)
{}
 
LogicException::LogicException(std::string const & msg) : Exception(msg)
{}
 
Error::Error(std::string const & msg, int i) : Exception(msg, i)
{}
 
}


