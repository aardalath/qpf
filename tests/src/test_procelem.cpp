#include "test_procelem.h"

namespace TestQPF {

TEST_F(TestProcessingElement, cleanup) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, configureProcElem) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, end) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, exec) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, executeProcessor) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, forkProcess) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, getExpandedDateTime) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, getMonitoringInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, getSimplifiedDateTime) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, getStatus) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, getTask) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, goIdle) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, initTaskInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, monitorProcElemLoop) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, obtainProcElemId) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, ProcessingElement) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, retrieveOutputProducts) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, sendUpdatedInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, setTaskInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, start) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProcessingElement, updateProgress) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestProcessingElementExit, TestProcessingElement_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
