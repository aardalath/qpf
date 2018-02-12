#ifndef TEST_PIPE_OSTREAMBUF_H
#define TEST_PIPE_OSTREAMBUF_H

#include "process.h"
#include "gtest/gtest.h"

//using namespace pipe_ostreambuf;

namespace Testpipe_ostreambuf {

class Testpipe_ostreambuf : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_ostreambuf() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_ostreambuf() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // pipe_ostreambuf::obj ev;
};

class Testpipe_ostreambufExit : public Testpipe_ostreambuf {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_ostreambufExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_ostreambufExit() {}

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

#endif // TEST_PIPE_OSTREAMBUF_H
