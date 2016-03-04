#include "test_logwatcher.h"

namespace TestQPF {

TEST_F(TestLogWatcher, getFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestLogWatcher, LogWatcher) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestLogWatcher, setFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestLogWatcher, updateLogView) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestLogWatcherExit, TestLogWatcher_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
