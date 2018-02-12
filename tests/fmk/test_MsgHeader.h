#ifndef TEST_MSGHEADER_H
#define TEST_MSGHEADER_H

#include "message.h"
#include "gtest/gtest.h"

//using namespace MsgHeader;

namespace TestMsgHeader {

class TestMsgHeader : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgHeader() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgHeader() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // MsgHeader::obj ev;
};

class TestMsgHeaderExit : public TestMsgHeader {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestMsgHeaderExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestMsgHeaderExit() {}

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

#endif // TEST_MSGHEADER_H
