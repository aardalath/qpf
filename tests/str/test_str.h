#ifndef TEST_PKGSTR_H
#define TEST_PKGSTR_H

#include "str.h"
#include "gtest/gtest.h"

//using namespace PkgStr;

namespace TestPkgStr {

class TestPkgStr : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestPkgStr() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestPkgStr() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // PkgStr::obj ev;
};

class TestPkgStrExit : public TestPkgStr {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestPkgStrExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestPkgStrExit() {}

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

#endif // TEST_PKGSTR_H
