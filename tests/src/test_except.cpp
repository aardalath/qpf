#include "test_except.h"

namespace TestQPF {

TEST_F(TestError, Error) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestException, code) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestException, Exception) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestException, what) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestLogicException, LogicException) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestRuntimeException, RuntimeException) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestErrorExit, TestError_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestExceptionExit, TestException_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestLogicExceptionExit, TestLogicException_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

TEST_F(TestRuntimeExceptionExit, TestRuntimeException_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
