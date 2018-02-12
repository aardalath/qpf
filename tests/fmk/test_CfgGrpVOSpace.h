#ifndef TEST_CFGGRPVOSPACE_H
#define TEST_CFGGRPVOSPACE_H

#include "config.h"
#include "gtest/gtest.h"

//using namespace CfgGrpVOSpace;

namespace TestCfgGrpVOSpace {

class TestCfgGrpVOSpace : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpVOSpace() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpVOSpace() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // CfgGrpVOSpace::obj ev;
};

class TestCfgGrpVOSpaceExit : public TestCfgGrpVOSpace {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestCfgGrpVOSpaceExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestCfgGrpVOSpaceExit() {}

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

#endif // TEST_CFGGRPVOSPACE_H
