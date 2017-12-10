/******************************************************************************
 * File:    taskorc.h
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.TaskOrchestrator
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

#ifndef TSKORC_H
#define TSKORC_H

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

//==========================================================================
// Class: TskOrc
//==========================================================================
class TskOrc : public Component {

public:
    struct Rule {
        std::string              name;
        //std::string              tag;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
        std::string              processingElement;
        std::string              condition;
    };

    typedef std::map<Rule *, ProductList>  RuleInputs;

    struct Processor {
        std::string name;
        std::string exePath;
        std::string inPath;
        std::string outPath;
        std::string version;
        int         counter;
    };

    struct OrchestrationParameters {
        std::vector<std::string>            productTypes;
        std::vector<Rule *>                 rules;
        std::map<std::string, Processor *>  processors;
    };

    struct OrchestrationMaps {
        std::multimap<std::string, Rule *>  prodAsInput;
        std::map<Rule *, std::string>       ruleDesc;
    };

public:
    //----------------------------------------------------------------------
    // ConstructorMethod: defineOrchestrationParams
    //----------------------------------------------------------------------
    TskOrc(const char * name, const char * addr = 0, Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    TskOrc(std::string name, std::string addr = std::string(), Synchronizer * s = 0);

    //----------------------------------------------------------------------
    // Method: defineOrchestrationParams
    //----------------------------------------------------------------------
    void defineOrchestrationParams();

    //----------------------------------------------------------------------
    // Method: createTasks
    //----------------------------------------------------------------------
    void createTasks(ProductList & inData, std::vector<TaskInfo> & tasks);
    
    //----------------------------------------------------------------------
    // Method: createTask
    //----------------------------------------------------------------------
    void createTask(Rule * rule, ProductList & inputs, TaskInfo & task);
    
protected:
    //----------------------------------------------------------------------
    // Method: fromRunningToOperational
    //----------------------------------------------------------------------
    void fromRunningToOperational();

private:
    //----------------------------------------------------------------------
    // Method: checkRulesForProductType
    //----------------------------------------------------------------------
    bool checkRulesForProductType(std::string prodType,
                                  RuleInputs & ruleInputs);

private:
    OrchestrationParameters  orcParams;
    OrchestrationMaps        orcMaps;

    ProductCollection        catalogue;
};

#endif  /* TSKORC_H */
