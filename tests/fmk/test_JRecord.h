#ifndef TEST_JRECORD_H
#define TEST_JRECORD_H

#include "datatypes.h"
#include "gtest/gtest.h"

//using namespace JRecord;

namespace TestJRecord {

class TestJRecord : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestJRecord() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestJRecord() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // JRecord::obj ev;
};

class TestJRecordExit : public TestJRecord {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestJRecordExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestJRecordExit() {}

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

#endif // TEST_JRECORD_H
