#ifndef TEST_QDTREPORTHANDLER_H
#define TEST_QDTREPORTHANDLER_H

#include "qdtrephdl.h"
#include "gtest/gtest.h"

//using namespace QDTReportHandler;

namespace TestQDTReportHandler {

class TestQDTReportHandler : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestQDTReportHandler() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestQDTReportHandler() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // QDTReportHandler::obj ev;
};

class TestQDTReportHandlerExit : public TestQDTReportHandler {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestQDTReportHandlerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestQDTReportHandlerExit() {}

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

#endif // TEST_QDTREPORTHANDLER_H
