#include "test_datatypes.h"

namespace TestQPF {

TEST_F(TestJsonStruct, getData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestJsonStruct, getDataString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestJsonStruct, JsonStruct) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestJsonStruct, opAssign) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestJsonStruct, setData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestJsonStruct, setDataString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessageData, assign) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_DATA_Exchange, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_DATA_Exchange, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessageData, MessageData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessageData, reset) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessageHeader, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessageHeader, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_INDATA, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_INDATA, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_TASK_Processing, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage_TASK_Processing, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestMessage, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestParameterList, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestParameterList, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductCollection, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductCollection, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductList, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductList, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductMetadata, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestProductMetadata, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestStringList, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestStringList, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgentInfo, TaskAgentInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgentInfo, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskAgentInfo, toFields) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskInfo, TaskInfo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskInfo, toData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestTaskInfo, toFields) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestJsonStructExit, TestJsonStruct_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessageExit, TestMessage_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessageDataExit, TestMessageData_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessage_DATA_ExchangeExit, TestMessage_DATA_Exchange_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessageHeaderExit, TestMessageHeader_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessage_INDATAExit, TestMessage_INDATA_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestMessage_TASK_ProcessingExit, TestMessage_TASK_Processing_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestParameterListExit, TestParameterList_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestProductCollectionExit, TestProductCollection_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestProductListExit, TestProductList_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestProductMetadataExit, TestProductMetadata_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestStringListExit, TestStringList_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestTaskAgentInfoExit, TestTaskAgentInfo_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestTaskInfoExit, TestTaskInfo_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
