#ifndef TEST_HOSTINFO_H
#define TEST_HOSTINFO_H

#include "hostinfo.h"
#include "gtest/gtest.h"

//using namespace HostInfo;

namespace TestHostInfo {

class TestHostInfo : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHostInfo() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHostInfo() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // HostInfo::obj ev;
};

class TestHostInfoExit : public TestHostInfo {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHostInfoExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHostInfoExit() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
};

}

#endif // TEST_HOSTINFO_H
