#define BOOST_TEST_MODULE day_03

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include "wire_distance.h"
#include "read_file.h"

BOOST_AUTO_TEST_CASE(test_split_input)
{
	std::vector<std::string> directions = split_string("R8,U5,L5,D3", ",");
	std::vector<std::string> expected = { "R8", "U5", "L5", "D3" };

	BOOST_CHECK_EQUAL_COLLECTIONS(directions.begin(), directions.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_point_ordering)
{
	const Point a{ 0, 0 };
	const Point b{ 1, 0 };
	const Point c{ 0, 1 };
	const Point d{ -2, 1 };

	BOOST_TEST( a < b );
	BOOST_TEST(a < c);
	BOOST_TEST(!(a < d));
	BOOST_TEST(!(a < a));
}


BOOST_AUTO_TEST_CASE(test_wire_corners_1)
{
	const auto output = wire_corners("R8,U5,L5,D3");
	const std::vector<Point> expected{ { 0, 0 }, { 8, 0 }, { 8, 5 }, { 3, 5 }, { 3, 2 } };

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_wire_corners_2)
{
	const auto output = wire_corners("U7,R6,D4,L4");
	const std::vector<Point> expected{ { 0, 0 }, { 0, 7 }, { 6, 7 }, { 6, 3 }, { 2, 3 } };

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_direction_functor)
{
	BOOST_TEST((direction_functor('R')({ 0, 0 }) == Point{ 1, 0 }));
	BOOST_TEST((direction_functor('U')({ 0, 0 }) == Point{ 0,  1 }));
	BOOST_TEST((direction_functor('L')({ 0, 0 }) == Point{ -1, 0 }));
	BOOST_TEST((direction_functor('D')({ 0, 0 }) == Point{ 0, -1 }));
}


BOOST_AUTO_TEST_CASE(test_wire_tiles)
{
	// clang-format off
	std::vector<std::pair<std::vector<Point>, std::vector<Point>>> data =
	{
		{
			wire_tiles(Point{ 0, 0 }, "R5"),
			{ { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 } }
		},
		{
			wire_tiles({ 2, 4 }, "U6"),
			{ { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 }, { 2, 9 }, { 2, 10 } }
		},
		{
			wire_tiles({ 1, 1 }, "L2"),
			{ { 0, 1 }, { -1, 1 } }
		},
		{
			wire_tiles({ -10, -10 }, "D3"),
			{ { -10, -11 }, { -10, -12 }, { -10, -13 } }
		}
	};
	// clang-format on


	for(auto it : data)
	{
		const auto output = it.first;
		const auto expected = it.second;
		BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
	}
}


BOOST_AUTO_TEST_CASE(test_wire_intersections)
{
	const std::string wire1 = "R8,U5,L5,D3";
	const std::string wire2 = "U7,R6,D4,L4";

	const std::vector<Point> expected{ { 3, 3 }, { 6, 5 } };

	const std::vector<Point> output = wire_intersections(wire1, wire2);

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_clostest_intersection)
{
	BOOST_TEST(closest_intersection("R8,U5,L5,D3", "U7,R6,D4,L4") == 6);
	BOOST_TEST(closest_intersection("R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83") == 159);
	BOOST_TEST(closest_intersection("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7") == 135);
}


BOOST_AUTO_TEST_CASE(test_clostest_intersection_by_steps)
{
	BOOST_TEST(closest_intersection_by_steps("R8,U5,L5,D3", "U7,R6,D4,L4") == 30);
	BOOST_TEST(closest_intersection_by_steps("R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83") == 610);
	BOOST_TEST(closest_intersection_by_steps("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7") == 410);
}
