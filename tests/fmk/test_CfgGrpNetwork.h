#ifndef TEST_CFGGRPNETWORK_H
#define TEST_CFGGRPNETWORK_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpNetwork;

namespace TestCfgGrpNetwork {

class TestCfgGrpNetwork : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpNetwork() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpNetwork() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpNetwork::obj ev;
};

class TestCfgGrpNetworkExit : public TestCfgGrpNetwork {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpNetworkExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpNetworkExit() {}

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

#endif // TEST_CFGGRPNETWORK_H
