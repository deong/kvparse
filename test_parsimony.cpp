#include "parsimony.h"
#include "parsimony_except.h"
#include <gtest/gtest.h>
#include <stdexcept>

using std::string;
using std::vector;
using std::runtime_error;

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

	parsimony::parameter_value("key.word", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("key_word", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("key-word", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("_keyword", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("k--ey.word_--_", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("keyword'", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("keyword''", val);
	EXPECT_EQ("value", val);

	parsimony::parameter_value("K", val);
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
	parsimony::parameter_value("integer1", ivalue);
	EXPECT_EQ(1, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_colon_space) 
{
	parsimony::parameter_value("integer2", ivalue);
	EXPECT_EQ(2, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_equals_space) 
{
	parsimony::parameter_value("integer3", ivalue);
	EXPECT_EQ(3, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_equals) 
{
	parsimony::parameter_value("integer4", ivalue);
	EXPECT_EQ(4, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_more_space) 
{
	parsimony::parameter_value("integer5", ivalue);
	EXPECT_EQ(5, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_comment) 
{
	parsimony::parameter_value("integer6", ivalue);
	EXPECT_EQ(6, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_negative_equals) 
{
	parsimony::parameter_value("integer7", ivalue);
	EXPECT_EQ(-7, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_negative_colon) 
{
	parsimony::parameter_value("integer8", ivalue);
	EXPECT_EQ(-8, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_octal_bad) 
{
	parsimony::parameter_value("integer9", ivalue);
	EXPECT_EQ(9, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_octal_good) 
{
	parsimony::parameter_value("integer9a", ivalue);
	EXPECT_EQ(10, ivalue);
}
	
TEST_F(parsimony_test, parse_integer_float) 
{
	EXPECT_THROW(parsimony::parameter_value("integer10", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_string) 
{
	EXPECT_THROW(parsimony::parameter_value("integer11", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_spaces) 
{
	EXPECT_THROW(parsimony::parameter_value("integer12", ivalue), illegal_value_error);
}
	
TEST_F(parsimony_test, parse_integer_first_dup) 
{
	EXPECT_THROW(parsimony::parameter_value("integer13", ivalue), ambiguous_keyword_error);
}
	
TEST_F(parsimony_test, parse_integer_second_dup) 
{
	EXPECT_THROW(parsimony::parameter_value("integer13", ivalue), ambiguous_keyword_error);
}

TEST_F(parsimony_test, parse_integer_leading_tab) 
{
	parsimony::parameter_value("integer15", ivalue);
	EXPECT_EQ(15, ivalue);
}

TEST_F(parsimony_test, missing_required)
{
	EXPECT_THROW(parsimony::parameter_value("integer99", ivalue, true), missing_keyword_error);
}

TEST_F(parsimony_test, optional_default)
{
	ivalue = 42;
	parsimony::parameter_value("integer99", ivalue, false);
	EXPECT_EQ(42, ivalue);
}

TEST_F(parsimony_test, string_parameter) 
{
	parsimony::parameter_value("string1", svalue);
	EXPECT_EQ("hello", svalue);
}

TEST_F(parsimony_test, string_parameter_optional) 
{
	svalue = "hello";
	parsimony::parameter_value("stringXXXX", svalue, false);
	EXPECT_EQ("hello", svalue);
}

TEST_F(parsimony_test, string_parameter_numeric) 
{
	parsimony::parameter_value("string2", svalue);
	EXPECT_EQ("100", svalue);
}

TEST_F(parsimony_test, string_parameter_multiword)
{
	parsimony::parameter_value("string3", svalue);
	EXPECT_EQ("This is a multiword string", svalue);
}
	
TEST_F(parsimony_test, string_only_quote)
{
	parsimony::parameter_value("string4", svalue);
	EXPECT_EQ("\"", svalue);
}
	
TEST_F(parsimony_test, string_only_double_quotes)
{
	parsimony::parameter_value("string5", svalue);
	EXPECT_EQ("", svalue);
}
	
TEST_F(parsimony_test, unsigned_int_parameter) 
{
	parsimony::parameter_value("uint-param1", uvalue, true);
	EXPECT_EQ(99, uvalue);
}

TEST_F(parsimony_test, unsigned_int_negative_parameter) 
{
	EXPECT_THROW(parsimony::parameter_value("uint-param2", uvalue, true), illegal_value_error);
}

TEST_F(parsimony_test, unsigned_int_string_parameter) 
{
	EXPECT_THROW(parsimony::parameter_value("uint-param3", uvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_parameter) 
{
	parsimony::parameter_value("double_param", dvalue);
	EXPECT_DOUBLE_EQ(3.14159, dvalue);
}

TEST_F(parsimony_test, double_from_int) 
{
	parsimony::parameter_value("double_param2", dvalue);
	EXPECT_DOUBLE_EQ(2001, dvalue);
}

TEST_F(parsimony_test, double_from_quoted_string) 
{
	EXPECT_THROW(parsimony::parameter_value("double_param-3", dvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_too_many_decimals) 
{
	EXPECT_THROW(parsimony::parameter_value("double_param-4", dvalue), illegal_value_error);
}

TEST_F(parsimony_test, double_negative_zero_pad) 
{
	parsimony::parameter_value("double_param5", dvalue);
	EXPECT_DOUBLE_EQ(-0.001, dvalue);
}

TEST_F(parsimony_test, double_explicit_positive) 
{
	parsimony::parameter_value("double_param6", dvalue);
	EXPECT_DOUBLE_EQ(0.001, dvalue);
}

TEST_F(parsimony_test, double_positive_nopad) 
{
	parsimony::parameter_value("double_param7", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(parsimony_test, double_negative_nopad) 
{
	parsimony::parameter_value("double_param8", dvalue);
	EXPECT_DOUBLE_EQ(-0.5, dvalue);
}

TEST_F(parsimony_test, double_explicit_positive_nopad) 
{
	parsimony::parameter_value("double_param9", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(parsimony_test, bool_yes) 
{
	parsimony::parameter_value("bool1", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_no) 
{
	parsimony::parameter_value("bool2", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_true) 
{
	parsimony::parameter_value("bool3", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_false) 
{
	parsimony::parameter_value("bool4", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_YES) 
{
	parsimony::parameter_value("bool5", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_NO) 
{
	parsimony::parameter_value("bool6", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_TRUE) 
{
	parsimony::parameter_value("bool7", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_FALSE) 
{
	parsimony::parameter_value("bool8", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_invalid) 
{
	EXPECT_THROW(parsimony::parameter_value("bool9", bvalue), illegal_value_error);
}

TEST_F(parsimony_test, bool_1) 
{
	parsimony::parameter_value("bool10", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(parsimony_test, bool_0) 
{
	parsimony::parameter_value("bool11", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(parsimony_test, bool_optional_true) 
{
	bvalue = true;
	parsimony::parameter_value("boolXXX", bvalue, false);
	EXPECT_TRUE(bvalue);
}
	
TEST_F(parsimony_test, bool_optional_false) 
{
	bvalue = false;
	parsimony::parameter_value("boolXXX", bvalue, false);
	EXPECT_FALSE(bvalue);
}
	
}  // namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



	
