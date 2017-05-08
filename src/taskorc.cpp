/******************************************************************************
 * File:    taskorc.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskorc
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
 *   Implement TaskOrchestrator class
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

#include "taskorc.h"
#include "urlhdl.h"
#include "str.h"
#include "infixeval.h"

#include "log.h"
using LibComm::Log;
#include "tools.h"

#include <sys/time.h>
#include <iterator>


////////////////////////////////////////////////////////////////////////////
// Namespace: QPF
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace QPF {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TaskOrchestrator::TaskOrchestrator(const char * name) :
    Component(name)
{
    canProcessMessage(MSG_INDATA_IDX);
}

//----------------------------------------------------------------------
// Method: defineOrchestrationParams
//----------------------------------------------------------------------
void TaskOrchestrator::defineOrchestrationParams(const OrchestrationParameters & op)
{
    // Take orchestration parameters from parent
    // (obtained from configuration)
    orcParams.productTypes = op.productTypes;
    for (unsigned int i = 0; i < op.rules.size(); ++i) {
        orcParams.rules.push_back(new Rule(*(op.rules.at(i))));
    }
    std::map<std::string, Processor*>::const_iterator peIt = op.processors.begin();
    while (peIt != op.processors.end()) {
        orcParams.processors[(*peIt).first] = new Processor(*((*peIt).second));
        ++peIt;
    }

    // Create map from product type to rules
    for (unsigned int i = 0; i < orcParams.rules.size(); ++i) {
        Rule * rule = orcParams.rules.at(i);
        for (unsigned int j = 0; j < rule->inputs.size(); ++j) {
            std::string inputProduct = rule->inputs.at(j);
            orcMaps.prodAsInput.insert(std::pair<std::string, Rule *>(inputProduct, rule));
        }
    }
}

//----------------------------------------------------------------------
// Method: fromRunningToOperational
//----------------------------------------------------------------------
void TaskOrchestrator::fromRunningToOperational()
{
    // Setup orchestration parameters
    defineOrchestrationParams(ConfigurationInfo::data().orcParams);

    // Dump rules
    std::stringstream ss;
    for (unsigned int i = 0; i < orcParams.rules.size(); ++i) {
        Rule * rule = orcParams.rules.at(i);
        ss.str("");
        ss << rule->processingElement << " :: ";
        std::copy(rule->inputs.begin(), rule->inputs.end(),
                  std::ostream_iterator<std::string>(ss," "));
        ss << " ==[" + rule->condition + "]==> ";
        std::copy(rule->outputs.begin(), rule->outputs.end(),
                  std::ostream_iterator<std::string>(ss," "));
        InfoMsg("Orc.Rule#" + str::toStr<int>(i + 1) + ":  " + ss.str());
        orcMaps.ruleDesc[rule] = ss.str();
    }

}

//----------------------------------------------------------------------
// Method: fromRunningToOff
//----------------------------------------------------------------------
void TaskOrchestrator::fromRunningToOff()
{
    InfoMsg("Ending . . . ");
}

//----------------------------------------------------------------------
// Method: processINDATA
//----------------------------------------------------------------------
void TaskOrchestrator::processINDATA()
{
    Message_INDATA * msg = dynamic_cast<Message_INDATA*>(msgData.msg);

    // Synthetic INDATA messages, that means reading products from folder
    for (auto & md : msg->productsMetadata.productList) {
        // Append product to catalogue
        catalogue.productList[md.first] = md.second;

        // Check the product type as input for any rule
        RuleInputs ruleInputs;
        if (checkRulesForProductType(md.first, ruleInputs)) {
            for (auto & kv : ruleInputs) {
                InfoMsg("Product type " + md.first + " fires rule: " +
                        orcMaps.ruleDesc[kv.first]);
                for (auto & itInp : kv.second.productList) {
                    InfoMsg("Input: " + itInp.productId);
                }
                // Generate and send processing task to TskMng
                sendTaskProcMsg(kv.first, kv.second);
            }
        }
    }
}

//----------------------------------------------------------------------
// Method: checkRulesForProductType
// Check if any of the rules that involve productType as input can
// be fired
//----------------------------------------------------------------------
bool TaskOrchestrator::checkRulesForProductType(std::string prodType,
                                                RuleInputs & ruleInputs)
{
    bool atLeastOneRuleFired = false;

    ruleInputs.clear();

    DbgMsg("Current catalogue contents:");
    for (auto & kv : catalogue.productList) {
        ProductMetadata & m = kv.second;
        DbgMsg("  - " + kv.first + " : " + m.productId + " [" + m.url + "]");
    }

    // Select all the rules that involve prodType as input
    std::pair<std::multimap<std::string, Rule *>::iterator,
            std::multimap<std::string, Rule *>::iterator> range;
    range = orcMaps.prodAsInput.equal_range(prodType);

    // If no rule found for that product type, no rule can be fired
    if (range.first == range.second) { return false; }

#define EVAL_CONDITION

#ifdef EVAL_CONDITION
    // Condition evaluator
    InFix::Evaluator<int> ev;
#else
#endif

    // Loop on selected rules
    std::multimap<std::string, Rule *>::iterator it = range.first;
    for (; it != range.second; ++it) {

#ifdef EVAL_CONDITION
        // Rule condition
        std::string condStr("");
#else
#endif
        Rule * rule = (*it).second;
        std::set<ProductType> requiredInputs(rule->inputs.begin(),
                                             rule->inputs.end());
        std::set<ProductType> availableInputs;
        ProductList inputs;

        // Check if all the inputs for this rule are available in the DB
        for (unsigned int i = 0; i < rule->inputs.size(); ++i) {
            std::string prd = rule->inputs.at(i);

            // Select all the products in the catalogue with this product type
            std::pair<std::multimap<std::string, ProductMetadata>::iterator,
                    std::multimap<std::string, ProductMetadata>::iterator> rangeProd;
            rangeProd = catalogue.productList.equal_range(prd);

            // Check that we have at least one product with the right product
            // type (specified by the loop)
            // Otherwise, the rule cannot be fired
            if (rangeProd.first == rangeProd.second) {
                inputs.productList.clear();
                continue;
            }

            // If this input product is available, store its metadata in the
            // list for the current rule
            std::multimap<std::string, ProductMetadata>::iterator itt = rangeProd.second;
            itt--;
            ProductMetadata & m = (*itt).second;
            std::string & pt = m.productType;
            inputs.productList.push_back(m);
            availableInputs.insert(pt);

#ifdef EVAL_CONDITION
            // Store also its metadata fields for rule conditions in the
            // condition string
            int date1 = str::strTo<int>(m.startTime.substr(0, 8));
            int time1 = str::strTo<int>(m.startTime.substr(9, 6));
//            int date2 = str::strTo<int>(m.endTime.substr(0, 8));
//            int time2 = str::strTo<int>(m.endTime.substr(9, 6));
            condStr += pt + ".date = " + str::toStr<int>(date1) + ";" +
                       pt + ".time = " + str::toStr<int>(time1) + ";" /*+
                       pt + ".date_end = " + str::toStr<int>(date2) + ";" +
                       pt + ".time_end = " + str::toStr<int>(time2) + ";"*/;
