#define BOOST_TEST_MODULE day_11

#include <boost/test/included/unit_test.hpp>

#include <cmath>
#include <vector>

#include "read_file.h"
#include "PaintRobot.h"


BOOST_AUTO_TEST_CASE(test_position_set)
{
	PositionSet s;
	s.emplace(0, 0);
}