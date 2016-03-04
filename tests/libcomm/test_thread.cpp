#include "test_thread.h"

#include <string>

namespace LibCommTests {

TestThread::TestThread()
{
    // You can do set-up work for each test here.
}

TestThread::~TestThread()
{
    // You can do clean-up work that doesn't throw exceptions here.
}

// If the constructor and destructor are not enough for setting up
// and cleaning up each test, you can define the following methods:

void TestThread::SetUp()
{
    // Code here will be called immediately after the constructor (right
    // before each test).
}

void TestThread::TearDown()
{
    // Code here will be called immediately after each test (right
    // before the destructor).
}

// Tests that the Foo::Bar() method does Abc.
TEST_F(TestThread, MethodBarDoesAbc) {
    const std::string input_filepath("this/package/testdata/myinputfile.dat");
    const std::string output_filepath("this/package/testdata/myoutputfile.dat");
    EXPECT_NE(input_filepath, output_filepath);
}

// Tests that Foo does Xyz.
TEST_F(TestThread, DoesXyz) {
    // Exercises the Xyz feature of Foo.
    ASSERT_EQ(1, 2);
}

}

