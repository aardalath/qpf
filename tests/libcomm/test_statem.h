#ifndef TestStateMachine_H
#define TestStateMachine_H

#include "statem.h"
#include "log.h"
#include "gtest/gtest.h"

using namespace LibComm;

namespace LibCommTests {

class TestStateMachine : public ::testing::Test {

protected:
    class MyStateMachine : public StateMachine {
    public:
        MyStateMachine() {
            Log::defineLogSystem("statem", "/dev/null");
            Log::setConsoleOutput(true);
        }

        inline void defState(int aState, std::string name)
        { defineState(aState, name); }

        inline void defValidTransition(int fromState, int toState)
        { defineValidTransition(fromState, toState); }
    };

    enum StateNums { ZERO = 0, ONE = 10, TWO = 20 };

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestStateMachine();

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestStateMachine();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

    // Objects declared here can be used by all tests in the test case for Foo.
    MyStateMachine sm;
};

}

#endif // TestStateMachine_H
