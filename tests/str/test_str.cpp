#include "test_str.h"

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestPkgStr {

TEST_F(TestPkgStr, Test_tpl_toStr) {
    EXPECT_EQ(std::string("42"), str::toStr<int>(42));
    EXPECT_NE(std::string(" 42"), str::toStr<int>(42));
    EXPECT_NE(std::string("42 "), str::toStr<int>(42));

    EXPECT_EQ(std::string("2"), str::toStr<float>(10. / 5.));

    EXPECT_EQ(std::string("4"), str::toStr<double>(2. * 2.));
    EXPECT_EQ(std::string("2.5"), str::toStr<double>(5. / 2.));
    

    EXPECT_EQ(std::string("Hola, mundo!"), str::toStr<std::string>("Hola, mundo!"));
}

TEST_F(TestPkgStr, Test_tpl_strTo) {
    EXPECT_EQ(42, str::strTo<int>("42"));
    EXPECT_EQ(42, str::strTo<int>("42 "));
    EXPECT_EQ(42, str::strTo<int>(" 42"));

    EXPECT_EQ(5., str::strTo<float>(" 5 "));

    EXPECT_EQ((double)(4.), str::strTo<double>(" 0.4e1"));
    EXPECT_EQ((double)(2.25), str::strTo<double>(" 22.5E-1"));    

    EXPECT_EQ(std::string("Hola,"), str::strTo<std::string>("Hola, mundo!"));    
}

TEST_F(TestPkgStr, Test_split) {
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "4"}),
              str::split("1 2 3 4", ' '));
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "", "4"}),
              str::split("1 2 3  4", ' '));
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "", "", "4"}),
              str::split("1 2 3   4 ", ' '));
    EXPECT_EQ(std::vector<std::string>({"This", "is", "a", "test"}),
              str::split("This.is.a.test.", '.'));
    EXPECT_EQ(std::vector<std::string>({"", "This", "is", "a", "test"}),
              str::split(".This.is.a.test.", '.'));

    std::vector<std::string> tokens;

    str::split("1 2 3 4", ' ', tokens);
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "4"}), tokens);
    tokens.clear();
    str::split("1 2 3  4", ' ', tokens);
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "", "4"}), tokens);
    tokens.clear();
    str::split("1 2 3   4 ", ' ', tokens);
    EXPECT_EQ(std::vector<std::string>({"1", "2", "3", "", "", "4"}), tokens);
    tokens.clear();
    str::split("This.is.a.test.", '.', tokens);
    EXPECT_EQ(std::vector<std::string>({"This", "is", "a", "test"}), tokens);
    tokens.clear();
    str::split(".This.is.a.test.", '.', tokens);
    EXPECT_EQ(std::vector<std::string>({"", "This", "is", "a", "test"}), tokens);
}

TEST_F(TestPkgStr, Test_join) {
    EXPECT_EQ("1 2 3 4", 
              str::join(std::vector<std::string>({"1", "2", "3", "4"}), " "));
    EXPECT_EQ("1 2 3  4",
             str::join(std::vector<std::string>({"1", "2", "3", "", "4"}), " "));
    EXPECT_EQ("1 2 3   4",
             str::join(std::vector<std::string>({"1", "2", "3", "", "", "4"}), " "));
    EXPECT_EQ("This.is.a.test",
              str::join(std::vector<std::string>({"This", "is", "a", "test"}), "."));
    EXPECT_EQ("This.HAHA..HAHA.is.HAHA.a.HAHA.test", 
              str::join(std::vector<std::string>({"This", "", "is", "a", "test"}), ".HAHA."));   
}

TEST_F(TestPkgStr, Test_replaceAll) {
    
}

TEST_F(TestPkgStr, Test_mid) {
    
}

TEST_F(TestPkgStr, Test_rtrim) {
    
}

TEST_F(TestPkgStr, Test_ltrim) {
    
}

TEST_F(TestPkgStr, Test_trim) {
    
}

TEST_F(TestPkgStr, Test_quoted) {
    
}

TEST_F(TestPkgStr, Test_toUpper) {
    
}

TEST_F(TestPkgStr, Test_getExtension) {
    
}

TEST_F(TestPkgStr, Test_getBaseName) {
    
}

TEST_F(TestPkgStr, Test_getDirName) {
    
}

TEST_F(TestPkgStr, Test_tagToTimestamp) {
    
}
    
}

