#ifndef TEST_VOSPACEHANDLER_H
#define TEST_VOSPACEHANDLER_H

#include "voshdl.h"
#include "gtest/gtest.h"

//using namespace VOSpaceHandler;

namespace TestVOSpaceHandler {

class TestVOSpaceHandler : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestVOSpaceHandler() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestVOSpaceHandler() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // VOSpaceHandler::obj ev;
};

class TestVOSpaceHandlerExit : public TestVOSpaceHandler {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestVOSpaceHandlerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestVOSpaceHandlerExit() {}

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

#endif // TEST_VOSPACEHANDLER_H
