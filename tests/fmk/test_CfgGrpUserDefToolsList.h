#ifndef TEST_CFGGRPUSERDEFTOOLSLIST_H
#define TEST_CFGGRPUSERDEFTOOLSLIST_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpUserDefToolsList;

namespace TestCfgGrpUserDefToolsList {

class TestCfgGrpUserDefToolsList : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpUserDefToolsList() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpUserDefToolsList() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpUserDefToolsList::obj ev;
};

class TestCfgGrpUserDefToolsListExit : public TestCfgGrpUserDefToolsList {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpUserDefToolsListExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpUserDefToolsListExit() {}

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

#endif // TEST_CFGGRPUSERDEFTOOLSLIST_H
