#ifndef TEST_MASTER_H
#define TEST_MASTER_H

#include "master.h"
#include "gtest/gtest.h"

//using namespace Master;

namespace TestMaster {

class TestMaster : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMaster() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMaster() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Master::obj ev;
};

class TestMasterExit : public TestMaster {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMasterExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMasterExit() {}

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

#endif // TEST_MASTER_H
