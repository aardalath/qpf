#ifndef TEST_LOGICEXCEPTION_H
#define TEST_LOGICEXCEPTION_H

#include "except.h"
#include "gtest/gtest.h"

//using namespace LogicException;

namespace TestLogicException {

class TestLogicException : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLogicException() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLogicException() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // LogicException::obj ev;
};

class TestLogicExceptionExit : public TestLogicException {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestLogicExceptionExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestLogicExceptionExit() {}

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

#endif // TEST_LOGICEXCEPTION_H
