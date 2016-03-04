#include "test_deployer.h"

namespace TestQPF {

TEST_F(TestDeployer, cleanUp) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, Deployer) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, existsDir) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, fexists) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, getConfigFileName) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, getConfigHandler) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, launchPeerNodes) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, mustLaunchHMI) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, processCmdLineOpts) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, readConfig) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, removeOldFiles) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, run) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, start) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestDeployer, waitingForGoAhead) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestDeployerExit, TestDeployer_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
