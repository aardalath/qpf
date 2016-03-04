#include "test_uuid.h"

namespace TestQPF {

TEST_F(TestUUID, asLowerString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, asString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, asUpperString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, clear) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, compare) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, copy) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, generate) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, generate_random) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, generate_time) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, generate_time_safe) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, is_null) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, parse) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, time) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, type) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, unparse) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, unparse_lower) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, unparse_upper) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, uuid) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, UUID) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestUUID, variant) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestUUIDExit, TestUUID_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
