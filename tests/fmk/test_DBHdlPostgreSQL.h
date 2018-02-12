#ifndef TEST_DBHDLPOSTGRESQL_H
#define TEST_DBHDLPOSTGRESQL_H

#include "dbhdlpostgre.h"
#include "gtest/gtest.h"

//using namespace DBHdlPostgreSQL;

namespace TestDBHdlPostgreSQL {

class TestDBHdlPostgreSQL : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDBHdlPostgreSQL() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDBHdlPostgreSQL() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // DBHdlPostgreSQL::obj ev;
};

class TestDBHdlPostgreSQLExit : public TestDBHdlPostgreSQL {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestDBHdlPostgreSQLExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestDBHdlPostgreSQLExit() {}

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

#endif // TEST_DBHDLPOSTGRESQL_H
