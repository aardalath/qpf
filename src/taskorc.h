/******************************************************************************
 * File:    taskorc.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.TaskOrchestrator
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
 *   Declare askOrchestrator class
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

#ifndef TASKORC_H
#define TASKORC_H

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
#include "cfg.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//==========================================================================
// Class: TaskOrchestrator
//==========================================================================
class TaskOrchestrator : public Component {

public:
    TaskOrchestrator(const char * name = 0);

    //----------------------------------------------------------------------
    // Method: defineOrchestrationParams
    //----------------------------------------------------------------------
    void defineOrchestrationParams(const OrchestrationParameters & op);

protected:

    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    virtual void fromRunningToOperational();

    //----------------------------------------------------------------------
    // Method: fromRunningToOff
    //----------------------------------------------------------------------
    virtual void fromRunningToOff();

protected:

    //----------------------------------------------------------------------
    // Method: processINDATA
    //----------------------------------------------------------------------
    virtual void processINDATA();

private:
    //----------------------------------------------------------------------
    // Method: checkRulesForProductType
    // Check if any of the rules that involve productType as input can
    // be fired
    //----------------------------------------------------------------------
    bool checkRulesForProductType(std::string prodType,
                                  RuleInputs & ruleInputs);

    //----------------------------------------------------------------------
    // Method: sendTaskProcMsg
    // Send a TaskProcessingMsg to the Task Manager, requesting the
    // execution of a rule
    //----------------------------------------------------------------------
    bool sendTaskProcMsg(Rule * rule,
                         ProductList & inputs);

private:
    OrchestrationParameters  orcParams;
    OrchestrationMaps        orcMaps;

    ProductCollection        catalogue;
};

}

#endif  /* TASKORC_H */
