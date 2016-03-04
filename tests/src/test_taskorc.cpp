#include "test_taskorc.h"

namespace TestQPF {

TEST_F(TestTaskOrchestrator, checkRulesForProductType) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, defineOrchestrationParams) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, fromRunningToOperational) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, processINDATA) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, sendTaskProcMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskOrchestrator, TaskOrchestrator) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestTaskOrchestratorExit, TestTaskOrchestrator_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
