#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include "config.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestConfiguration : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestConfiguration() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestConfiguration() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    Configuration * o;
};

class TestConfigurationExit : public TestConfiguration {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestConfigurationExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestConfigurationExit() {}

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

#endif // TEST_CONFIG_H
