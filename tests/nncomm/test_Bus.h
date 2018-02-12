#ifndef TEST_BUS_H
#define TEST_BUS_H

#include "bus.h"
#include "gtest/gtest.h"

//using namespace Bus;

namespace TestBus {

class TestBus : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestBus() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestBus() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Bus::obj ev;
};

class TestBusExit : public TestBus {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestBusExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestBusExit() {}

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

#endif // TEST_BUS_H
