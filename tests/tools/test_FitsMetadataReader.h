#ifndef TEST_FITSMETADATAREADER_H
#define TEST_FITSMETADATAREADER_H

#include "metadatareader.h"
#include "gtest/gtest.h"

//using namespace FitsMetadataReader;

namespace TestFitsMetadataReader {

class TestFitsMetadataReader : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestFitsMetadataReader() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestFitsMetadataReader() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // FitsMetadataReader::obj ev;
};

class TestFitsMetadataReaderExit : public TestFitsMetadataReader {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    TestFitsMetadataReaderExit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestFitsMetadataReaderExit() {}

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

#endif // TEST_FITSMETADATAREADER_H
