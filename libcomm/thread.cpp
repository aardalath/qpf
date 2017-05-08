/******************************************************************************
 * File:    thread.cpp
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Thread
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
 *   Implement Thread class
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

#include "thread.h"
#include "common.h"

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

namespace LibComm {

Thread::Thread()
{
    threadStatus = THR_RUNNING;
}

Thread::~Thread()
{
}

void Thread::create()
{
    thr = std::thread(&Thread::bootstrap, this, this);
}

void Thread::stop()
{
    mutexGlobalStatus.lock();
    globalStatus = THR_STOPPED;
    mutexGlobalStatus.unlock();
}

int Thread::wait()
{
    thr.join();
    return getThreadStatus();
}

int Thread::getThreadStatus()
{
    mutexThreadStatus.lock();
    int myThreadStatus = threadStatus;
    mutexThreadStatus.unlock();
    return myThreadStatus;
}

int Thread::getGlobalStatus()
{
    mutexGlobalStatus.lock();
    int myGlobalStatus = globalStatus;
    mutexGlobalStatus.unlock();
    return myGlobalStatus;
}

void Thread::bootstrap(void *aThread)
{
   Thread* myThread = (Thread*) aThread;

   int myThreadStatus = myThread->run();
   myThread->mutexThreadStatus.lock();
   myThread->threadStatus = myThreadStatus;
   myThread->mutexThreadStatus.unlock();
   if (myThreadStatus == THR_DONE) exit(0);
}

pid_t Thread::spawn(Thread * aThreadObject)
{
  pid_t childpid = fork(); /* Spawn process */
  if (childpid >= 0) {
    if (childpid == 0) { /* CHILD */
      int myThreadStatus = aThreadObject->run();
      exit((myThreadStatus == THR_DONE) ? 0 : myThreadStatus); 
    } else {
      return childpid;
    }
  } else {
    perror("fork to run Thread object as process");
    return 0;
  }
}

int Thread::globalStatus = THR_RUNNING;

std::mutex Thread::mutexGlobalStatus;

}
