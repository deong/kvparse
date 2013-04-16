#include "kvparse.h"
#include "kvparse_except.h"
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
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config2.cfg"), syntax_error);
}

TEST(basic_parse_test, syntax_missing_value_colon) 
{
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config3.cfg"), syntax_error);
}

TEST(basic_parse_test, syntax_valid_identifiers) 
{
	string val;
	kvparse::read_configuration_file("tests/test_config5.cfg");

	kvparse::parameter_value("key.word", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("key_word", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("key-word", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("_keyword", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("k--ey.word_--_", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("keyword'", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("keyword''", val);
	EXPECT_EQ("value", val);

	kvparse::parameter_value("K", val);
	EXPECT_EQ("value", val);
}

TEST(basic_parse_test, syntax_invalid_identifiers) 
{
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config4.cfg"), syntax_error);
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config6.cfg"), syntax_error);
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config7.cfg"), syntax_error);
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config8.cfg"), syntax_error);
	EXPECT_THROW(kvparse::read_configuration_file("tests/test_config9.cfg"), syntax_error);
}

// The fixture for testing class Foo.
class kvparse_test : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	
	kvparse_test() {
		// You can do set-up work for each test here.
		kvparse::read_configuration_file("tests/test_config1.cfg");
	}
	
	virtual ~kvparse_test() {
		// You can do clean-up work that doesn't throw exceptions here.
		kvparse::clear();
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
	list<string> lsvalue;
	list<int> livalue;
	list<unsigned int> luvalue;
	list<double> ldvalue;
	vector<double> vdvalue;
	vector<int> vivalue;
	vector<string> vsvalue;
};

TEST_F(kvparse_test, parse_integer_colon) 
{
	kvparse::parameter_value("integer1", ivalue);
	EXPECT_EQ(1, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_colon_space) 
{
	kvparse::parameter_value("integer2", ivalue);
	EXPECT_EQ(2, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_equals_space) 
{
	kvparse::parameter_value("integer3", ivalue);
	EXPECT_EQ(3, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_equals) 
{
	kvparse::parameter_value("integer4", ivalue);
	EXPECT_EQ(4, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_more_space) 
{
	kvparse::parameter_value("integer5", ivalue);
	EXPECT_EQ(5, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_comment) 
{
	kvparse::parameter_value("integer6", ivalue);
	EXPECT_EQ(6, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_negative_equals) 
{
	kvparse::parameter_value("integer7", ivalue);
	EXPECT_EQ(-7, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_negative_colon) 
{
	kvparse::parameter_value("integer8", ivalue);
	EXPECT_EQ(-8, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_octal_bad) 
{
	kvparse::parameter_value("integer9", ivalue);
	EXPECT_EQ(9, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_octal_good) 
{
	kvparse::parameter_value("integer9a", ivalue);
	EXPECT_EQ(10, ivalue);
}
	
TEST_F(kvparse_test, parse_integer_float) 
{
	EXPECT_THROW(kvparse::parameter_value("integer10", ivalue), illegal_value_error);
}
	
TEST_F(kvparse_test, parse_integer_string) 
{
	EXPECT_THROW(kvparse::parameter_value("integer11", ivalue), illegal_value_error);
}
	
TEST_F(kvparse_test, parse_integer_spaces) 
{
	EXPECT_THROW(kvparse::parameter_value("integer12", ivalue), illegal_value_error);
}
	
TEST_F(kvparse_test, parse_integer_first_dup) 
{
	EXPECT_THROW(kvparse::parameter_value("integer13", ivalue), ambiguous_keyword_error);
}
	
TEST_F(kvparse_test, parse_integer_second_dup) 
{
	EXPECT_THROW(kvparse::parameter_value("integer13", ivalue), ambiguous_keyword_error);
}

TEST_F(kvparse_test, parse_integer_leading_tab) 
{
	kvparse::parameter_value("integer15", ivalue);
	EXPECT_EQ(15, ivalue);
}

TEST_F(kvparse_test, missing_required)
{
	EXPECT_THROW(kvparse::parameter_value("integer99", ivalue, true), missing_keyword_error);
}

TEST_F(kvparse_test, optional_default)
{
	ivalue = 42;
	kvparse::parameter_value("integer99", ivalue, false);
	EXPECT_EQ(42, ivalue);
}

TEST_F(kvparse_test, string_parameter) 
{
	kvparse::parameter_value("string1", svalue);
	EXPECT_EQ("hello", svalue);
}

TEST_F(kvparse_test, string_parameter_optional) 
{
	svalue = "hello";
	kvparse::parameter_value("stringXXXX", svalue, false);
	EXPECT_EQ("hello", svalue);
}

TEST_F(kvparse_test, string_parameter_numeric) 
{
	kvparse::parameter_value("string2", svalue);
	EXPECT_EQ("100", svalue);
}

TEST_F(kvparse_test, string_parameter_multiword)
{
	kvparse::parameter_value("string3", svalue);
	EXPECT_EQ("This is a multiword string", svalue);
}
	
TEST_F(kvparse_test, string_only_quote)
{
	EXPECT_THROW(kvparse::parameter_value("string4", svalue), syntax_error);
}
	
TEST_F(kvparse_test, string_only_double_quotes)
{
	kvparse::parameter_value("string5", svalue);
	EXPECT_EQ("", svalue);
}
	
TEST_F(kvparse_test, unsigned_int_parameter) 
{
	kvparse::parameter_value("uint-param1", uvalue, true);
	EXPECT_EQ(99, uvalue);
}

TEST_F(kvparse_test, unsigned_int_negative_parameter) 
{
	EXPECT_THROW(kvparse::parameter_value("uint-param2", uvalue, true), illegal_value_error);
}

TEST_F(kvparse_test, unsigned_int_string_parameter) 
{
	EXPECT_THROW(kvparse::parameter_value("uint-param3", uvalue), illegal_value_error);
}

TEST_F(kvparse_test, double_parameter) 
{
	kvparse::parameter_value("double_param", dvalue);
	EXPECT_DOUBLE_EQ(3.14159, dvalue);
}

TEST_F(kvparse_test, double_from_int) 
{
	kvparse::parameter_value("double_param2", dvalue);
	EXPECT_DOUBLE_EQ(2001, dvalue);
}

TEST_F(kvparse_test, double_from_quoted_string) 
{
	EXPECT_THROW(kvparse::parameter_value("double_param-3", dvalue), illegal_value_error);
}

TEST_F(kvparse_test, double_too_many_decimals) 
{
	EXPECT_THROW(kvparse::parameter_value("double_param-4", dvalue), illegal_value_error);
}

TEST_F(kvparse_test, double_negative_zero_pad) 
{
	kvparse::parameter_value("double_param5", dvalue);
	EXPECT_DOUBLE_EQ(-0.001, dvalue);
}

TEST_F(kvparse_test, double_explicit_positive) 
{
	kvparse::parameter_value("double_param6", dvalue);
	EXPECT_DOUBLE_EQ(0.001, dvalue);
}

TEST_F(kvparse_test, double_positive_nopad) 
{
	kvparse::parameter_value("double_param7", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(kvparse_test, double_negative_nopad) 
{
	kvparse::parameter_value("double_param8", dvalue);
	EXPECT_DOUBLE_EQ(-0.5, dvalue);
}

TEST_F(kvparse_test, double_explicit_positive_nopad) 
{
	kvparse::parameter_value("double_param9", dvalue);
	EXPECT_DOUBLE_EQ(0.5, dvalue);
}

TEST_F(kvparse_test, bool_yes) 
{
	kvparse::parameter_value("bool1", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(kvparse_test, bool_no) 
{
	kvparse::parameter_value("bool2", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, bool_true) 
{
	kvparse::parameter_value("bool3", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(kvparse_test, bool_false) 
{
	kvparse::parameter_value("bool4", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, bool_YES) 
{
	kvparse::parameter_value("bool5", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(kvparse_test, bool_NO) 
{
	kvparse::parameter_value("bool6", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, bool_TRUE) 
{
	kvparse::parameter_value("bool7", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(kvparse_test, bool_FALSE) 
{
	kvparse::parameter_value("bool8", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, bool_invalid) 
{
	EXPECT_THROW(kvparse::parameter_value("bool9", bvalue), illegal_value_error);
}

TEST_F(kvparse_test, bool_1) 
{
	kvparse::parameter_value("bool10", bvalue);
	EXPECT_TRUE(bvalue);
}

TEST_F(kvparse_test, bool_0) 
{
	kvparse::parameter_value("bool11", bvalue);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, bool_optional_true) 
{
	bvalue = true;
	kvparse::parameter_value("boolXXX", bvalue, false);
	EXPECT_TRUE(bvalue);
}
	
TEST_F(kvparse_test, bool_optional_false) 
{
	bvalue = false;
	kvparse::parameter_value("boolXXX", bvalue, false);
	EXPECT_FALSE(bvalue);
}

TEST_F(kvparse_test, string_list) 
{
	kvparse::parameter_value("string_vals", lsvalue);
	ASSERT_EQ(3, lsvalue.size());
	list<string>::iterator it=lsvalue.begin();
	EXPECT_EQ("alpha", *it++);
	EXPECT_EQ("bravo", *it++);
	EXPECT_EQ("delta", *it++);
}

TEST_F(kvparse_test, string_list_with_quotes)
{
	lsvalue.clear();
	kvparse::parameter_value("string_vals2", lsvalue);
	ASSERT_EQ(3, lsvalue.size());
	list<string>::iterator it = lsvalue.begin();
	EXPECT_EQ("alpha", *it++);
	EXPECT_EQ("bravo delta", *it++);
	EXPECT_EQ("gamma", *it++);
}

}  // namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



	
