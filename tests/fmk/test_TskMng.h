#ifndef TEST_TSKMNG_H
#define TEST_TSKMNG_H

#include "tskmng.h"
#include "gtest/gtest.h"

//using namespace TskMng;

namespace TestTskMng {

class TestTskMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // TskMng::obj ev;
};

class TestTskMngExit : public TestTskMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTskMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTskMngExit() {}

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

#endif // TEST_TSKMNG_H
