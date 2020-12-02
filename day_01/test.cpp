#define BOOST_TEST_MODULE day_01

#include <boost/test/included/unit_test.hpp>

#include "calculateFuelRequirement.h"

BOOST_AUTO_TEST_CASE( test_mass_12 )
{
	BOOST_TEST(calculateFuelRequirement(12) == 2);
}

BOOST_AUTO_TEST_CASE( test_mass_14)
{
	BOOST_TEST(calculateFuelRequirement(14) == 2);
}

BOOST_AUTO_TEST_CASE( test_mass_1969 )
{
	BOOST_TEST(calculateFuelRequirement(1969) == 654);
}

BOOST_AUTO_TEST_CASE( test_mass_100756 )
{
	BOOST_TEST(calculateFuelRequirement(100756) == 33583);
}

BOOST_AUTO_TEST_CASE( test_mass_sum )
{
	const std::vector<int> input{ 12, 14, 1969, 100756 };
	const int expected = 2 + 2 + 654 + 33583;
	BOOST_TEST(calculateFuelRequirementSum(input) == expected);
}

BOOST_AUTO_TEST_CASE( test_mass_and_fuel_14 )
{
	BOOST_TEST(calculateFuelRequirementRecursive(14) == 2);
}

BOOST_AUTO_TEST_CASE(test_mass_and_fuel_1969)
{
	BOOST_TEST(calculateFuelRequirementRecursive(1969) == 966);
}

BOOST_AUTO_TEST_CASE(test_mass_and_fuel_100756)
{
	BOOST_TEST(calculateFuelRequirementRecursive(100756) == 50346);
}

BOOST_AUTO_TEST_CASE(test_mass_and_fuel_sum)
{
	const std::vector<int> input{ 12, 14, 1969, 100756 };
	const int expected = 2 + 2 + 966 + 50346;
	BOOST_TEST(calculateFuelRequirementSumRecursive(input) == expected);
}