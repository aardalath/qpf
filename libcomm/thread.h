/******************************************************************************
 * File:    thread.h
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Thread
 *
 * Version: 1.0
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
 *   Declare Thread base class
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

#ifndef THREAD_H
#define THREAD_H

//------------------------------------------------------------
// Topic: System dependencies
//------------------------------------------------------------

#include <thread>
#include <mutex>

//------------------------------------------------------------
// Topic: Project dependencies
//------------------------------------------------------------

#include "propdef.h"

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

//==========================================================================
// Class: Thread
// ----------------
//
// Base class to classes that must run in a separate thread.
//==========================================================================
class Thread {

public:

    //----------------------------------------------------------------------
    // Enum: ThreadStatus
    // Thread status at any moment
    //
    // THR_START   - Initial status of the class
    // THR_RUNNING - The child thread has been launched and is running
    // THR_STOPPED - The child thread has been stopped
    // THR_DONE    - Thread has finished
    //----------------------------------------------------------------------
    enum ThreadStatus {
        THR_START,
        THR_RUNNING,
        THR_STOPPED,
        THR_DONE,
    };

public:
    //----------------------------------------------------------------------
    // Constructor: Thread
    // Constructor for the base class Thread
    //----------------------------------------------------------------------
    Thread();

    //----------------------------------------------------------------------
    // Destructor: Logger
    // (Virtual) Destructor
    //----------------------------------------------------------------------
    virtual ~Thread();

public:

    //----------------------------------------------------------------------
    // Method: create
    // Launches the child thread
    //----------------------------------------------------------------------
    void create();

    //----------------------------------------------------------------------
    // Method: stop
    // Stops the child thread
    //----------------------------------------------------------------------
    void stop();

    //----------------------------------------------------------------------
    // Method: wait
    // Waits for end of the child thread
    //----------------------------------------------------------------------
    int wait();

    //----------------------------------------------------------------------
    // Method: getThreadStatus
    // Access to the status of a single thread
    // Returns:
    //   The status of the thread
    //----------------------------------------------------------------------
    int getThreadStatus();

    //----------------------------------------------------------------------
    // Static: getGlobalStatus
    // Access to the status of a thread family
    // Returns:
    //   The status of a thread family
    //----------------------------------------------------------------------
    static int getGlobalStatus();

    //----------------------------------------------------------------------
    // Static Method: spawn
    // Mechanism to run thread as a separate process (experimental)
    //----------------------------------------------------------------------
    static pid_t spawn(Thread * aThreadObject);

protected:

    //----------------------------------------------------------------------
    // Method: run
    // (Virtual) Function that stores the functionality of the child thread
    //----------------------------------------------------------------------
    virtual int run() = 0;

    //----------------------------------------------------------------------
    // Static: bootstrap
    // Handy internal function to encapsulate the execution of a child thread
    //----------------------------------------------------------------------
    void bootstrap(void *aThread);

    //----------------------------------------------------------------------
    // Variable: thrId
    // Thread Identifier
    //----------------------------------------------------------------------
    std::thread                 thr;

private:

    //----------------------------------------------------------------------
    // Variable: mutexThreadStatus
    // Mutex for the <threadStatus> data member
    //----------------------------------------------------------------------
    std::mutex                  mutexThreadStatus;

    //----------------------------------------------------------------------
    // Variable: mutexGlobalStatus
    // Mutex for the <globalStatus> data member
    //----------------------------------------------------------------------
    static std::mutex           mutexGlobalStatus;

    //----------------------------------------------------------------------
    // Variable: threadStatus
    // Thread Status data member
    //----------------------------------------------------------------------
    int                         threadStatus;

    //----------------------------------------------------------------------
    // Variable: globalStatus
    // Thread Family Status data member
    //----------------------------------------------------------------------
    static int                  globalStatus;

};

}

#endif  /* THREAD_H */
