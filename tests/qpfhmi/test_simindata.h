#ifndef TEST_SIMINDATA_H
#define TEST_SIMINDATA_H

#include "simindata.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestSimInData : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestSimInData() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestSimInData() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    SimInData o;
};

class TestSimInDataExit : public TestSimInData {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestSimInDataExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestSimInDataExit() {}

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

#endif // TEST_SIMINDATA_H
