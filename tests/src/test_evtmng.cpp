#include "test_evtmng.h"

namespace TestQPF {

TEST_F(TestEventManager, EventManager) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, fromInitialisedToRunning) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, fromOperationalToRunning) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, go) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, processINDATA) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, processMONIT_INFO) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestEventManager, processTASK_RES) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestEventManagerExit, TestEventManager_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
