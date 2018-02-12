#ifndef TEST_GENERICFILEHANDLER_H
#define TEST_GENERICFILEHANDLER_H

#include "genfhdl.h"
#include "gtest/gtest.h"

//using namespace GenericFileHandler;

namespace TestGenericFileHandler {

class TestGenericFileHandler : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestGenericFileHandler() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestGenericFileHandler() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // GenericFileHandler::obj ev;
};

class TestGenericFileHandlerExit : public TestGenericFileHandler {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestGenericFileHandlerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestGenericFileHandlerExit() {}

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

#endif // TEST_GENERICFILEHANDLER_H
