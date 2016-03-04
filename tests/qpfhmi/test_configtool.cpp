#include "test_configtool.h"

namespace TestQPF {

TEST_F(TestConfigTool, ConfigTool) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestConfigTool, readConfig) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestConfigToolExit, TestConfigTool_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
