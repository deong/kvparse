#include "parsimony.h"
#include "parsimony_except.h"
#include <gtest/gtest.h>

namespace {

// The fixture for testing class Foo.
class parsimony_test : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	
	parsimony_test() {
		// You can do set-up work for each test here.
		parsimony::read_configuration_file("test_config1.cfg");
	}
	
	virtual ~parsimony_test() {
		// You can do clean-up work that doesn't throw exceptions here.
		parsimony::clear();
	}
	
	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:
	
	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}
	
	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
	
	// Objects declared here can be used by all tests in the test case for Foo.
	int value;
};

TEST_F(parsimony_test, parse_integer_colon) 
{
	parsimony::integer_parameter("keyword1", value);
	EXPECT_EQ(1, value);
}
	
TEST_F(parsimony_test, parse_integer_colon_space) 
{
	parsimony::integer_parameter("keyword2", value);
	EXPECT_EQ(2, value);
}
	
TEST_F(parsimony_test, parse_integer_equals_space) 
{
	parsimony::integer_parameter("keyword3", value);
	EXPECT_EQ(3, value);
}
	
TEST_F(parsimony_test, parse_integer_equals) 
{
	parsimony::integer_parameter("keyword4", value);
	EXPECT_EQ(4, value);
}
	
TEST_F(parsimony_test, parse_integer_more_space) 
{
	parsimony::integer_parameter("keyword5", value);
	EXPECT_EQ(5, value);
}
	
TEST_F(parsimony_test, parse_integer_comment) 
{
	parsimony::integer_parameter("keyword6", value);
	EXPECT_EQ(6, value);
}
	
TEST_F(parsimony_test, parse_integer_negative_equals) 
{
	parsimony::integer_parameter("keyword7", value);
	EXPECT_EQ(-7, value);
}
	
TEST_F(parsimony_test, parse_integer_negative_colon) 
{
	parsimony::integer_parameter("keyword8", value);
	EXPECT_EQ(-8, value);
}
	
TEST_F(parsimony_test, parse_integer_octal_bad) 
{
	parsimony::integer_parameter("keyword9", value);
	EXPECT_EQ(9, value);
}
	
TEST_F(parsimony_test, parse_integer_octal_good) 
{
	parsimony::integer_parameter("keyword9a", value);
	EXPECT_EQ(10, value);
}
	
TEST_F(parsimony_test, parse_integer_float) 
{
	EXPECT_THROW(parsimony::integer_parameter("keyword10", value), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_string) 
{
	EXPECT_THROW(parsimony::integer_parameter("keyword11", value), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_spaces) 
{
	EXPECT_THROW(parsimony::integer_parameter("keyword12", value), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_first_dup) 
{
	EXPECT_THROW(parsimony::integer_parameter("keyword13", value), ambiguous_keyword_error);
}
	
TEST_F(parsimony_test, parse_integer_second_dup) 
{
	EXPECT_THROW(parsimony::integer_parameter("keyword14", value), ambiguous_keyword_error);
}

}  // namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



	
