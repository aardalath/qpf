#include "test_hmipxy.h"

namespace TestQPF {

TEST_F(TestHMIProxy, concurrentRun) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, fromOperationalToRunning) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, fromRunningToOff) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, getStartSignal) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, getTaskResInfo) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, HMIProxy) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, isThereTaskResInfo) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, processTASK_RES) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, registerTaskRes) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, requestStop) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestHMIProxy, sendInData) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestHMIProxyExit, TestHMIProxy_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