#else
#endif
        }

        std::ostream_iterator<ProductType> out_it (std::cerr, ", ");

        std::cerr << "requiredInputs :> ";
        std::copy(requiredInputs.begin(), requiredInputs.end(), out_it);
        std::cerr << "availableInputs :> ";
        std::copy(availableInputs.begin(), availableInputs.end(), out_it);
        std::cerr << std::endl;

        if (availableInputs == requiredInputs) {
#ifdef EVAL_CONDITION
            // Evaluate the rule condition:
            int result = 0;
            if (! rule->condition.empty()) {
                // a. Include the condition in the condition string
                condStr += rule->condition;
                // b. Evaluate condition
                std::cerr << "Evaluating condition: " << condStr << std::endl;
                ev.clear();
                ev.set(condStr);
                try {
                    result = ev.getValue();
                } catch (...) {
                    result = 1;
                }
            } else {
                result = 1;
            }
            if (result > 0) {
                ruleInputs[rule] = inputs;
                atLeastOneRuleFired = true;
            }
#else
            ruleInputs[rule] = inputs;
            atLeastOneRuleFired = true;
#endif
        }

    } // check for each rule with provided product type

    return atLeastOneRuleFired;
}

//----------------------------------------------------------------------
// Method: sendTaskProcMsg
// Send a TaskProcessingMsg to the Task Manager, requesting the
// execution of a rule
//----------------------------------------------------------------------
bool TaskOrchestrator::sendTaskProcMsg(Rule * rule,
                                       ProductList & inputs)
{
    // Define task object
    TaskInfo task;

    DateTime epoch = LibComm::timeTag();
    task.taskName = "task-" + rule->name + "_" + epoch;
    task.taskPath = rule->processingElement;
    task.taskStart = epoch;
    task.taskEnd = "";
    task.taskExitCode = 0;
    task.taskStatus = TASK_SCHEDULED;

    URLHandler urlh;

    for (unsigned int i = 0; i < inputs.productList.size(); ++i) {
        urlh.setProduct(inputs.productList.at(i));
        ProductMetadata & m = urlh.fromLocalArch2Gateway();
        task.inputs.productList[m.productType] = m;
    }

    for (unsigned int i = 0; i < rule->outputs.size(); ++i) {
        ProductMetadata m;
        m.productType = rule->outputs.at(i);
        task.outputs.productList[m.productType] = m;
    }

    task.params.paramList.clear();

    MessageHeader hdr;
    buildMsgHeader(MSG_TASK_PROC_IDX, "TskOrc", "", hdr);

    Message_TASK_PROC msg;
    buildMsgTASKPROC(hdr, task, rule->name, msg);

    std::array<std::string,2> fwdRecip = {"TskMng", "DataMng"};
    for (std::string & recip : fwdRecip) {
        MessageData msgToRecip(new Message_TASK_PROC);
        msgToRecip.msg->setData(msg.getData());
        setForwardTo(recip, msgToRecip.msg->header);
        PeerMessage * msgForRecip = buildPeerMsg(msgToRecip.msg->header.destination,
                                                 msgToRecip.msg->getDataString(),
                                                 MSG_TASK_PROC);
        registerMsg(selfPeer()->name, *msgForRecip);
        setTransmissionToPeer(recip, msgForRecip);
    }

    return true;
}

}
