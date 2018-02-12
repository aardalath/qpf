#ifndef TEST_JVALUE_H
#define TEST_JVALUE_H

#include "datatypes.h"
#include "gtest/gtest.h"

//using namespace JValue;

namespace TestJValue {

class TestJValue : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestJValue() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestJValue() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // JValue::obj ev;
};

class TestJValueExit : public TestJValue {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestJValueExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestJValueExit() {}

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

#endif // TEST_JVALUE_H
