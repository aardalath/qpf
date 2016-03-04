#ifndef TEST_HMIPXY_H
#define TEST_HMIPXY_H

#include "hmipxy.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestHMIProxy : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHMIProxy() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHMIProxy() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    HMIProxy o;
};

class TestHMIProxyExit : public TestHMIProxy {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHMIProxyExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHMIProxyExit() {}

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

#endif // TEST_HMIPXY_H
