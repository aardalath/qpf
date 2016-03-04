#ifndef TEST_UUID_H
#define TEST_UUID_H

#include "uuid.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestUUID : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestUUID() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestUUID() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    UUID * o;
};

class TestUUIDExit : public TestUUID {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestUUIDExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestUUIDExit() {}

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

#endif // TEST_UUID_H
