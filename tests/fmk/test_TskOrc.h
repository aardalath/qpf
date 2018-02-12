#ifndef TEST_TSKORC_H
#define TEST_TSKORC_H

#include "tskorc.h"
#include "gtest/gtest.h"

//using namespace TskOrc;

namespace TestTskOrc {

class TestTskOrc : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskOrc() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskOrc() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // TskOrc::obj ev;
};

class TestTskOrcExit : public TestTskOrc {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskOrcExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskOrcExit() {}

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

#endif // TEST_TSKORC_H
