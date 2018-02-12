#ifndef TEST_PAIR_H
#define TEST_PAIR_H

#include "pair.h"
#include "gtest/gtest.h"

//using namespace Pair;

namespace TestPair {

class TestPair : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestPair() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestPair() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Pair::obj ev;
};

class TestPairExit : public TestPair {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestPairExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestPairExit() {}

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

#endif // TEST_PAIR_H
