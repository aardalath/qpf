/******************************************************************************
 * File:    statem.h
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
 *   Declare StateMachine class
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

#ifndef STATEM_H
#define STATEM_H

//============================================================
// Group: External Dependencies
//============================================================

//------------------------------------------------------------
// Topic: System headers
//   - string
//   - map
//------------------------------------------------------------

#include <string>
#include <map>

//------------------------------------------------------------
// Topic: External packages
//   none
//------------------------------------------------------------

//------------------------------------------------------------
// Topic: Project headers
//   - propdef.h
//------------------------------------------------------------
#include "propdef.h"
#include "log.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//==========================================================================
// Class: StateMachine
//==========================================================================
class StateMachine {

    Property(StateMachine, int, state, State);
    Property(StateMachine, Log::LogLevel, invalidTransit, InvalidTransit);

public:

    //----------------------------------------------------------------------
    // Constructor: StateMachine
    //----------------------------------------------------------------------
    StateMachine() : state(0), invalidTransit(Log::FATAL) {}

    //----------------------------------------------------------------------
    // Destructor: ~StateMachine
    // Virtual destructor
    //----------------------------------------------------------------------
    virtual ~StateMachine() {}

    //----------------------------------------------------------------------
    // Method: transitTo
    // Forces transit from current state to new state
    //
    // Parameter:
    //   newState - Target state identifier (code)
    //
    // Returns:
    //   True is transit to new state was possible
    //----------------------------------------------------------------------
    bool transitTo(int newState);

    //----------------------------------------------------------------------
    // Method: getStateName
    // Get name of state with provided identifier
    //
    // Parameter:
    //   aState - State identifier (code)
    //
    // Returns:
    //   Name of the corresponding state
    //----------------------------------------------------------------------
    std::string getStateName(int aState);

protected:

    //----------------------------------------------------------------------
    // Method: defineState
    // Initializes map of states
    //
    // Parameters:
    //   aState - State code (integer)
    //   name   - State Name
    //----------------------------------------------------------------------
    virtual void defineState(int aState, std::string name);

    //----------------------------------------------------------------------
    // Method: defineValidTransition
    // Defines valid transitions from state A to B
    //
    // Parameteres:
    //   fromState - Initial state
    //   toState   - Final state of the transition
    //----------------------------------------------------------------------
    virtual void defineValidTransition(int fromState, int toState);

protected:

    //----------------------------------------------------------------------
    // Variables: Transitions and states
    //   validTransitions - Map of valid transitions
    //   stateNames       - Map between state names and codes
    //----------------------------------------------------------------------
    std::multimap<int, int> validTransitions;
    std::map<int, std::string> stateNames;
};

}

#endif  /* STATEM_H */