#ifndef TEST_CFGGRPORCHESTRATION_H
#define TEST_CFGGRPORCHESTRATION_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpOrchestration;

namespace TestCfgGrpOrchestration {

class TestCfgGrpOrchestration : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpOrchestration() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpOrchestration() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpOrchestration::obj ev;
};

class TestCfgGrpOrchestrationExit : public TestCfgGrpOrchestration {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpOrchestrationExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpOrchestrationExit() {}

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

#endif // TEST_CFGGRPORCHESTRATION_H
