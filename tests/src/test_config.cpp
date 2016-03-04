#include "test_config.h"

namespace TestQPF {

TEST_F(TestConfiguration, applyNewConfig) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, Configuration) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getCfgInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getConnectionsForNode) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getEnvVar) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getGeneralInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getHMINodeName) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getMachine) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNode) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNodeByName) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNumMachines) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNumNodes) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNumOrchRules) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getNumProcs) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getOrchRule) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getProc) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, getProductTypes) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, processConfiguration) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, readConfigurationFromDB) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, readConfigurationFromFile) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, reset) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, saveConfigurationToDB) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, setConfigFile) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestConfiguration, setLastAccess) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestConfigurationExit, TestConfiguration_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
