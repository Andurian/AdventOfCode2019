#define BOOST_TEST_MODULE day_10

#include <boost/test/included/unit_test.hpp>

#include <cmath>
#include <vector>

#include "asteroids.h"
#include "read_file.h"
#include "infix_ostream_iterator.h"



BOOST_AUTO_TEST_CASE(test_asteroids_convert_image)
{
	const auto result = convert_image(read_collection("input_test_01.txt", "\n"));
	const std::vector<Eigen::Vector2d> expected{
		{ 1, 0 }, { 4, 0 }, { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 }, { 4, 2 }, { 4, 3 }, { 3, 4 }, { 4, 4 },
	};

	BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_asteroid_can_see)
{
    const auto asteroids = convert_image(read_collection("input_test_01.txt", "\n"));

    BOOST_TEST(can_see(asteroids[0], asteroids[1], asteroids) == true);
    BOOST_TEST(can_see(asteroids[2], asteroids[6], asteroids) == false);
    BOOST_TEST(can_see(asteroids[1], asteroids[6], asteroids) == true);
    BOOST_TEST(can_see(asteroids[1], asteroids[7], asteroids) == false);
    BOOST_TEST(can_see(asteroids[1], asteroids[8], asteroids) == true);
    BOOST_TEST(can_see(asteroids[1], asteroids[9], asteroids) == false);
    BOOST_TEST(can_see(asteroids[2], asteroids[3], asteroids) == true);
    BOOST_TEST(can_see(asteroids[3], asteroids[2], asteroids) == true);
    BOOST_TEST(can_see(asteroids[0], asteroids[5], asteroids) == true);
    BOOST_TEST(can_see(asteroids[5], asteroids[0], asteroids) == true);
}


BOOST_AUTO_TEST_CASE(test_asteroids_num_can_see)
{
	std::vector<Asteroid> input = asteroids_from_file("input_test_01.txt");

	std::vector<int> result;
	for(const auto & a : input)
	{
		result.push_back(num_can_see(a, input));
	}

	std::vector<int> expected{ 7, 7, 6, 7, 7, 7, 5, 7, 8, 7 };

	BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_asteroid_best_01)
{
    const auto asteroids = convert_image(read_collection("input_test_01.txt", "\n"));

    BOOST_TEST(num_can_see(best_asteroid(asteroids), asteroids) == 8);
}


BOOST_AUTO_TEST_CASE(test_angle, *boost::unit_test::tolerance(0.001))
{
	BOOST_TEST(angle({ 0, -1 }, { 0, -1 }) == 0 );
	BOOST_TEST(angle({ 0, -1 }, { 1, 0 }) == M_PI_2 );
	BOOST_TEST(angle({ 0, -1 }, { 0, 1 }) == M_PI );
	BOOST_TEST(angle({ 0, -1 }, { -1, 0 }) == 3 * M_PI_2 );
}


BOOST_AUTO_TEST_CASE(test_asteroid_ordering)
{
	std::vector<Asteroid> input{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

	std::vector<Asteroid> expected{ { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	auto result = sorted_clockwise({ 0, 0 }, input);

	BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_asteroid_vaporization_order)
{
	std::vector<Asteroid> input = asteroids_from_file("input_test_05.txt");

	Asteroid pos{ 11, 13 };
	input.erase(std::find(input.begin(), input.end(), pos));

	std::vector<Asteroid> vap = vaporization_order(pos, input);

	BOOST_TEST(vap[0] == Asteroid( 11, 12 ));
	BOOST_TEST(vap[1] == Asteroid( 12, 1 ));
	BOOST_TEST(vap[2] == Asteroid( 12, 2 ));
	BOOST_TEST(vap[9] == Asteroid( 12, 8 ));
	BOOST_TEST(vap[19] == Asteroid( 16, 0 ));
	BOOST_TEST(vap[49] == Asteroid( 16, 9 ));
	BOOST_TEST(vap[99] == Asteroid( 10, 16 ));
	BOOST_TEST(vap[198] == Asteroid( 9, 6 ));
	BOOST_TEST(vap[199] == Asteroid( 8, 2 ));
	BOOST_TEST(vap[200] == Asteroid( 10, 9 ));
	BOOST_TEST(vap[298] == Asteroid( 11, 1 ));
}

