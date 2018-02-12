#ifndef TEST_MESSAGEBASE_H
#define TEST_MESSAGEBASE_H

#include "message.h"
#include "gtest/gtest.h"

//using namespace MessageBase;

namespace TestMessageBase {

class TestMessageBase : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMessageBase() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMessageBase() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // MessageBase::obj ev;
};

class TestMessageBaseExit : public TestMessageBase {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMessageBaseExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMessageBaseExit() {}

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

#endif // TEST_MESSAGEBASE_H
