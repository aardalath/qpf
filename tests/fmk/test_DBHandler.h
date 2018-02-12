#ifndef TEST_DBHANDLER_H
#define TEST_DBHANDLER_H

#include "dbhdl.h"
#include "gtest/gtest.h"

//using namespace DBHandler;

namespace TestDBHandler {

class TestDBHandler : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDBHandler() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDBHandler() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // DBHandler::obj ev;
};

class TestDBHandlerExit : public TestDBHandler {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDBHandlerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDBHandlerExit() {}

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

#endif // TEST_DBHANDLER_H
