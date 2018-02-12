#ifndef TEST_DATAMNG_H
#define TEST_DATAMNG_H

#include "datamng.h"
#include "gtest/gtest.h"

//using namespace DataMng;

namespace TestDataMng {

class TestDataMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDataMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDataMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // DataMng::obj ev;
};

class TestDataMngExit : public TestDataMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDataMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDataMngExit() {}

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

#endif // TEST_DATAMNG_H
