/******************************************************************************
 * File:    log.cpp
 *          This file is part of the LibComm communications library
 *
 * Domain:  LibComm.LibComm.Log
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
 *   Implement Log class
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

#include "log.h"
#include "common.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <sstream>

////////////////////////////////////////////////////////////////////////////
// Namespace: LibComm
// -----------------------
//
// Library namespace
////////////////////////////////////////////////////////////////////////////
namespace LibComm {

std::map<std::string, std::fstream *>            Log::logStream;
std::map<std::string, int >                      Log::sizeOfLogBuffer;
std::map<std::string, std::vector<std::string> > Log::logBuffer;
std::map<std::string, bool> Log::dateChangeShown = {};
std::string Log::dateString("");

std::string Log::logBaseDir = std::string("/qpf/run");

static const Log::LogLevel DefaultMinimumLogLevel = Log::INFO;

Log::LogLevel Log::minimumLogLevel = DefaultMinimumLogLevel;

bool Log::consoleOutput = false;
bool Log::quietExit = false;

static void showBacktrace(void);

//----------------------------------------------------------------------
// Constructor: Log
// This constructor is made protected to make a singleton
//----------------------------------------------------------------------
Log::Log()
{
}

//----------------------------------------------------------------------
// Destructor: ~Log
// Virtual destructor
//----------------------------------------------------------------------
Log::~Log()
{
}

//----------------------------------------------------------------------
// Static Method: log
// Put message into log stream
//----------------------------------------------------------------------
void Log::log(std::string caller, Log::LogLevel level, std::string message)
{
  static char LogLevelLetters[] = {'T', 'D', 'I', 'W', 'E', 'F'};

  std::map<std::string, std::fstream *>::iterator logIt;
  logIt = logStream.find(caller);
  if (logIt == logStream.end()) {
    // Caller not registered
    std::stringstream msg;
    msg << "Caller '" 	<< caller << "' not registered in logging system!";
    log("SYSTEM", Log::WARNING, msg.str());
    return;
  }

  std::fstream * fs = logIt->second;
  std::string severityTag = "";
  if (level == Log::ERROR) { severityTag = "ERROR: "; }
  if (level == Log::FATAL) { severityTag = "FATAL: "; }
  std::string tag = getTimeTag();
  std::string msg = tag + " " + caller
    + " [" + LogLevelLetters[(int)(level)] + "] " + severityTag + message + "\n";
  if (!dateChangeShown[caller]) {
      (*fs) << tag + " " + caller
               + " [" + LogLevelLetters[(int)(level)] + "] " + dateString + "\n";
      dateChangeShown[caller] = true;
  }
  (*fs) << msg;
  fs->sync();
  if (consoleOutput) {
    std::cout << msg;
    std::cout.flush();
  }

  if (level == Log::FATAL) {
    std::cerr << msg;
    exit(1);
  }

  int logMsgs = logBuffer[caller].size();
  int maxMsgs = sizeOfLogBuffer[caller];

  if (logMsgs >= maxMsgs) {
    for (int i = 0; i < (logMsgs - maxMsgs + 1); ++i) {
      logBuffer[caller].erase(logBuffer[caller].begin());
    }
  }
  logBuffer[caller].push_back(msg);
}

//----------------------------------------------------------------------
// Static Method: getLastLogMsgs
// Get the content of the log buffer for the caller
//----------------------------------------------------------------------
void Log::getLastLogMsgs(std::string caller, std::vector<std::string> & lastMsgs)
{
  lastMsgs.clear();
  lastMsgs = logBuffer[caller];
}

//----------------------------------------------------------------------
// Function: getTimeTag
// Provides a date+time tag in the form HH:MM:SS.ssssss
//
// The tag does not include the date, but the system logs the date
// changes from one day to the next one.
//----------------------------------------------------------------------
std::string Log::getTimeTag()
{
  static int yday = 0;
  static const char * wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  time_t secs;
  struct tm locTime;
  struct timeval tv;

  time(&secs);
  (void)localtime_r(&secs, &locTime);

  if (locTime.tm_yday != yday) {
    yday = locTime.tm_yday;
    std::stringstream msg;
    char hour[20];
    sprintf(hour, "%04d-%02d-%02d",
            1900 + locTime.tm_year, locTime.tm_mon, locTime.tm_mday);
    msg << "Date: "
        << hour << "  " << wday[locTime.tm_wday]
        << "  (year day " << locTime.tm_yday << ")";
    dateString = msg.str();
    for (std::map<std::string, std::fstream *>::iterator it = logStream.begin();
         it != logStream.end(); ++it) {
        dateChangeShown[it->first] = false;
//      log(it->first, Log::INFO, msg.str());
    }
    time(&secs);
    (void)localtime_r(&secs, &locTime);
  }

  gettimeofday(&tv, NULL);

  char dateTimeArray[15+1];
  sprintf(dateTimeArray, "%02d:%02d:%02d.%06ld",
          locTime.tm_hour, locTime.tm_min, locTime.tm_sec, tv.tv_usec);

  return std::string(dateTimeArray);
}

//----------------------------------------------------------------------
// Static Method: setQuietExit
// Change internal quietExit data member
//----------------------------------------------------------------------
void Log::setQuietExit(bool val)
{
  quietExit = val;
}

//----------------------------------------------------------------------
// Static Method: defineLogSystem
// Initialises a log system for a given entity
//----------------------------------------------------------------------
void Log::defineLogSystem(std::string caller, std::string logFile, int numOfMsgs)
{
  static int recursiveCalls = 0;
  static bool isInitialised = false;
  if (!isInitialised) {
    isInitialised = true;
    std::string systemLogFile = logBaseDir + "/log/SYSTEM.syslog";
    recursiveCalls++;
    if (recursiveCalls > 10) { abort(); }
    defineLogSystem("SYSTEM", systemLogFile);
    atexit(Log::closeLogStreams);
  }

  std::fstream * fs = new std::fstream(logFile.c_str(),
                                       std::ios::out | std::ios::trunc);
  assert(!fs->fail());
  logStream[caller] = fs;

  defineSizeOfLogBuffer(caller, numOfMsgs);
}

//----------------------------------------------------------------------
// Static Method: defineSizeOfLogBuffer
// Specifies the size of the log buffer
//----------------------------------------------------------------------
void Log::defineSizeOfLogBuffer(std::string caller, int numOfMsgs)
{
  std::vector<std::string> emptyBuffer;

  sizeOfLogBuffer[caller] = numOfMsgs;
  logBuffer[caller]       = emptyBuffer;
}

//----------------------------------------------------------------------
// Static Method: closeLogStreams
// Closes streams before exit
//----------------------------------------------------------------------
void Log::closeLogStreams()
{
  if (! quietExit) {
    for (std::map<std::string, std::fstream *>::iterator it = logStream.begin();
         it != logStream.end(); ++it) {
      log(it->first, Log::INFO, "***** Automatic log stream closing by SYSTEM *****");
    }
  }
  for (std::map<std::string, std::fstream *>::iterator it = logStream.begin();
       it != logStream.end(); ++it) {
    it->second->close();
  }
}

//----------------------------------------------------------------------
// Static Method: setConsoleOutput
// Sets the consoleOutput internal flag
//----------------------------------------------------------------------
void Log::setConsoleOutput(bool b)
{
  consoleOutput = b;
}

//----------------------------------------------------------------------
// Static Method: setLogBaseDir
// Change base directory for log file
//----------------------------------------------------------------------
void Log::setLogBaseDir(std::string base)
{
    logBaseDir = base;
}

//----------------------------------------------------------------------
// Static Method: getLogBaseDir
// Access to base directory for log file
//----------------------------------------------------------------------
std::string Log::getLogBaseDir()
{
    return logBaseDir;
}

//----------------------------------------------------------------------
// Static Method: setMinLogLevel
// Sets the value of the minimum log level
//
// Parameters:
//   lvl - (I) Minimum log level to be used
//----------------------------------------------------------------------
void Log::setMinLogLevel(Log::LogLevel lvl)
{
    Log::minimumLogLevel = lvl;
}

//----------------------------------------------------------------------
// Static Method: getMinLogLevel
// Returns the value of the minimum log level
//
// Returns:
//   Value of the minimum log level
//----------------------------------------------------------------------
Log::LogLevel Log::getMinLogLevel()
{
    return Log::minimumLogLevel;
}

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void showBacktrace(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    fprintf(stderr, "backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        fprintf(stderr, "%s\n", strings[j]);

    free(strings);
}

}
