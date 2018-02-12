#ifndef TEST_CONTAINERMNG_H
#define TEST_CONTAINERMNG_H

#include "cntrmng.h"
#include "gtest/gtest.h"

//using namespace ContainerMng;

namespace TestContainerMng {

class TestContainerMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestContainerMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestContainerMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // ContainerMng::obj ev;
};

class TestContainerMngExit : public TestContainerMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestContainerMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestContainerMngExit() {}

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

#endif // TEST_CONTAINERMNG_H
