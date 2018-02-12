#ifndef TEST_EXCEPTION_H
#define TEST_EXCEPTION_H

#include "except.h"
#include "gtest/gtest.h"

//using namespace Exception;

namespace TestException {

class TestException : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestException() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestException() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Exception::obj ev;
};

class TestExceptionExit : public TestException {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestExceptionExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestExceptionExit() {}

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

#endif // TEST_EXCEPTION_H
