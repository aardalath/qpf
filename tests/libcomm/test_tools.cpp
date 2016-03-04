#include "test_tools.h"

#include <string>
#include <cmath>
#include <sys/time.h>
#include <time.h>

inline int getUSecs(struct timeval tv) {
    return (tv.tv_sec % 1440) * 1000000 + tv.tv_usec;
}

namespace LibCommTests {

TestTools::TestTools()
{
    // You can do set-up work for each test here.
}

TestTools::~TestTools()
{
    // You can do clean-up work that doesn't throw exceptions here.
}

// If the constructor and destructor are not enough for setting up
// and cleaning up each test, you can define the following methods:

void TestTools::SetUp()
{
    // Code here will be called immediately after the constructor (right
    // before each test).
}

void TestTools::TearDown()
{
    // Code here will be called immediately after each test (right
    // before the destructor).
}

TEST_F(TestTools, TtoStrConversion)
{
    // chars
    EXPECT_EQ(toStr<char>('0'), std::string("0"));
    EXPECT_EQ(toStr<char>(0267), std::string("\267"));
    EXPECT_EQ(toStr<unsigned char>(65U), std::string("A"));

    // shorts
    EXPECT_EQ(toStr<short>(32), std::string("32"));
    EXPECT_EQ(toStr<short>(-2678), std::string("-2678"));
    EXPECT_EQ(toStr<unsigned short>(-1), std::string("65535"));

    // int
    EXPECT_EQ(toStr<int>(129), std::string("129"));
    EXPECT_EQ(toStr<int>(-2678), std::string("-2678"));
    EXPECT_EQ(toStr<unsigned int>(-1), std::string("4294967295"));

    // long / long long
    EXPECT_EQ(toStr<long long>(129), std::string("129"));
    EXPECT_EQ(toStr<long long>(-2678), std::string("-2678"));
    EXPECT_EQ(toStr<unsigned long>(-1), std::string("18446744073709551615"));
    EXPECT_EQ(toStr<unsigned long long>(-1), std::string("18446744073709551615"));

    // float
    EXPECT_EQ(toStr<float>(-5.4), std::string("-5.4"));
    EXPECT_EQ(toStr<float>(1.000000000005), std::string("1"));
    EXPECT_EQ(toStr<float>(-0.00002), std::string("-2e-05"));

    // double
    EXPECT_EQ(toStr<double>(3.14), std::string("3.14"));
    EXPECT_EQ(toStr<double>(-1.0e-20), std::string("-1e-20"));

    // char *
    const char s[] = "Hi world!";
    char * s0 = const_cast<char *>(&(s[0]));
    EXPECT_EQ(toStr<char *>(s0), std::string(s));
    EXPECT_EQ(toStr<const char *>(s), std::string(s));

    // string
    std::string ss("Hola mundo!");
    EXPECT_EQ(toStr<std::string>(ss), ss);
}

TEST_F(TestTools, StrToTConversionChar)
{
    // char
    EXPECT_EQ(strTo<char>(std::string("a")), 'a');
    EXPECT_EQ(strTo<char>(std::string("This is a test")), 'T');
    EXPECT_EQ(strTo<char>(std::string("34")), 51); // '3'

    // shorts
    EXPECT_EQ(strTo<short>("32"), 32);
    EXPECT_EQ(strTo<unsigned short>("-1"), 65535);
    EXPECT_EQ(strTo<short>("65535"), 32767);

    // int
    EXPECT_EQ(strTo<int>("129"), 129);
    EXPECT_EQ(strTo<int>("-2678"), -2678);
    EXPECT_EQ(strTo<unsigned int>("4294967295"), -1);

    // long / long long
    EXPECT_EQ(strTo<long long>("129"), 129);
    EXPECT_EQ(strTo<long long>("-2678"), -2678);
    EXPECT_EQ(strTo<unsigned long>("18446744073709551615"), -1);
    EXPECT_EQ(strTo<unsigned long long>("18446744073709551615"), -1);

    // float
    EXPECT_FLOAT_EQ(strTo<float>("-5.4"), -5.4);
    EXPECT_FLOAT_EQ(strTo<float>("1"), 1.000000000005);
    EXPECT_FLOAT_EQ(strTo<float>("-2e-05"), -0.00002);

    // double
    EXPECT_FLOAT_EQ(strTo<double>("3.14"), 3.14);
    EXPECT_FLOAT_EQ(strTo<double>("-1e-20"), -1.0e-20);

    // string
    std::string ss("Hola mundo!");
    EXPECT_EQ(strTo<std::string>(ss), std::string("Hola"));
}

TEST_F(TestTools, WaitForHeartBeat)
{
    int secMsec[][2] = {{0,     100},
                        {0,    1000},
                        {0,   50000},
                        {1,  200300}
                       // {2, 1500000},
                       // {3,   50000},
                       // {1, 3000000}
                       };

    struct timeval tv1, tv2;
    for (int i = 0; i < 4; ++i) {
        int nusecs = secMsec[i][0] * 1000000 + secMsec[i][1];
        waitForHeartBeat(secMsec[i][0], secMsec[i][1]);
        (void)gettimeofday(&tv1, 0);
        waitForHeartBeat(secMsec[i][0], secMsec[i][1]);
        (void)gettimeofday(&tv2, 0);
        int usecs1 = getUSecs(tv1);
        int usecs2 = getUSecs(tv2);
        EXPECT_NEAR((usecs2 - usecs1), nusecs, 200);
    }
}

TEST_F(TestTools, SplitString)
{
    std::string s("This,is,a,,comma,separated,tokens,list,,");
    std::vector<std::string> tokens1, tokens2;

    tokens1 = split(s, ',');
    split(s, ',', tokens2);
    bool vectorsAreEqual = (tokens1 == tokens2);
    EXPECT_EQ(vectorsAreEqual, true);
    EXPECT_EQ(tokens1.size(), 9);
    EXPECT_EQ(tokens1[3], "");
}

TEST_F(TestTools, ReplaceAll)
{
    std::string s1("This is a test string");
    std::string s2 = replaceAll(s1, "is", "us");

    EXPECT_EQ(s2, s1);
    EXPECT_EQ(s2, "Thus us a test string");

    std::string s3 = replaceAll(s1, "us ", "");
    EXPECT_EQ(s1, "Tha test string");
}

TEST_F(TestTools, GetYMD)
{
    int year, month, day;

    std::string s1("20150511");
    getYMD(const_cast<char*>(s1.c_str()), year, month, day);
    EXPECT_EQ(year,  2015);
    EXPECT_EQ(month,    5);
    EXPECT_EQ(day,     11);

    std::string s2("2015 3 1");
    getYMD(const_cast<char*>(s2.c_str()), year, month, day);
    EXPECT_EQ(year,  2015);
    EXPECT_EQ(month,    3);
    EXPECT_EQ(day,      1);

    std::string s3("Thisis03");
    getYMD(const_cast<char*>(s3.c_str()), year, month, day);
    EXPECT_EQ(year,     0);
    EXPECT_EQ(month,    0);
    EXPECT_EQ(day,      0);
}

TEST_F(TestTools, TimeTag)
{
    int year, month, day;
    int hour, min, sec;

    time_t tmt = time(0);
    std::string s(timeTag());
    getYMD(const_cast<char*>(s.c_str()), year, month, day);
    std::vector<std::string> tagSections;
    std::string ss(s);
    tagSections = split(ss, 'T');

    char dateSection[9];
    sprintf(dateSection, "%04d%02d%02d", year, month, day);
    EXPECT_EQ(tagSections.at(0), std::string(dateSection));

    sscanf(tagSections.at(1).c_str(), "%02d%02d%02d", &hour, &min, &sec);

    struct tm * tmv = localtime(&tmt);

    EXPECT_EQ(year,  tmv->tm_year + 1900);
    EXPECT_EQ(month, tmv->tm_mon + 1);
    EXPECT_EQ(day,   tmv->tm_mday);
    EXPECT_EQ(hour,  tmv->tm_hour);
    EXPECT_EQ(min,   tmv->tm_min);
    EXPECT_EQ(sec,   tmv->tm_sec);
}

TEST_F(TestTools, WaitUntilNextSecond)
{
    time_t tmt1 = time(0);
    waitUntilNextSecond();
    time_t tmt2 = time(0);
    EXPECT_NEAR(tmt1, tmt2, 1);
}

}
