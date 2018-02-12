#ifndef TEST_SERVICEMNG_H
#define TEST_SERVICEMNG_H

#include "srvmng.h"
#include "gtest/gtest.h"

//using namespace ServiceMng;

namespace TestServiceMng {

class TestServiceMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestServiceMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestServiceMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // ServiceMng::obj ev;
};

class TestServiceMngExit : public TestServiceMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestServiceMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestServiceMngExit() {}

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

#endif // TEST_SERVICEMNG_H
