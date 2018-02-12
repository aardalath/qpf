#ifndef TEST_DOCKERMNG_H
#define TEST_DOCKERMNG_H

#include "dckmng.h"
#include "gtest/gtest.h"

//using namespace DockerMng;

namespace TestDockerMng {

class TestDockerMng : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDockerMng() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDockerMng() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // DockerMng::obj ev;
};

class TestDockerMngExit : public TestDockerMng {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDockerMngExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDockerMngExit() {}

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

#endif // TEST_DOCKERMNG_H
