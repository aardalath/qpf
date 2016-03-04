/******************************************************************************
 * File:    statem.cpp
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.StateMachine
 *
 * Version: 0.0.1
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement StateMachine class
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
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "statem.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//----------------------------------------------------------------------
// Method: transitTo
//----------------------------------------------------------------------
bool StateMachine::transitTo(int newState)
{
  bool isValid = false;
  std::pair <std::multimap<int, int>::iterator,
             std::multimap<int, int>::iterator> validFrom;

  validFrom = validTransitions.equal_range(state);
  for (std::multimap<int, int>::iterator it = validFrom.first;
       it != validFrom.second; ++it) {
    if (it->second == newState) {
      isValid = true;
      setState( newState );
      break;
    }
  }

  if (!isValid) {
    if (stateNames.find(newState) != stateNames.end()) {
      Log::log("SYSTEM", invalidTransit,
               "Component tried an invalid state transition from "
               + stateNames[state] + " to " + stateNames[newState]);
    } else {
      Log::log("SYSTEM", invalidTransit,
               "Component tried an invalid state transition from "
               + stateNames[state] + " to an unknown state.");
    }
  }

  return isValid;
}

//----------------------------------------------------------------------
// Method: getStateName
//----------------------------------------------------------------------
std::string StateMachine::getStateName(int aState)
{
  return stateNames[aState];
}


//----------------------------------------------------------------------
// Method: defineState
// Initializes map of states
//----------------------------------------------------------------------
void StateMachine::defineState(int aState, std::string name)
{
  stateNames[aState] = name;
}

//----------------------------------------------------------------------
// Method: defineValidTransition
// Initializes map of valid transitions
//----------------------------------------------------------------------
void StateMachine::defineValidTransition(int fromState, int toState)
{
  validTransitions.insert(std::make_pair(fromState, toState));
}

}
