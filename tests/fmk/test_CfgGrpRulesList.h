#ifndef TEST_CFGGRPRULESLIST_H
#define TEST_CFGGRPRULESLIST_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpRulesList;

namespace TestCfgGrpRulesList {

class TestCfgGrpRulesList : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpRulesList() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpRulesList() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpRulesList::obj ev;
};

class TestCfgGrpRulesListExit : public TestCfgGrpRulesList {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpRulesListExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpRulesListExit() {}

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

#endif // TEST_CFGGRPRULESLIST_H
