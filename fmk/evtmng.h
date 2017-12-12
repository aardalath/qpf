/******************************************************************************
 * File:    evtmng.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.Component
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declare Component class
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   MsgTypes, CommNode
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
//------------------------------------------------------------
#include <mutex>

//------------------------------------------------------------
// Topic: External packages
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   . component.h
//   - dwatcher.h
//------------------------------------------------------------
#include "component.h"
#include "dwatcher.h"

//==========================================================================
// Class: EventManager
//==========================================================================
class EvtMng : public Component {

public:
    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    EvtMng(const char * name, const char * addr = 0, Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    EvtMng(std::string name, std::string addr = std::string(), Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: runEachIteration
    //----------------------------------------------------------------------
    virtual void runEachIteration();

public:
    //----------------------------------------------------------------------
    // Method: runEachIteration
    //----------------------------------------------------------------------
    bool getInData(ProductList & inData, std::string & space);

    //----------------------------------------------------------------------
    // Method: getReprocData
    // Store in argument variables the REPROCDATA products
    //----------------------------------------------------------------------
    bool getReprocData(ProductList & reprocData);
    
    //----------------------------------------------------------------------
    // Method: isHMIActive
    //----------------------------------------------------------------------
    bool isHMIActive();
    
    //----------------------------------------------------------------------
    // Method: isQuitRequested
    //----------------------------------------------------------------------
    bool isQuitRequested();
    
    //----------------------------------------------------------------------
    // Method: isQuitRequested
    //----------------------------------------------------------------------
    void quit();

    //----------------------------------------------------------------------
    // Method: sendProcFmkInfoUpdate
    // Send an update on the ProcessingFrameworkInfo structure
    //----------------------------------------------------------------------
    void sendProcFmkInfoUpdate(json & fmkInfoValue);

protected:
    //----------------------------------------------------------------------
    // Method: processHMICmdMsg
    //----------------------------------------------------------------------
    virtual void processHMICmdMsg(ScalabilityProtocolRole* c, MessageString & m);

private:
    DirWatcher * dw;

    ProductList inboxProducts;
    ProductList reprocProducts;

    std::mutex mtxInData;
    std::mutex mtxReproc;
    std::mutex mtxHostInfo;

    std::map<std::string, json> elements;

    bool requestQuit;
    bool hmiActive;
};
#endif
