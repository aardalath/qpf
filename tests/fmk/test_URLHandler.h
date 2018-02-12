#ifndef TEST_URLHANDLER_H
#define TEST_URLHANDLER_H

#include "urlhdl.h"
#include "gtest/gtest.h"

//using namespace URLHandler;

namespace TestURLHandler {

class TestURLHandler : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestURLHandler() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestURLHandler() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // URLHandler::obj ev;
};

class TestURLHandlerExit : public TestURLHandler {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestURLHandlerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestURLHandlerExit() {}

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

#endif // TEST_URLHANDLER_H
