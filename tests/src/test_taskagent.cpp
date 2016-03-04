#include "test_taskagent.h"

namespace TestQPF {

TEST_F(TestTaskAgent, checkProcessingElements) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, execAdditonalLoopTasks) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, fromRunningToOperational) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, procElemFinished) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, processCMD) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, processTASK_PROC) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, sendMonitInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, sendTaskResMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgent, TaskAgent) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestTaskAgentExit, TestTaskAgent_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
