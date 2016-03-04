#include "test_datamng.h"

namespace TestQPF {

TEST_F(TestDataManager, archiveDSSnEAS) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, DataManager) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, fromOperationalToRunning) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, fromRunningToOff) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, fromRunningToOperational) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, getProductLatest) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, initializeDB) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, isProductAvailable) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, processINDATA) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, processTASK_PROC) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, processTASK_RES) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, saveProductsToDB) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, saveTaskToDB) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestDataManager, saveToDB) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestDataManagerExit, TestDataManager_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
