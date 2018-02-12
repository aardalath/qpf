#ifndef TEST_FILENAMESPEC_H
#define TEST_FILENAMESPEC_H

#include "filenamespec.h"
#include "gtest/gtest.h"

//using namespace FileNameSpec;

namespace TestFileNameSpec {

class TestFileNameSpec : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestFileNameSpec() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestFileNameSpec() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // FileNameSpec::obj ev;
};

class TestFileNameSpecExit : public TestFileNameSpec {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestFileNameSpecExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestFileNameSpecExit() {}

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

#endif // TEST_FILENAMESPEC_H
