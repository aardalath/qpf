#include "test_valueiojsoncpp.h"

namespace TestSDC {

TEST_F(TestValueIOJsonCpp, deserialize) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValueIOJsonCpp, exportData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValueIOJsonCpp, importData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValueIOJsonCpp, readFrom) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValueIOJsonCpp, saveTo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValueIOJsonCpp, serialize) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestValueIOJsonCppExit, TestValueIOJsonCpp_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
