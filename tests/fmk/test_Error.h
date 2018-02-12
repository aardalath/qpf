#ifndef TEST_ERROR_H
#define TEST_ERROR_H

#include "except.h"
#include "gtest/gtest.h"

//using namespace Error;

namespace TestError {

class TestError : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestError() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestError() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Error::obj ev;
};

class TestErrorExit : public TestError {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestErrorExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestErrorExit() {}

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

#endif // TEST_ERROR_H
