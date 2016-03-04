/******************************************************************************
 * File:    taskorc.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.TaskOrchestrator
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
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

    struct Rule {
        std::string              name;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
        std::string              processingElement;
    };

    struct ProcElem {
        std::string name;
        std::string exePath;
        std::string inPath;
        std::string outPath;
    };

    struct OrchestrationParameters {
        std::vector<std::string>           productTypes;
        std::vector<Rule *>                rules;
        std::map<std::string, ProcElem *>  procElems;
    };

    struct OrchestrationMaps {
        std::multimap<std::string, Rule *>  prodAsInput;
        std::map<Rule *, std::string>       ruleDesc;
    };

    typedef std::map<Rule *, ProductList>   RuleInputs;

    //----------------------------------------------------------------------
    // Method: defineOrchestrationParams
    //----------------------------------------------------------------------
    void defineOrchestrationParams(OrchestrationParameters & op);

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
