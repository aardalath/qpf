#include "test_log.h"

#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <sys/time.h>
#include <time.h>

#include "tools.h"

using namespace LibComm;

namespace LibCommTests {

std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos)
{
    pos = in.tellg();
    return in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getLastLine(std::string fileName)
{
    std::ifstream in(fileName);
    std::ifstream::pos_type pos = in.tellg();

    std::ifstream::pos_type lastPos;
    while (in >> std::ws && ignoreline(in, lastPos))
        pos = lastPos;

    in.clear();
    in.seekg(pos);

    std::string line;
    std::getline(in, line);
    return line;
}

TEST_F(TestLog, AcceptsBasicSettings) {
    Log::setConsoleOutput(true);
    Log::setQuietExit(true);
    Log::setQuietExit(false);
    SUCCEED();
}

TEST_F(TestLog, HandleLogBaseDir) {
    /* Current configuration makes /qpg/run the log base dir */
    EXPECT_EQ(Log::getLogBaseDir(), std::string("/qpf/run"));
    Log::setLogBaseDir("/var/run");
    EXPECT_EQ(Log::getLogBaseDir(), std::string("/var/run"));
    Log::setLogBaseDir("/tmp");
    EXPECT_EQ(Log::getLogBaseDir(), std::string("/tmp"));
}

TEST_F(TestLog, CanDefineLogSystem) {
    mkdir("/tmp/log", 0777);
    Log::defineLogSystem("testlog", "/tmp/log/testlog.log");
    // Test it by writing messages to the log
    int bufferSize = 1000; // default buffer size
    for (int i = 0; i < bufferSize; ++i) {
        Log::log("testlog", Log::DEBUG, "This is a DEBUG message");
        Log::log("testlog", Log::INFO, "This is an informational message");
        Log::log("testlog", Log::WARNING, "WARNING! Something strange happens!");
        Log::log("testlog", Log::ERROR, "ERROR: Cannot work properly!");
    }
    SUCCEED();
    // Get buffer of last messages
    std::vector<std::string> lastMsgs;
    Log::getLastLogMsgs("testlog", lastMsgs);
    EXPECT_EQ(lastMsgs.size(), bufferSize);
    // Check last line in log file
    std::string lastFileLine = getLastLine("/tmp/log/testlog.log");
    EXPECT_EQ(lastMsgs.at(bufferSize - 1), lastFileLine + "\n");
}

TEST_F(TestLog, CanSetBufferSize) {
    int bufferSize = 100; // default buffer size
    Log::defineSizeOfLogBuffer("testlog", bufferSize);
    for (int i = 0; i < bufferSize; ++i) {
        Log::log("testlog", Log::DEBUG, "This is a DEBUG message");
        Log::log("testlog", Log::INFO, "This is an informational message");
        Log::log("testlog", Log::WARNING, "WARNING! Something strange happens!");
        Log::log("testlog", Log::ERROR, "ERROR: Cannot work properly!");
    }
    SUCCEED();
    // Get buffer of last messages
    std::vector<std::string> lastMsgs;
    Log::getLastLogMsgs("testlog", lastMsgs);
    EXPECT_EQ(lastMsgs.size(), bufferSize);
}

TEST_F(TestLog, ProvidesTimeTag) {
    std::string tag1 = Log::getTimeTag();
    std::string s = replaceAll(tag1, ":", " ");
    std::string fields = replaceAll(s, ".", " ");
    std::stringstream ss(fields);
    int hour, min, sec, usec;
    ss >> hour >> min >> sec >> usec;

    time_t tmt = time(0);
    struct tm * tmv = localtime(&tmt);

    EXPECT_EQ(hour,  tmv->tm_hour);
    EXPECT_EQ(min,   tmv->tm_min);
    EXPECT_EQ(sec,   tmv->tm_sec);
    EXPECT_LT(usec,  1000000);
}

TEST_F(TestLogExit, ExitsOnFatal) {
    EXPECT_EXIT({Log::log("testlog", Log::FATAL, "FATAL ERROR!!");},
                 ::testing::ExitedWithCode(1),
                "");
}

}

