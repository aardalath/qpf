#include "test_frmagentstatus.h"

namespace TestQPF {

TEST_F(TestFrmAgentStatus, FrmAgentStatus) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestFrmAgentStatus, updateInfo) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestFrmAgentStatusExit, TestFrmAgentStatus_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
