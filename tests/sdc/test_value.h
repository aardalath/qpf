#ifndef TEST_VALUE_H
#define TEST_VALUE_H

#include "value.h"
#include "gtest/gtest.h"

using namespace SDC;

namespace TestSDC {

class TestValue : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestValue() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestValue() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    Value * o;
};

class TestValueExit : public TestValue {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestValueExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestValueExit() {}

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

#endif // TEST_VALUE_H
