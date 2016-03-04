#ifndef TestRouter2RouterPeer_H
#define TestRouter2RouterPeer_H

#include "r2rpeer.h"
#include "gtest/gtest.h"

using namespace LibComm;

namespace LibCommTests {

class TestRouter2RouterPeer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestRouter2RouterPeer();

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRouter2RouterPeer();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

    // Objects declared here can be used by all tests in the test case for Foo.
};

}

#endif // TestRouter2RouterPeer_H
