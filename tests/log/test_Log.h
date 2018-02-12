#ifndef TEST_LOG_H
#define TEST_LOG_H

#include "log.h"
#include "gtest/gtest.h"

//using namespace Log;

namespace TestLog {

class TestLog : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLog() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLog() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Log::obj ev;
};

class TestLogExit : public TestLog {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLogExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLogExit() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
};

}

#endif // TEST_LOG_H
