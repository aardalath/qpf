#ifndef TEST_SYNCHRONIZER_H
#define TEST_SYNCHRONIZER_H

#include "sync.h"
#include "gtest/gtest.h"

//using namespace Synchronizer;

namespace TestSynchronizer {

class TestSynchronizer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestSynchronizer() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestSynchronizer() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Synchronizer::obj ev;
};

class TestSynchronizerExit : public TestSynchronizer {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestSynchronizerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestSynchronizerExit() {}

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

#endif // TEST_SYNCHRONIZER_H
