#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include "timer.h"
#include "gtest/gtest.h"

//using namespace Timer;

namespace TestTimer {

class TestTimer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTimer() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTimer() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Timer::obj ev;
};

class TestTimerExit : public TestTimer {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTimerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTimerExit() {}

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

#endif // TEST_TIMER_H
