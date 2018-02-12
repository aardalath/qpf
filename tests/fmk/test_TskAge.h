#ifndef TEST_TSKAGE_H
#define TEST_TSKAGE_H

#include "tskage.h"
#include "gtest/gtest.h"

//using namespace TskAge;

namespace TestTskAge {

class TestTskAge : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskAge() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskAge() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // TskAge::obj ev;
};

class TestTskAgeExit : public TestTskAge {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskAgeExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskAgeExit() {}

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

#endif // TEST_TSKAGE_H
