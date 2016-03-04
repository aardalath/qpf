#include "test_taskmng.h"

namespace TestQPF {

TEST_F(TestTaskManager, exeRule) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, fromRunningToOperational) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, processMONIT_INFO) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, processTASK_PROC) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, processTASK_RES) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, selectAgent) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, sendMonitInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, sendTaskAgMsg) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, sendTaskRes) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskManager, TaskManager) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestTaskManagerExit, TestTaskManager_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
