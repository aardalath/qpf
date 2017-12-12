/******************************************************************************
 * File:    scopeexit.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.ScopeExit
 *
 * Version:  1.0
 *
 * Date:    2017/12/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of ScopeExit
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
#ifndef SCOPEEXIT_H
#define SCOPEEXIT_H

#include <functional>

//======================================================================
// Class: ScopeExit
// Auto-release resources upon exit of scope
//======================================================================
class ScopeExit {
public:
    ScopeExit(std::function<void()> f): f_(f) {}
    ~ScopeExit() { f_(); }
private:
    std::function<void()> f_;
};

#endif // SCOPEEXIT_H
