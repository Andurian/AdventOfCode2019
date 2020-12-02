#define BOOST_TEST_MODULE day_08

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <iostream>

#include "space_image.h"
#include "read_file.h"


BOOST_AUTO_TEST_CASE(test_space_image_layers)
{
	const std::vector<int> data{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 };
	const std::vector<std::vector<int>> layers{ {1, 2, 3, 4, 5, 6}, {7, 8, 9, 0, 1, 2} };

	const std::vector<std::vector<int>> output = split_layers(data, 3, 2);

	BOOST_TEST(layers.size() == output.size());

	for(std::size_t layer = 0; layer < std::min(layers.size(), output.size()); ++layer)
	{
		BOOST_CHECK_EQUAL_COLLECTIONS(layers[layer].begin(), layers[layer].end(), output[layer].begin(), output[layer].end());
	}
}

BOOST_AUTO_TEST_CASE(test_space_image_final_color)
{
	const auto data = split_string("0222112222120000", Converter<int>{});
	const std::vector<int> expected = {0, 1, 1, 0};

	const auto result = final_color(data, 2, 2);

	BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_space_image_to_string)
{
	const auto data = split_string("0222112222120000", Converter<int>{});
	const std::string expected{" 0\n0 "};

	const auto result = to_string(data, 2, 2);

	BOOST_TEST(result == expected);
}
