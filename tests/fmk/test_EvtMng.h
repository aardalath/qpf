#ifndef TEST_EVTMNG_H
#define TEST_EVTMNG_H

#include "evtmng.h"
#include "gtest/gtest.h"

//using namespace EvtMng;

namespace TestEvtMng {

class TestEvtMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestEvtMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestEvtMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // EvtMng::obj ev;
};

class TestEvtMngExit : public TestEvtMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestEvtMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestEvtMngExit() {}

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

#endif // TEST_EVTMNG_H
