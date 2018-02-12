#ifndef TEST_COMPONENT_H
#define TEST_COMPONENT_H

#include "component.h"
#include "gtest/gtest.h"

//using namespace Component;

namespace TestComponent {

class TestComponent : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestComponent() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestComponent() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Component::obj ev;
};

class TestComponentExit : public TestComponent {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestComponentExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestComponentExit() {}

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

#endif // TEST_COMPONENT_H
