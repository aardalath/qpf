#include "test_cfginfo.h"

namespace TestQPF {

TEST_F(TestConfigurationInfo, clear) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfigurationInfo, ConfigurationInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfigurationInfo, data) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestConfigurationInfoExit, TestConfigurationInfo_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
