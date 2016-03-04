#include "test_filenamespec.h"

namespace TestQPF {

TEST_F(TestFileNameSpec, buildProductId) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, FileNameSpec) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, parseFileName) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, setAssignations) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, setFileNameSpec) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, setProductIdTpl) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileNameSpec, setRegEx) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestFileNameSpecExit, TestFileNameSpec_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
