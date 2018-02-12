#ifndef TEST_ALERT_H
#define TEST_ALERT_H

#include "alert.h"
#include "gtest/gtest.h"

//using namespace Alert;

namespace TestAlert {

class TestAlert : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestAlert() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestAlert() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Alert::obj ev;
};

class TestAlertExit : public TestAlert {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestAlertExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestAlertExit() {}

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

#endif // TEST_ALERT_H
