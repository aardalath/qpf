#ifndef TEST_MSGBODYINDATA_H
#define TEST_MSGBODYINDATA_H

#include "message.h"
#include "gtest/gtest.h"

//using namespace MsgBodyINDATA;

namespace TestMsgBodyINDATA {

class TestMsgBodyINDATA : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgBodyINDATA() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgBodyINDATA() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // MsgBodyINDATA::obj ev;
};

class TestMsgBodyINDATAExit : public TestMsgBodyINDATA {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgBodyINDATAExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgBodyINDATAExit() {}

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

#endif // TEST_MSGBODYINDATA_H
