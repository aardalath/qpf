#include "test_filetransfer.h"

namespace TestQPF {

TEST_F(TestFileTransfer, download) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, FileTransfer) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, mode) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, setMode) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, str) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, writeData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestFileTransfer, writeDataImpl) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestFileTransferExit, TestFileTransfer_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
