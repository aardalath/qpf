#include "test_UUID.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestUUID {

TEST_F(TestUUID, Test_UUID) {
    UUID u;
}

TEST_F(TestUUID, Test_clear) {
    // void clear();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_compare) {
    // bool compare(const UUID & uu2);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_copy) {
    // void copy(UUID & dst);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_generate) {
    // void generate();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_generate_random) {
    // void generate_random();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_generate_time) {
    // void generate_time();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_generate_time_safe) {
    // bool generate_time_safe();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_is_null) {
    // bool is_null();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_parse) {
    // bool parse(const char *in);
    // bool parse(std::string & in);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_unparse) {
    // void unparse(char *out);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_unparse_lower) {
    // void unparse_lower(char *out);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_unparse_upper) {
    // void unparse_upper(char *out);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_asString) {
    // std::string asString();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_asLowerString) {
    // std::string asLowerString();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_asUpperString) {
    // std::string asUpperString();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_time) {
    // time_t time(struct timeval *ret_tv);
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_type) {
    // int type();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_variant) {
    // int variant();
    // EXPECT_EQ();
}
TEST_F(TestUUID, Test_uuid) {
    // uuid_t & uuid();
    // EXPECT_EQ();
}

}   
