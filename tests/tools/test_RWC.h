#ifndef TEST_RWC_H
#define TEST_RWC_H

#include "rwc.h"
#include "gtest/gtest.h"

//using namespace RWC;

namespace TestRWC {

class TestRWC : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestRWC() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRWC() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // RWC::obj ev;
};

class TestRWCExit : public TestRWC {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestRWCExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRWCExit() {}

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

#endif // TEST_RWC_H
