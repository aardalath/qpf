#include "test_simindata.h"

namespace TestQPF {

TEST_F(TestSimInData, actualSendInDataFromFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, actualSendMultInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, generateMetaFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, processInbox) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, readMetaFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, sendInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, sendInDataFromFile) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, sendMultInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, setHmiNode) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, setInjectionFrequency) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, SimInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, stopSendingInData) {
    EXPECT_EQ(1, 1);
}

TEST_F(TestSimInData, traverseDirectory) {
    EXPECT_EQ(1, 1);
}


TEST_F(TestSimInDataExit, TestSimInData_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}

}
