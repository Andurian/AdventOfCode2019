#define BOOST_TEST_MODULE day_04

#include <boost/test/included/unit_test.hpp>

#include "secure_container.h"


BOOST_AUTO_TEST_CASE(test_condition_length)
{
	BOOST_TEST(has_digits(100000, 6));
	BOOST_TEST(has_digits(123453, 6));
	BOOST_TEST(!has_digits(1111, 6));
	BOOST_TEST(!has_digits(1112521, 6));
	BOOST_TEST(!has_digits(011221, 6));
}


BOOST_AUTO_TEST_CASE(test_condition_identical_neighbors)
{
	BOOST_TEST(has_identical_neighbors(100000));
	BOOST_TEST(has_identical_neighbors(1103));
	BOOST_TEST(has_identical_neighbors(100));
	BOOST_TEST(!has_identical_neighbors(120));
	BOOST_TEST(!has_identical_neighbors(1010101010));
}


BOOST_AUTO_TEST_CASE(test_condition_has_decreasing_neighbors)
{
	BOOST_TEST(!is_not_decreasing(123454321));
	BOOST_TEST(!is_not_decreasing(10000000));
	BOOST_TEST(!is_not_decreasing(23343));
	BOOST_TEST(!is_not_decreasing(101010));
	BOOST_TEST(is_not_decreasing(12345678));
	BOOST_TEST(is_not_decreasing(11111111));
	BOOST_TEST(is_not_decreasing(11111112));
}


BOOST_AUTO_TEST_CASE(test_is_password)
{
	BOOST_TEST(is_password(111111));
	BOOST_TEST(!is_password(223450));
	BOOST_TEST(!is_password(123789));
}


BOOST_AUTO_TEST_CASE(test_day_04_a)
{
	int num = 0;
	for (int i = 382345; i <= 843167; ++i)
	{
		if (is_password(i))
		{
			++num;
		}
	}
	BOOST_TEST(num == 460);
}


BOOST_AUTO_TEST_CASE(test_has_exactly_two_identical_neighbors)
{
	BOOST_TEST(has_exactly_two_identical_neighbors(112233));
	BOOST_TEST(has_exactly_two_identical_neighbors(112223));
	BOOST_TEST(!has_exactly_two_identical_neighbors(123444));
	BOOST_TEST(!has_exactly_two_identical_neighbors(11123444));
	BOOST_TEST(!has_exactly_two_identical_neighbors(799999));
	BOOST_TEST(has_exactly_two_identical_neighbors(111122));
}


BOOST_AUTO_TEST_CASE(test_day_04_b)
{
	int num = 0;
	for (int i = 382345; i <= 843167; ++i)
	{
		if (is_password_extended_rule(i))
		{
			++num;
		}
	}
	BOOST_TEST(num == 290);
}
