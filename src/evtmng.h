/******************************************************************************
 * File:    evtmng.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.EventManager
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
 *   Declare EventManager class
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

#ifndef EVTMNG_H
#define EVTMNG_H

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
#include "dwatcher.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: EventManager
//==========================================================================
class EventManager : public Component {

public:
    EventManager(const char * name = 0);

public:

    //----------------------------------------------------------------------
    // Method: go
    //----------------------------------------------------------------------
    void go();

protected:

    //----------------------------------------------------------------------
    // Method: fromInitialisedToRunning
    //----------------------------------------------------------------------
    virtual void fromInitialisedToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

    //----------------------------------------------------------------------
    // Method: execAdditonalLoopTasks
    //----------------------------------------------------------------------
    virtual void execAdditonalLoopTasks();

protected:

    //----------------------------------------------------------------------
    // Method: processINDATA
    //----------------------------------------------------------------------
    virtual void processINDATA();

    //----------------------------------------------------------------------
    // Method: processTASK_RES
    //----------------------------------------------------------------------
    virtual void processTASK_RES();

    //----------------------------------------------------------------------
    // Method: processMONIT_INFO
    //----------------------------------------------------------------------
    virtual void processMONIT_INFO();
#ifdef XCMD
    //----------------------------------------------------------------------
    // Method: processCMD
    //----------------------------------------------------------------------
    virtual void processCMD();
#endif
private:
    std::atomic<bool> waitingForGO;
    DirWatcher * dw;
};

}

#endif  /* EVTMNG_H */
