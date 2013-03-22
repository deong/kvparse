#include "parsimony.h"
#include "parsimony_except.h"
#include <gtest/gtest.h>

using std::string;
using std::vector;

namespace {

	class basic_parse_test : public ::testing::Test 
	{
	};

// need to test basic syntax parsing outside of the test fixture
TEST(basic_parse_test, syntax_missing_value) 
{
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config2.cfg"), runtime_error);
}

TEST(basic_parse_test, syntax_missing_value_colon) 
{
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config3.cfg"), runtime_error);
}

TEST(basic_parse_test, syntax_valid_identifiers) 
{
	string val;
	parsimony::read_configuration_file("tests/test_config5.cfg");

	parsimony::string_parameter("key.word", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("key_word", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("key-word", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("_keyword", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("k--ey.word_--_", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("keyword'", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("keyword''", val);
	EXPECT_EQ("value", val);

	parsimony::string_parameter("K", val);
	EXPECT_EQ("value", val);
}

TEST(basic_parse_test, syntax_invalid_identifiers) 
{
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config4.cfg"), runtime_error);
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config6.cfg"), runtime_error);
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config7.cfg"), runtime_error);
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config8.cfg"), runtime_error);
	EXPECT_THROW(parsimony::read_configuration_file("tests/test_config9.cfg"), runtime_error);
}

// The fixture for testing class Foo.
class parsimony_test : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	
	parsimony_test() {
		// You can do set-up work for each test here.
		parsimony::read_configuration_file("tests/test_config1.cfg");
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
	int ivalue;
	string svalue;
	unsigned int uvalue;
	double dvalue;
	bool bvalue;
	list<string> lvalue;
	vector<double> vdvalue;
	vector<int> vivalue;
	vector<string> vsvalue;
};

TEST_F(parsimony_test, parse_integer_colon) 
{
	parsimony::integer_parameter("integer1", ivalue);
	EXPECT_EQ(1, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_colon_space) 
{
	parsimony::integer_parameter("integer2", ivalue);
	EXPECT_EQ(2, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_equals_space) 
{
	parsimony::integer_parameter("integer3", ivalue);
	EXPECT_EQ(3, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_equals) 
{
	parsimony::integer_parameter("integer4", ivalue);
	EXPECT_EQ(4, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_more_space) 
{
	parsimony::integer_parameter("integer5", ivalue);
	EXPECT_EQ(5, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_comment) 
{
	parsimony::integer_parameter("integer6", ivalue);
	EXPECT_EQ(6, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_negative_equals) 
{
	parsimony::integer_parameter("integer7", ivalue);
	EXPECT_EQ(-7, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_negative_colon) 
{
	parsimony::integer_parameter("integer8", ivalue);
	EXPECT_EQ(-8, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_octal_bad) 
{
	parsimony::integer_parameter("integer9", ivalue);
	EXPECT_EQ(9, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_octal_good) 
{
	parsimony::integer_parameter("integer9a", ivalue);
	EXPECT_EQ(10, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_float) 
{
	EXPECT_THROW(parsimony::integer_parameter("integer10", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_string) 
{
	EXPECT_THROW(parsimony::integer_parameter("integer11", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_spaces) 
{
	EXPECT_THROW(parsimony::integer_parameter("integer12", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_first_dup) 
{
	EXPECT_THROW(parsimony::integer_parameter("integer13", ivalue), ambiguous_keyword_error);
}
	
TEST_F(parsimony_test, parse_integer_second_dup) 
{
	EXPECT_THROW(parsimony::integer_parameter("integer13", ivalue), ambiguous_keyword_error);
}

TEST_F(parsimony_test, parse_integer_leading_tab) 
{
	parsimony::integer_parameter("integer15", ivalue);
	EXPECT_EQ(15, ivalue);
}

TEST_F(parsimony_test, missing_required)
{
	EXPECT_THROW(parsimony::integer_parameter("integer99", ivalue, true), missing_keyword_error);
}

TEST_F(parsimony_test, optional_default)
{
	ivalue = 42;
	parsimony::integer_parameter("integer99", ivalue);
	EXPECT_EQ(42, ivalue);
}

TEST_F(parsimony_test, string_parameter) 
{
	parsimony::string_parameter("string1", svalue);
	EXPECT_EQ("hello", svalue);
}

TEST_F(parsimony_test, string_parameter_optional) 
{
	svalue = "hello";
	parsimony::string_parameter("stringXXXX", svalue);
	EXPECT_EQ("hello", svalue);
}

TEST_F(parsimony_test, string_parameter_numeric) 
{
	parsimony::string_parameter("string2", svalue);
	EXPECT_EQ("100", svalue);
}

TEST_F(parsimony_test, string_parameter_multiword)
{
	parsimony::string_parameter("string3", svalue);
	EXPECT_EQ("This is a multiword string", svalue);
}
	
TEST_F(parsimony_test, string_only_quote)
{
	parsimony::string_parameter("string4", svalue);
	EXPECT_EQ("\"", svalue);
}
	
TEST_F(parsimony_test, string_only_double_quotes)
{
	parsimony::string_parameter("string5", svalue);
	EXPECT_EQ("", svalue);
}
	
TEST_F(parsimony_test, unsigned_int_parameter) 
{
	parsimony::unsigned_integer_parameter("uint-param1", uvalue, true);
	EXPECT_EQ(99, uvalue);
}

TEST_F(parsimony_test, unsigned_int_negative_parameter) 
{
	EXPECT_THROW(parsimony::unsigned_integer_parameter("uint-param2", uvalue, true), illegal_value_error);
}

TEST_F(parsimony_test, unsigned_int_string_parameter) 
{
	EXPECT_THROW(parsimony::unsigned_integer_parameter("uint-param3", uvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_parameter) 
{
	parsimony::double_parameter("double_param", dvalue);
	EXPECT_DOUBLE_EQ(3.14159, dvalue);
}

TEST_F(parsimony_test, double_from_int) 
{
	parsimony::double_parameter("double_param2", dvalue);
	EXPECT_DOUBLE_EQ(2001, dvalue);
}

TEST_F(parsimony_test, double_from_quoted_string) 
{
	EXPECT_THROW(parsimony::double_parameter("double_param-3", dvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_too_many_decimals) 
{
	EXPECT_THROW(parsimony::double_parameter("double_param-4", dvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_negative_zero_pad) 
{
	parsimony::double_parameter("double_param5", dvalue);
	EXPECT_DOUBLE_EQ(-0.001, dvalue);
}

TEST_F(parsimony_test, double_explicit_positive) 
{
	parsimony::double_parameter("double_param6", dvalue);
	EXPECT_DOUBLE_EQ(0.001, dvalue);
}

TEST_F(parsimony_test, double_positive_nopad) 
{
	parsimony::double_parameter("double_param7", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(parsimony_test, double_negative_nopad) 
{
	parsimony::double_parameter("double_param8", dvalue);
	EXPECT_DOUBLE_EQ(-0.5, dvalue);
}

TEST_F(parsimony_test, double_explicit_positive_nopad) 
{
	parsimony::double_parameter("double_param9", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(parsimony_test, bool_yes) 
{
	parsimony::boolean_parameter("bool1", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_no) 
{
	parsimony::boolean_parameter("bool2", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_true) 
{
	parsimony::boolean_parameter("bool3", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_false) 
{
	parsimony::boolean_parameter("bool4", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_YES) 
{
	parsimony::boolean_parameter("bool5", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_NO) 
{
	parsimony::boolean_parameter("bool6", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_TRUE) 
{
	parsimony::boolean_parameter("bool7", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_FALSE) 
{
	parsimony::boolean_parameter("bool8", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_invalid) 
{
	EXPECT_THROW(parsimony::boolean_parameter("bool9", bvalue), illegal_value_error);
}

TEST_F(parsimony_test, bool_1) 
{
	parsimony::boolean_parameter("bool10", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_0) 
{
	parsimony::boolean_parameter("bool11", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_optional_true) 
{
	bvalue = true;
	parsimony::boolean_parameter("boolXXX", bvalue);
	EXPECT_TRUE(bvalue);
}
	
TEST_F(parsimony_test, bool_optional_false) 
{
	bvalue = false;
	parsimony::boolean_parameter("boolXXX", bvalue);
	EXPECT_FALSE(bvalue);
}
	
}  // namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



	
