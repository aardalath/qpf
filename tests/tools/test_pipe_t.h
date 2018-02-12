#ifndef TEST_PIPE_T_H
#define TEST_PIPE_T_H

#include "process.h"
#include "gtest/gtest.h"

//using namespace pipe_t;

namespace Testpipe_t {

class Testpipe_t : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_t() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_t() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // pipe_t::obj ev;
};

class Testpipe_tExit : public Testpipe_t {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipe_tExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipe_tExit() {}

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

#endif // TEST_PIPE_T_H
