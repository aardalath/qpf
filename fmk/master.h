/******************************************************************************
 * File:    master.h
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
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
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

#ifndef MASTER_H
#define MASTER_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: External packages
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   . component.h
//------------------------------------------------------------
#include "evtmng.h"
#include "datamng.h"
#include "logmng.h"
#include "tskorc.h"
#include "tskmng.h"
#include "tskage.h"

//==========================================================================
// Class: Master
//==========================================================================
class Master : public Component {

public:
    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    Master(const char * name, const char * addr = 0, Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    Master(std::string name, std::string addr = std::string(), Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Method: init
    //----------------------------------------------------------------------
    void init();

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromOperationalToRunning
    //----------------------------------------------------------------------
    virtual void fromOperationalToRunning();

    //----------------------------------------------------------------------
    // Method: setEvtMng
    //----------------------------------------------------------------------
    void setEvtMng(EvtMng * obj);
    
    //----------------------------------------------------------------------
    // Method: setTskMng
    //----------------------------------------------------------------------
    void setTskMng(TskMng * obj);
    
    //----------------------------------------------------------------------
    // Method: runEachIteration
    //----------------------------------------------------------------------
    virtual void runEachIteration();

private:
    EvtMng  * evtMng;
    DataMng * datMng;
    LogMng  * logMng;
    TskOrc  * tskOrc;
    TskMng  * tskMng;

    std::vector<Component*> subComponents;
    
    bool requestQuit;
};

#endif
