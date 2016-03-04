#include "test_value.h"

namespace TestSDC {

TEST_F(TestValue, Value) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asArray) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asBool) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asDbl) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asDict) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asInt) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asSerialized) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, asString) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, begin) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, clear) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, deserialize) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, dump) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, end) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, getData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, getSizeOfData) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isArray) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isBool) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isDbl) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isDict) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isInt) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isNull) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, isStr) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, opAssign) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, opInStream) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, opSubscript) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, prettyFormat) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, readFrom) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, saveTo) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, serialize) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, set) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, setDefaultType) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, setType) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, size) {
    EXPECT_EQ(1, 2);
}

TEST_F(TestValue, type) {
    EXPECT_EQ(1, 2);
}


TEST_F(TestValueExit, TestValue_Exit) {
    EXPECT_EXIT({ exit(0); },
                ::testing::ExitedWithCode(1),
                "");
}

}
