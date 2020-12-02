#define BOOST_TEST_MODULE intprogram

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <iostream>

#include "Intprogram.h"

BOOST_AUTO_TEST_CASE(test_intprogram_constructible)
{
	Intprogram{std::vector<Integer>{1, 2, 3, 4} };
}