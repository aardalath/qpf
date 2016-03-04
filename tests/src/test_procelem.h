#ifndef TEST_PROCELEM_H
#define TEST_PROCELEM_H

#include "procelem.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestProcessingElement : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestProcessingElement() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestProcessingElement() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    ProcessingElement * o;
};

class TestProcessingElementExit : public TestProcessingElement {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestProcessingElementExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestProcessingElementExit() {}

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

#endif // TEST_PROCELEM_H
