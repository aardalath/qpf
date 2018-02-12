#ifndef TEST_STATEMACHINE_H
#define TEST_STATEMACHINE_H

#include "sm.h"
#include "gtest/gtest.h"

//using namespace StateMachine;

namespace TestStateMachine {

class TestStateMachine : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestStateMachine() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestStateMachine() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // StateMachine::obj ev;
};

class TestStateMachineExit : public TestStateMachine {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestStateMachineExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestStateMachineExit() {}

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

#endif // TEST_STATEMACHINE_H
