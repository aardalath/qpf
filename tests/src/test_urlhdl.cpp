#include "test_urlhdl.h"

namespace TestQPF {

TEST_F(TestURLHandler, copyfile) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromArchive2Local) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromExternal2Inbox) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromFolder2Inbox) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromInbox2Local) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromLocal2Archive) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromLocal2Shared) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromProcessing2Shared) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromShared2Local) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, fromShared2Processing) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, relocate) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, setProcElemRunDir) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestURLHandler, URLHandler) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestURLHandlerExit, TestURLHandler_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
