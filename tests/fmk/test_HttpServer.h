#ifndef TEST_HTTPSERVER_H
#define TEST_HTTPSERVER_H

#include "httpserver.h"
#include "gtest/gtest.h"

//using namespace HttpServer;

namespace TestHttpServer {

class TestHttpServer : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHttpServer() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHttpServer() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // HttpServer::obj ev;
};

class TestHttpServerExit : public TestHttpServer {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestHttpServerExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestHttpServerExit() {}

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

#endif // TEST_HTTPSERVER_H
