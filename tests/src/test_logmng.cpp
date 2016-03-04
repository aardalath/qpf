#include "test_logmng.h"

namespace TestQPF {

TEST_F(TestLogManager, execAdditonalLoopTasks) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestLogManager, LogManager) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestLogManager, processDATA_INFO) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestLogManagerExit, TestLogManager_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
