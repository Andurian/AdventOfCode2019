#define BOOST_TEST_MODULE common

#include <sstream>

#include <boost/test/included/unit_test.hpp>

#include "read_file.h"

BOOST_AUTO_TEST_CASE(test_read_file_01)
{
	BOOST_TEST( std::string{"testcontent"} == read_file("file_01.txt") );
}

BOOST_AUTO_TEST_CASE(test_read_file_02)
{
	BOOST_TEST(read_file("file_02.txt") == "this is\na\ntest file");
}

BOOST_AUTO_TEST_CASE(test_split_at_space)
{
	const auto output = split_string("a b c", " ");
	const auto expected = std::vector<std::string>{ "a", "b", "c" };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_split_at_multiple_space)
{
	const auto output = split_string("  a  b     c  ", " ");
	const auto expected = std::vector<std::string>{ "a", "b", "c" };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_split_at_multichar_delimiter)
{
	const auto output = split_string("a, b, c", ", ");
	const auto expected = std::vector<std::string>{ "a", "b", "c" };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_split_at_multiple_multichar_delimiter)
{
	const auto output = split_string(", a, , b, , , c, , ", ", ");
	const auto expected = std::vector<std::string>{ "a", "b", "c" };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_read_and_split_to_int)
{
	const auto output = read_collection("file_03.txt", ", ", Converter<int>());
	const auto expected = std::vector<int>{ 0, 1, 2, 3, 4, 5 };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_split_no_delimiter_to_int)
{
	const auto output = split_string("1234567890", Converter<int>{});
	const auto expected = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_read_file_and_split_no_delimiter_to_int)
{
	const auto output = read_collection("file_04.txt", Converter<int>{});
	const auto expected = std::vector<int>{ 1,1,1,0,1,0,1,0,1,2,2,2,2,0,1,0,2,0,2,0,1,2,1,2 };
	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}