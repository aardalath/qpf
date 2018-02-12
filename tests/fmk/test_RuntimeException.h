#ifndef TEST_RUNTIMEEXCEPTION_H
#define TEST_RUNTIMEEXCEPTION_H

#include "except.h"
#include "gtest/gtest.h"

//using namespace RuntimeException;

namespace TestRuntimeException {

class TestRuntimeException : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestRuntimeException() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRuntimeException() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // RuntimeException::obj ev;
};

class TestRuntimeExceptionExit : public TestRuntimeException {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestRuntimeExceptionExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRuntimeExceptionExit() {}

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

#endif // TEST_RUNTIMEEXCEPTION_H
