#include "test_component.h"

namespace TestQPF {

TEST_F(TestComponent, buildMsgHeader) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, buildMsgINDATA) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, buildMsgTASKPROC) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, canProcessMessage) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, Component) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, convertTo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, fromInitialisedToRunning) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, fromOperationalToRunning) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, fromRunningToOperational) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, init) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, is) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, process) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, registerMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, run) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, sendLogPacketAsDataInfoMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, setForwardTo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, setHeartBeatPeriod) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestComponent, writeToFile) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestComponentExit, TestComponent_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
