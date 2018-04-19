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
    EXPECT_EQ("", 
              str::join(std::vector<std::string>({}), " "));
    EXPECT_EQ("1", 
              str::join(std::vector<std::string>({"1"}), " "));
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
    std::string s("12 34 56 78 91 23 45");
    EXPECT_EQ("1 34 56 78 91 3 45", str::replaceAll(s, "2", ""));
    s.assign("this is a test");
    EXPECT_EQ("this NOT is NOT a test", str::replaceAll(s, "is", "is NOT"));
    EXPECT_EQ(s, str::replaceAll(s, "B", "X"));
}

TEST_F(TestPkgStr, Test_mid) {
    std::string s("This is a test string");
    EXPECT_EQ("This", str::mid(s, 0, 4));
    EXPECT_EXIT(str::mid(s, 50, 4), ::testing::ExitedWithCode(255),
                "ERROR: s:'This is a test string', from:50, howmany:4");
    EXPECT_EXIT(str::mid(s, -1, 4), ::testing::ExitedWithCode(255),
                "ERROR: s:'This is a test string', from:-1, howmany:4");
    
    EXPECT_EQ("test string", str::mid(s, 10));
    EXPECT_EXIT(str::mid(s, 50), ::testing::ExitedWithCode(255),
                "ERROR: s:'This is a test string', from:50");
}

TEST_F(TestPkgStr, Test_rtrim) {
    std::string s("  This is a test string----   ");
    EXPECT_EQ("  This is a test string----", str::rtrim(s));
    EXPECT_EQ("  This is a test string", str::rtrim(s, "-"));
    EXPECT_EQ("  This is a te", str::rtrim(s, "ingrst "));
    EXPECT_EQ("  This is a te", str::rtrim(s));
}

TEST_F(TestPkgStr, Test_ltrim) {    
    std::string s("  ----This is a test string   ");
    EXPECT_EQ("----This is a test string   ", str::ltrim(s));
    EXPECT_EQ("This is a test string   ", str::ltrim(s, "-"));
    EXPECT_EQ("a test string   ", str::ltrim(s, "ishT "));
    EXPECT_EQ("a test string   ", str::ltrim(s));
}

TEST_F(TestPkgStr, Test_trim) {
    std::string s("  ----This is a test string   ");
    EXPECT_EQ("----This is a test string", str::trim(s));
    EXPECT_EQ("This is a test string", str::trim(s, "-"));
    EXPECT_EQ("a test str", str::trim(s, "ishngT "));
    EXPECT_EQ("a test str", str::trim(s));    
}

TEST_F(TestPkgStr, Test_quoted) {
    EXPECT_EQ("'quoted'", str::quoted("quoted"));
    EXPECT_EQ("'quoted'", str::quoted("quoted", '\''));
    EXPECT_EQ("\"quoted\"", str::quoted("quoted", '"'));
    EXPECT_EQ("*quoted*", str::quoted("quoted", '*'));
    EXPECT_EQ("  ", str::quoted("", ' '));
}

TEST_F(TestPkgStr, Test_toUpper) {
    std::string s;
    s = "hello"; str::toUpper(s); EXPECT_EQ("HELLO", s);
    s = "Hello"; str::toUpper(s); EXPECT_EQ("HELLO", s);
    s = "Hello boY"; str::toUpper(s); EXPECT_EQ("HELLO BOY", s);
    s = "áñç"; str::toUpper(s); EXPECT_EQ("áñç", s);
    s = ""; str::toUpper(s); EXPECT_EQ("", s);    
}

TEST_F(TestPkgStr, Test_getExtension) {
    EXPECT_EQ("fits", str::getExtension("myfile.fits"));
    EXPECT_EQ("gz", str::getExtension("myfile.tar.gz"));
    EXPECT_EQ("", str::getExtension("myfile"));
}

TEST_F(TestPkgStr, Test_getBaseName) {
    EXPECT_EQ("myfile.fits", str::getBaseName(std::string("/a/file/myfile.fits")));
    EXPECT_EQ("myfile.tar.gz", str::getBaseName(std::string("./b/myfile.tar.gz")));
    EXPECT_EQ("folder", str::getBaseName(std::string("/a/folder/")));    
}

TEST_F(TestPkgStr, Test_getDirName) {
    EXPECT_EQ("/a/file", str::getDirName(std::string("/a/file/myfile.fits")));
    EXPECT_EQ("./b", str::getDirName(std::string("./b/myfile.tar.gz")));
    EXPECT_EQ("/a", str::getDirName(std::string("/a/folder/")));    
}

TEST_F(TestPkgStr, Test_tagToTimestamp) {
    EXPECT_EQ("2023-05-11 12:13:14", str::tagToTimestamp("20230511T121314"));
    EXPECT_EQ("2023-05-11 12:13:14.5", str::tagToTimestamp("20230511T121314.5"));
    EXPECT_EQ("2023-05-11 12:13:14", str::tagToTimestamp("20230511T121314.0", false));
    EXPECT_EQ("2023-05-11 12:13:14.0", str::tagToTimestamp("20230511T121314.0"));
    EXPECT_EQ("2001-01-01 00:00:00", str::tagToTimestamp("", false));
    EXPECT_EQ("2001-01-01 00:00:00.0", str::tagToTimestamp(""));
}
    
}

