#ifndef TEST_ARCHIVEMODEL_H
#define TEST_ARCHIVEMODEL_H

#include "archivemodel.h"
#include "gtest/gtest.h"

using namespace QPF;

namespace TestQPF {

class TestArchiveModel : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestArchiveModel() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestArchiveModel() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    ArchiveModel o;
};

class TestArchiveModelExit : public TestArchiveModel {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestArchiveModelExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestArchiveModelExit() {}

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

#endif // TEST_ARCHIVEMODEL_H
