#ifndef TEST_TASKORC_H
#define TEST_TASKORC_H

#include "taskorc.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestTaskOrchestrator : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTaskOrchestrator() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTaskOrchestrator() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    TaskOrchestrator * o;
};

class TestTaskOrchestratorExit : public TestTaskOrchestrator {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestTaskOrchestratorExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestTaskOrchestratorExit() {}

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

#endif // TEST_TASKORC_H
