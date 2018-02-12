#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H

#include "process.h"
#include "gtest/gtest.h"

//using namespace pipeline;

namespace Testpipeline {

class Testpipeline : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Testpipeline() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Testpipeline() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // pipeline::obj ev;
};

class TestpipelineExit : public Testpipeline {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestpipelineExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestpipelineExit() {}

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

#endif // TEST_PIPELINE_H
