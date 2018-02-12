#ifndef TEST_CFGGRPSWARM_H
#define TEST_CFGGRPSWARM_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpSwarm;

namespace TestCfgGrpSwarm {

class TestCfgGrpSwarm : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpSwarm() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpSwarm() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpSwarm::obj ev;
};

class TestCfgGrpSwarmExit : public TestCfgGrpSwarm {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpSwarmExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpSwarmExit() {}

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

#endif // TEST_CFGGRPSWARM_H
