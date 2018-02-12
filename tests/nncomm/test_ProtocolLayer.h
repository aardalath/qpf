#ifndef TEST_PROTOCOLLAYER_H
#define TEST_PROTOCOLLAYER_H

#include "protlayer.h"
#include "gtest/gtest.h"

//using namespace ProtocolLayer;

namespace TestProtocolLayer {

class TestProtocolLayer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestProtocolLayer() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestProtocolLayer() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // ProtocolLayer::obj ev;
};

class TestProtocolLayerExit : public TestProtocolLayer {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestProtocolLayerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestProtocolLayerExit() {}

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

#endif // TEST_PROTOCOLLAYER_H
