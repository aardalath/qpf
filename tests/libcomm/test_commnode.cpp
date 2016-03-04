#include "test_commnode.h"

#include <string>

namespace LibCommTests {

TestCommNode::TestCommNode()
{
    // You can do set-up work for each test here.
}

TestCommNode::~TestCommNode()
{
    // You can do clean-up work that doesn't throw exceptions here.
}

// If the constructor and destructor are not enough for setting up
// and cleaning up each test, you can define the following methods:

void TestCommNode::SetUp()
{
    // Code here will be called immediately after the constructor (right
    // before each test).
}

void TestCommNode::TearDown()
{
    // Code here will be called immediately after each test (right
    // before the destructor).
}

// Tests that the Foo::Bar() method does Abc.
TEST_F(TestCommNode, MethodBarDoesAbc) {
    const std::string input_filepath("this/package/testdata/myinputfile.dat");
    const std::string output_filepath("this/package/testdata/myoutputfile.dat");
    EXPECT_NE(input_filepath, output_filepath);
}

// Tests that Foo does Xyz.
TEST_F(TestCommNode, DoesXyz) {
    // Exercises the Xyz feature of Foo.
    ASSERT_EQ(1, 1);
}

}

