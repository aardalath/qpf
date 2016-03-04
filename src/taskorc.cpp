/******************************************************************************
 * File:    taskorc.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.taskorc
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
void TaskOrchestrator::defineOrchestrationParams(OrchestrationParameters & op)
{
    // Take orchestration parameters from parent
    // (obtained from configuration)
    orcParams.productTypes = op.productTypes;
    for (unsigned int i = 0; i < op.rules.size(); ++i) {
        orcParams.rules.push_back(new Rule(*(op.rules.at(i))));
    }
    std::map<std::string, Processor*>::iterator peIt = op.processors.begin();
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
    // Dump rules
    std::stringstream ss;
    for (unsigned int i = 0; i < orcParams.rules.size(); ++i) {
        Rule * rule = orcParams.rules.at(i);
        ss.str("");
        ss << rule->processingElement << " :: ";
        std::copy(rule->inputs.begin(), rule->inputs.end(),
                  std::ostream_iterator<std::string>(ss," "));
        ss << " => ";
        std::copy(rule->outputs.begin(), rule->outputs.end(),
                  std::ostream_iterator<std::string>(ss," "));
        InfoMsg("Orc.Rule#" + LibComm::toStr<int>(i + 1) + ":  " + ss.str());
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
    Message_INDATA * msgInData = dynamic_cast<Message_INDATA*>(msgData.msg);
    for (auto it: msgInData->productsMetadata.productList) {
        // Append product to catalogue
        catalogue.productList[it.first] = it.second;

        // Check the product type as input for any rule
        RuleInputs ruleInputs;
        if (checkRulesForProductType(it.first, ruleInputs)) {
            for (auto & kv : ruleInputs) {
                InfoMsg("Product type " + it.first + " fires rule: " +
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
    bool areThereRulesToFire = false;

    ruleInputs.clear();

    // Select all the rules that involve prodType as input
    std::pair<std::multimap<std::string, Rule *>::iterator,
            std::multimap<std::string, Rule *>::iterator> range;
    range = orcMaps.prodAsInput.equal_range(prodType);

    // If no rule found for that product type, no rule can be fired
    if (range.first == range.second) { return false; }

    // Loop on selected rules
    std::multimap<std::string, Rule *>::iterator it = range.first;
    for (; it != range.second; ++it) {

        Rule * rule = (*it).second;
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
            inputs.productList.push_back((*itt).second);
        }

        if (inputs.productList.size() > 0) {
            ruleInputs[rule] = inputs;
            areThereRulesToFire = true;
        }

    }

    return areThereRulesToFire;
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

    for (unsigned int i = 0; i < inputs.productList.size(); ++i) {
        ProductMetadata & m = inputs.productList.at(i);
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
