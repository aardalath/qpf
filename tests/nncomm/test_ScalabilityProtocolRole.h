#ifndef TEST_SCALABILITYPROTOCOLROLE_H
#define TEST_SCALABILITYPROTOCOLROLE_H

#include "scalprotrole.h"
#include "gtest/gtest.h"

//using namespace ScalabilityProtocolRole;

namespace TestScalabilityProtocolRole {

class TestScalabilityProtocolRole : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestScalabilityProtocolRole() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestScalabilityProtocolRole() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // ScalabilityProtocolRole::obj ev;
};

class TestScalabilityProtocolRoleExit : public TestScalabilityProtocolRole {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestScalabilityProtocolRoleExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestScalabilityProtocolRoleExit() {}

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

#endif // TEST_SCALABILITYPROTOCOLROLE_H
