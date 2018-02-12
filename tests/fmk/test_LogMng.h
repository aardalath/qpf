#ifndef TEST_LOGMNG_H
#define TEST_LOGMNG_H

#include "logmng.h"
#include "gtest/gtest.h"

//using namespace LogMng;

namespace TestLogMng {

class TestLogMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLogMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLogMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // LogMng::obj ev;
};

class TestLogMngExit : public TestLogMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLogMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLogMngExit() {}

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

#endif // TEST_LOGMNG_H
