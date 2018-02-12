#ifndef TEST_PIPE_STREAMBUF_H
#define TEST_PIPE_STREAMBUF_H

#include "process.h"
#include "gtest/gtest.h"

//using namespace pipe_streambuf;

namespace Testpipe_streambuf {

class Testpipe_streambuf : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_streambuf() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_streambuf() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // pipe_streambuf::obj ev;
};

class Testpipe_streambufExit : public Testpipe_streambuf {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_streambufExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_streambufExit() {}

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

#endif // TEST_PIPE_STREAMBUF_H
