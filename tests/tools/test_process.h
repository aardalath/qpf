#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include "process.h"
#include "gtest/gtest.h"

//using namespace process;

namespace Testprocess {

class Testprocess : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testprocess() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testprocess() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // process::obj ev;
};

class TestprocessExit : public Testprocess {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestprocessExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestprocessExit() {}

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

#endif // TEST_PROCESS_H
