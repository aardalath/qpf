#include "test_app.h"

namespace TestQPF {

TEST_F(TestApp, App) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestApp, getConfigFileName) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestApp, getConfigHandler) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestApp, mustLaunchHMI) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestApp, notify) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestAppExit, TestApp_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
