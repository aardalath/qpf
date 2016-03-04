#include "test_statem.h"

#include <string>

namespace LibCommTests {

TestStateMachine::TestStateMachine()
{
    // You can do set-up work for each test here.
}

TestStateMachine::~TestStateMachine()
{
    // You can do clean-up work that doesn't throw exceptions here.
}

// If the constructor and destructor are not enough for setting up
// and cleaning up each test, you can define the following methods:

void TestStateMachine::SetUp()
{    
    // Code here will be called immediately after the constructor (right
    // before each test).
    sm.defState(ZERO, "ZERO");
    sm.defState(ONE,  "ONE");
    sm.defState(TWO,  "TWO");

    sm.defValidTransition(ZERO, ONE);
    sm.defValidTransition(ONE,  TWO);
    sm.defValidTransition(TWO,  ZERO);
}

void TestStateMachine::TearDown()
{
    // Code here will be called immediately after each test (right
    // before the destructor).
}

TEST_F(TestStateMachine, CanGetStateNames) {
    EXPECT_EQ(sm.getStateName(ZERO), std::string("ZERO"));
    EXPECT_EQ(sm.getStateName(ONE), std::string("ONE"));
    EXPECT_EQ(sm.getStateName(TWO), std::string("TWO"));
}

TEST_F(TestStateMachine, CanRedefineStateNames) {
    sm.defState(ONE, "FIRST");
    sm.defState(TWO, "SECOND");
    EXPECT_EQ(sm.getStateName(ONE), std::string("FIRST"));
    EXPECT_EQ(sm.getStateName(TWO), std::string("SECOND"));
    EXPECT_EQ(sm.getStateName(99), std::string(""));
}

TEST_F(TestStateMachine, HandlesValidInvalidStateTransits) {
    sm.setInvalidTransit(Log::ERROR);

    EXPECT_TRUE(sm.transitTo(ONE));
    EXPECT_EQ(sm.getState(), ONE);
    EXPECT_TRUE(sm.transitTo(TWO));
    EXPECT_EQ(sm.getState(), TWO);
    EXPECT_TRUE(sm.transitTo(ZERO));
    EXPECT_EQ(sm.getState(), ZERO);

    EXPECT_FALSE(sm.transitTo(TWO));
    EXPECT_EQ(sm.getState(), ZERO);

    EXPECT_TRUE(sm.transitTo(ONE));
    EXPECT_FALSE(sm.transitTo(ZERO));
    EXPECT_EQ(sm.getState(), ONE);

    EXPECT_TRUE(sm.transitTo(TWO));
    EXPECT_FALSE(sm.transitTo(ONE));
    EXPECT_EQ(sm.getState(), TWO);
}

}

