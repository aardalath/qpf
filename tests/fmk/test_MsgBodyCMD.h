#ifndef TEST_MSGBODYCMD_H
#define TEST_MSGBODYCMD_H

#include "message.h"
#include "gtest/gtest.h"

//using namespace MsgBodyCMD;

namespace TestMsgBodyCMD {

class TestMsgBodyCMD : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgBodyCMD() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgBodyCMD() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // MsgBodyCMD::obj ev;
};

class TestMsgBodyCMDExit : public TestMsgBodyCMD {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgBodyCMDExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgBodyCMDExit() {}

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

#endif // TEST_MSGBODYCMD_H
