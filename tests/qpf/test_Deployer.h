#ifndef TEST_DEPLOYER_H
#define TEST_DEPLOYER_H

#include "deployer.h"
#include "gtest/gtest.h"

//using namespace Deployer;

namespace TestDeployer {

class TestDeployer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDeployer() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDeployer() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // Deployer::obj ev;
};

class TestDeployerExit : public TestDeployer {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDeployerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDeployerExit() {}

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

#endif // TEST_DEPLOYER_H
