/******************************************************************************
 * File:    logmng.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.LogManager
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015,2016,2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare LogManager class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   Component
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

#ifndef LOGMNG_H
#define LOGMNG_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - component.h
//------------------------------------------------------------

#include "component.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: LogManager
//==========================================================================
class LogManager : public Component {

public:
    LogManager(const char * name = 0);

protected:

    //----------------------------------------------------------------------
    // Method: processDATA_INFO
    //----------------------------------------------------------------------
    virtual void processDATA_INFO();

    //----------------------------------------------------------------------
    // Method: execAdditonalLoopTasks
    //----------------------------------------------------------------------
    virtual void execAdditonalLoopTasks();

};

}

#endif  /* LOGMNG_H */
